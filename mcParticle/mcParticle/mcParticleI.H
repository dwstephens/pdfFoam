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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

#if FOAM_HEX_VERSION < 0x200
inline Foam::label& Foam::mcParticle::face()
{
    return facei_;
}
#endif


inline const Foam::point& Foam::mcParticle::positionOld() const
{
    return positionOld_;
}


inline Foam::point& Foam::mcParticle::positionOld()
{
    return positionOld_;
}


inline Foam::label Foam::mcParticle::cellOld() const
{
    return celliOld_;
}


inline Foam::label& Foam::mcParticle::cellOld()
{
    return celliOld_;
}


inline Foam::label Foam::mcParticle::faceOld() const
{
    return faceiOld_;
}


inline Foam::label& Foam::mcParticle::faceOld()
{
    return faceiOld_;
}


inline Foam::label Foam::mcParticle::procOld() const
{
    return procOld_;
}


inline Foam::label& Foam::mcParticle::procOld()
{
    return procOld_;
}


inline Foam::scalar Foam::mcParticle::m() const
{
    return m_;
}


inline Foam::scalar& Foam::mcParticle::m()
{
    return m_;
}


inline const Foam::vector& Foam::mcParticle::UParticle() const
{
    return UParticle_;
}


inline Foam::vector& Foam::mcParticle::UParticle()
{
    return UParticle_;
}


inline const Foam::vector& Foam::mcParticle::UParticleOld() const
{
    return UParticle_;
}


inline Foam::vector& Foam::mcParticle::UParticleOld()
{
    return UParticle_;
}


inline const Foam::vector& Foam::mcParticle::Ucorrection() const
{
    return Ucorrection_;
}


inline Foam::vector& Foam::mcParticle::Ucorrection()
{
    return Ucorrection_;
}


inline const Foam::vector& Foam::mcParticle::Utracking() const
{
    return Utracking_;
}


inline Foam::vector& Foam::mcParticle::Utracking()
{
    return Utracking_;
}


inline Foam::scalar Foam::mcParticle::Omega() const
{
  return Omega_;
}


inline Foam::scalar& Foam::mcParticle::Omega()
{
  return Omega_;
}


inline const Foam::scalarField& Foam::mcParticle::Phi() const
{
  return Phi_;
}


inline Foam::scalarField& Foam::mcParticle::Phi()
{
  return Phi_;
}


inline Foam::scalar Foam::mcParticle::rho() const
{
  return rho_;
}


inline Foam::scalar& Foam::mcParticle::rho()
{
  return rho_;
}

inline Foam::scalar Foam::mcParticle::eta() const
{
  return eta_;
}

inline Foam::scalar& Foam::mcParticle::eta()
{
  return eta_;
}

inline Foam::label Foam::mcParticle::ghost() const
{
    return ghost_;
}


inline Foam::label& Foam::mcParticle::ghost()
{
    return ghost_;
}


inline Foam::label Foam::mcParticle::nSteps() const
{
    return nSteps_;
}


inline Foam::label& Foam::mcParticle::nSteps()
{
    return nSteps_;
}


inline const Foam::vector& Foam::mcParticle::shift() const
{
    return shift_;
}


inline Foam::vector& Foam::mcParticle::shift()
{
    return shift_;
}


inline Foam::scalar Foam::mcParticle::Co() const
{
    return Co_;
}


inline Foam::scalar& Foam::mcParticle::Co()
{
    return Co_;
}


inline const Foam::vector& Foam::mcParticle::reflectionBoundaryVelocity() const
{
    return reflectionBoundaryVelocity_;
}


inline Foam::vector& Foam::mcParticle::reflectionBoundaryVelocity()
{
    return reflectionBoundaryVelocity_;
}


inline bool Foam::mcParticle::isOnInletBoundary() const
{
    return isOnInletBoundary_;
}


inline bool& Foam::mcParticle::isOnInletBoundary()
{
    return isOnInletBoundary_;
}


inline bool Foam::mcParticle::reflected() const
{
    return reflected_;
}


inline bool& Foam::mcParticle::reflected()
{
    return reflected_;
}


inline bool Foam::mcParticle::reflectedAtOpenBoundary() const
{
    return reflectedAtOpenBoundary_;
}


inline bool& Foam::mcParticle::reflectedAtOpenBoundary()
{
    return reflectedAtOpenBoundary_;
}

// ************************************************************************* //
