/*---------------------------------------------------------------------------*\
                pdfFoam: General Purpose PDF Solution Algorithm
                   for Reactive Flow Simulations in OpenFOAM

 Copyright (C) 2012 Michael Wild, Heng Xiao, Patrick Jenny,
                    Institute of Fluid Dynamics, ETH Zurich
-------------------------------------------------------------------------------
License
    This file is part of pdfFoam.

    pdfFoam is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) version 3 of the same License.

    pdfFoam is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with pdfFoam.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "mcParticle.H"
#include "IOstreams.H"
#include "mcParticleCloud.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::mcParticle::mcParticle
(
    const Cloud<mcParticle>& cloud,
    Istream& is,
    bool readFields
)
:
#if FOAM_HEX_VERSION < 0x200
    Particle<mcParticle>(cloud, is, readFields),
#else
    particle(cloud.pMesh(), is, readFields),
#endif
    reflectionBoundaryVelocity_(vector::zero),
    reflected_(false),
    reflectedAtOpenBoundary_(false)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            m_ = readScalar(is);
            is  >> positionOld_
                >> celliOld_
                >> faceiOld_
                >> procOld_
                >> UParticle_
                >> UParticleOld_
                >> Ucorrection_
                >> Utracking_
                >> Omega_
                >> rho_
                >> eta_
                >> shift_
                >> Co_
                >> reflectionBoundaryVelocity_
                >> ghost_
                >> nSteps_
                >> isOnInletBoundary_
                >> reflected_
                >> reflectedAtOpenBoundary_
                >> Phi_
                ;
        }
        else
        {
            static const size_t offset = sizeof(BoundaryOfDataMembers);
            static const ptrdiff_t binaryLength =
                &endOfDataMembers_ - &beginOfDataMembers_ - offset;
            is.read
            (
                reinterpret_cast<char*>(&beginOfDataMembers_)+offset,
                binaryLength
            );
            is >> Phi_;
        }
    }

    // Check state of Istream
    is.check("mcParticle::mcParticle(Istream&)");
}


void Foam::mcParticle::readFields(Cloud<mcParticle>& c)
{
    if (!c.size())
    {
        return;
    }
    mcParticleCloud& mcpc = refCast<mcParticleCloud>(c);
    base::readFields(c);

    IOField<scalar> m(c.fieldIOobject("m", IOobject::MUST_READ));
    c.checkFieldIOobject(c, m);

    IOField<vector> UParticle
    (
        c.fieldIOobject("UParticle", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, UParticle);

    IOField<scalar> Omega(c.fieldIOobject("Omega", IOobject::MUST_READ));
    c.checkFieldIOobject(c, Omega);

    IOField<scalar> rho(c.fieldIOobject("rho", IOobject::MUST_READ));
    c.checkFieldIOobject(c, rho);

    IOField<scalar> eta(c.fieldIOobject("eta", IOobject::MUST_READ));
    c.checkFieldIOobject(c, eta);

    PtrList<IOField<scalar> > PhiFields(mcpc.scalarNames().size());
    forAll(mcpc.scalarNames(), PhiI)
    {
        PhiFields.set
        (
            PhiI,
            new IOField<scalar>
            (
                c.fieldIOobject
                (
                    mcpc.scalarNames()[PhiI],
                    IOobject::MUST_READ
                )
            )
        );
        c.checkFieldIOobject(c, PhiFields[PhiI]);
    }

    label i = 0;
    forAllIter(Cloud<mcParticle>, c, iter)
    {
        mcParticle& p = iter();

        p.m_ = m[i];
        p.UParticle_ = UParticle[i];
        p.Omega_ = Omega[i];
        p.rho_ = rho[i];
        p.eta_ = eta[i];
        p.shift_ = vector::zero;
        p.Co_ = 0.;
        p.reflectionBoundaryVelocity_ = vector::zero;
        p.ghost_ = 0;
        p.nSteps_ = 0;
        p.isOnInletBoundary_ = false;
        p.reflected_ = false;
        p.reflectedAtOpenBoundary_ = false;
        p.Phi_.setSize(PhiFields.size());
        forAll(PhiFields, PhiI)
        {
            p.Phi_[PhiI] = PhiFields[PhiI][i];
        }
        i++;
    }
}


void Foam::mcParticle::writeFields(const Cloud<mcParticle>& c)
{
    const mcParticleCloud& mcpc = refCast<const mcParticleCloud>(c);
    base::writeFields(c);

    label np = c.size();

    IOField<scalar> m(c.fieldIOobject("m", IOobject::NO_READ), np);
    IOField<vector> UParticle
    (
        c.fieldIOobject("UParticle", IOobject::NO_READ),
        np
    );
    IOField<vector> Ucorrection
    (
        c.fieldIOobject("Ucorrection", IOobject::NO_READ),
        np
    );
    IOField<scalar> Omega(c.fieldIOobject("Omega", IOobject::NO_READ), np);
    IOField<scalar> rho(c.fieldIOobject("rho", IOobject::NO_READ), np);
    IOField<scalar> eta(c.fieldIOobject("eta", IOobject::NO_READ), np);
    PtrList<IOField<scalar> > PhiFields(mcpc.scalarNames().size());
    forAll(mcpc.scalarNames(), PhiI)
    {
        PhiFields.set
        (
            PhiI,
            new IOField<scalar>
            (
                c.fieldIOobject
                (
                    mcpc.scalarNames()[PhiI],
                    IOobject::NO_READ
                ),
                np
            )
        );
    }

    label i = 0;
    forAllConstIter(Cloud<mcParticle>, c, iter)
    {
        const mcParticle& p = iter();

        m[i] = p.m_;
        UParticle[i] = p.UParticle_;
        Ucorrection[i] = p.Ucorrection_;
        Omega[i] = p.Omega_;
        forAll(PhiFields, PhiI)
        {
            PhiFields[PhiI][i] = p.Phi_[PhiI];
        }
        rho[i] = p.rho_;
        eta[i] = p.eta_;
        i++;
    }

    m.write();
    UParticle.write();
    Ucorrection.write();
    Omega.write();
    rho.write();
    eta.write();
    forAll(PhiFields, PhiI)
    {
        PhiFields[PhiI].write();
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const mcParticle& p)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const mcParticle::base&>(p)
            << token::SPACE << p.m_
            << token::SPACE << p.positionOld_
            << token::SPACE << p.celliOld_
            << token::SPACE << p.faceiOld_
            << token::SPACE << p.procOld_
            << token::SPACE << p.UParticle_
            << token::SPACE << p.UParticleOld_
            << token::SPACE << p.Ucorrection_
            << token::SPACE << p.Utracking_
            << token::SPACE << p.Omega_
            << token::SPACE << p.rho_
            << token::SPACE << p.eta_
            << token::SPACE << p.shift_
            << token::SPACE << p.Co_
            << token::SPACE << p.reflectionBoundaryVelocity_
            << token::SPACE << p.ghost_
            << token::SPACE << p.nSteps_
            << token::SPACE << p.isOnInletBoundary_
            << token::SPACE << p.reflected_
            << token::SPACE << p.reflectedAtOpenBoundary_
            << token::SPACE << p.Phi_;
    }
    else
    {
        os  << static_cast<const mcParticle::base&>(p);
        static const size_t offset =
            sizeof(mcParticle::BoundaryOfDataMembers);
        static const ptrdiff_t binaryLength =
            &p.endOfDataMembers_ - &p.beginOfDataMembers_ - offset;
        os.write
        (
            reinterpret_cast<const char*>(&p.beginOfDataMembers_) + offset,
            binaryLength
        );
        os  << p.Phi_;
    }

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const mcParticle&)");

    return os;
}


// ************************************************************************* //
