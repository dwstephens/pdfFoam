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

#include "mcMuradogluPositionCorrection.H"

#include "addToRunTimeSelectionTable.H"
#include "fvCFD.H"
#include "interpolation.H"
#include "mcParticleCloud.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

    defineTypeNameAndDebug(mcMuradogluPositionCorrection, 0);
    addNamedToRunTimeSelectionTable
    (
        mcPositionCorrection,
        mcMuradogluPositionCorrection,
        mcPositionCorrection,
        Muradoglu
    );

} // namespace Foam

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::mcMuradogluPositionCorrection::mcMuradogluPositionCorrection
(
    mcParticleCloud& cloud,
    const objectRegistry& db,
    const word& subDictName
)
:
    mcEllipticRelaxationPositionCorrection(cloud, db, subDictName),

    phi_
    (
        IOobject
        (
            thermoDict().lookupOrDefault<word>("phiPosCorrName", "phiPosCorr"),
            db.time().timeName(),
            db,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        cloud.mesh()
    ),

    gradPhi_
    (
        IOobject
        (
            "mcMuradogluPositionCorrection::grad(phi)",
            db.time().timeName(),
            db,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cloud.mesh(),
        dimVelocity,
        zeroGradientFvPatchScalarField::typeName
    )
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void Foam::mcMuradogluPositionCorrection::updateInternals()
{
    readCoeffs();
    U0_.value() = gMax(mag(cloud().Ufv().internalField()));
    if (U0_.value() < VSMALL)
    {
        WarningIn("mcMuradogluPositionCorrection::updateInternals()")
            << "max(mag(" << cloud().Ufv().name() << ")) is very small. "
            << "The position correction will have (almost) no effect."
            << nl;
    }
    // frequency helper variable
    volScalarField omega = c_*U0_/L();
    QInst_ = (cloud().pndcPdfInst() - cloud().rhocPdfInst())/cloud().rhocPdf();

    // need to set deltaT to that of the particles
    Time& runTime = const_cast<Time&>(db().time());
    runTime.subCycle
    (
        ceil(runTime.deltaT().value()/max(cloud().deltaT().value(), 1e-6))
    );

    // solve for mean normalized density difference
    solve
    (
        fvm::ddt(Q_)
        ==
      - fvm::Sp(omega, Q_) + omega*QInst_
      + f_*U0_*L()*fvm::laplacian(Q_)
    );

    // time-integrator for correction potential
    solve(fvm::ddt(phi_) == b_*sqr(U0_)*QInst_);

    // indicator variable
    zeta_ = pos(cloud().pndcPdfInst()/cloud().rhocPdfInst() - eps_);

    // note: gradInterpolationConstantTet gives really bad results.
    gradPhi_ = fvc::grad(phi_);
    gradQInst_ = fvc::grad(QInst_);
    gradQ_ = fvc::grad(Q_);
    gradPhiInterp_ = interpolation<vector>::New
    (
        cloud().solutionDict().interpolationScheme(gradPhi_.name()),
        gradPhi_
    );
    gradQInstInterp_ = interpolation<vector>::New
    (
        cloud().solutionDict().interpolationScheme(gradQInst_.name()),
        gradQInst_
    );
    gradQInterp_ = interpolation<vector>::New
    (
        cloud().solutionDict().interpolationScheme(gradQ_.name()),
        gradQ_
    );
    zetaInterp_ = interpolation<scalar>::New
    (
        cloud().solutionDict().interpolationScheme(zeta_.name()),
        zeta_
    );
    // reset deltaT
    runTime.endSubCycle();
}


void Foam::mcMuradogluPositionCorrection::correct(mcParticle& part)
{
    const point& p = part.position();
    label c = part.cell();
    label f = part.face();
    scalar z = zetaInterp_().interpolate(p, c, f);
    dimensionedScalar l = LInterp_().interpolate(p, c, f);
    part.Ucorrection() -=
        gradPhiInterp_().interpolate(p, c, f)
      + (a_*U0_*l).value()
      * (z != 0 ? gradQInterp_ : gradQInstInterp_)().interpolate(p, c, f);
}

// ************************************************************************* //
