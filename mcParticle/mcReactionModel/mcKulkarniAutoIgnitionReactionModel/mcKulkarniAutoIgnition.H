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

Class
    Foam::mcKulkarniAutoIgnitionReactionModel

Description
    Auto-iginition model using tabulated chemistry as proposed by Kulkarni and
    Polifke.

    This auto-ignition model follows the description given by Kulkarni and
    Polifke @cite kulkarni:2012. Reads
    constant/autoIgnition/{z,pv,cEq,cdot,rho} (mixture fraction, normalised
    progress variable, equilibrium progress variable values, progress variable
    source term and density) and additional fields specified in the "scalars"
    list. z and pv must be monothonically increasing. cEq must be list the
    (non-normalised) equilibrium progress variable value for every entry in z.
    If Nz is the number of entries in z and Npv the number of entries in pv,
    then cdot, rho and all the fields specified in "scalars" must be lists of
    lists, the outer list of size Nz, the inner of size Npv.

SourceFiles
    mcKulkarniAutoIgnitionReactionModel.C

Author
    Michael Wild

SeeAlso
    http://dx.doi.org/10.1155/2012/780370

\*---------------------------------------------------------------------------*/

#ifndef mcKulkarniAutoIgnitionReactionModel_H
#define mcKulkarniAutoIgnitionReactionModel_H

#include "mcReactionModel.H"

#include "autoPtr.H"
#include "scalarIOList.H"
#if FOAM_HEX_VERSION >= 0x200
#include "scalarListIOList.H"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

template<class> class interpolation;

/*---------------------------------------------------------------------------*\
             Class mcKulkarniAutoIgnitionReactionModel Declaration
\*---------------------------------------------------------------------------*/

class mcKulkarniAutoIgnitionReactionModel :
    public mcReactionModel
{

    // Private Data

        //- Name of the mixture-fraction field
        const word zName_;

        //- Name of the progress-variable field
        const word cName_;

        //- Mixture-fraction, normalised progress-variable vectors and
        // equilibrium progress variable values
        const scalarIOList z_, pv_, cEq_;

        //- Number of z and chi data points
        const label Nz_, Npv_;

        //- Maximum equilibirum progress variable
        const scalar cEqMax_;

        //- Names of the additional interpolated fields
        wordList addedNames_;

        //- Interpolation tables
        PtrList<scalarListIOList> phi_;

        //- Indexes of the mixture fraction and progress variables properties
        const label zIdx_, cIdx_, pvIdx_;

        //- Indexes of the additional interpolated fields
        labelList addedIdx_;

        //- Number of native fields (progress variable c and density rho for
        // now)
        static const label nNativeFields_ = 2;

    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        mcKulkarniAutoIgnitionReactionModel(const mcKulkarniAutoIgnitionReactionModel&);
        void operator=(const mcKulkarniAutoIgnitionReactionModel&);

public:

    //- Runtime type information
    TypeName("mcKulkarniAutoIgnitionReactionModel");

    // Constructors

        //- Construct from components
        mcKulkarniAutoIgnitionReactionModel
        (
            mcParticleCloud& cloud,
            const objectRegistry& db,
            const word& subDictName
        );

    // Member Functions

        //- Apply the reaction model to a single particle
        virtual void correct(mcParticle& p);

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
