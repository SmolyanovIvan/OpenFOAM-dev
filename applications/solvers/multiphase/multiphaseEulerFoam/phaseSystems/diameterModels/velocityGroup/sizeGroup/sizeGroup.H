/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::diameterModels::sizeGroup

Description
    This class represents a single sizeGroup belonging to a velocityGroup.
    The main property of a sizeGroup is its representative volume. The
    corresponding volScalarField f'number'.'phaseName'.'populationBalanceName'
    gives the volume fraction of the sizeGroup such that all sizeGroup fractions
    over a velocityGroup must sum to unity. The field is either read from the
    startTime directory if present or constructed from a reference field called
    f.'phaseName'.'populationBalanceName' where the boundary condition types
    must be specified. All field and boundary condition values are reset to
    match the 'value' given in the sizeGroup subdictionary.

    The corresponding physical diameter that is representative for the class is
    computed by a separate shapeModel, specified in the velocityGroupCoeffs
    dictionary.

Usage
    \table
        Property     | Description                | Required    | Default value
        dSph         | Sphere equivalent diameter | yes         | none
        value        | Field and BC value         | yes         | none
    \endtable

    Example
    \verbatim
    f1
    {
        ds      3e-3;
        value   1.0;
    }
    \endverbatim

See also
    Foam::diameterModels::velocityGroup
    Foam::diameterModels::populationBalanceModel

SourceFiles
    sizeGroup.C

\*---------------------------------------------------------------------------*/

#ifndef sizeGroup_H
#define sizeGroup_H

#include "dictionaryEntry.H"
#include "velocityGroup.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diameterModels
{

class shapeModel;

/*---------------------------------------------------------------------------*\
                          Class sizeGroup Declaration
\*---------------------------------------------------------------------------*/

class sizeGroup
:
    public volScalarField
{
private:

    // Private Data

        dictionary dict_;

        //- Phase this  sizeGroup belongs to
        const phaseModel& phase_;

        //- VelocityGroup this sizeGroup belongs to
        const velocityGroup& velocityGroup_;

        //- Sphere equivalent diameter of the sizeGroup
        const dimensionedScalar dSph_;

        //- Representative volume of the sizeGroup
        const dimensionedScalar x_;

        //- Initial value and value at boundaries
        const scalar value_;

        //- Label of this sizeGroup within the corresponding populationBalance
        mutable autoPtr<label> i_;

        //- Model for describing the representative shape of the elements in the
        //  sizeGroup
        autoPtr<shapeModel> shapeModel_;


public:

    // Constructors

        sizeGroup
        (
            const word& name,
            const dictionary& dict,
            const phaseModel& phase,
            const velocityGroup& velocityGroup,
            const fvMesh& mesh
        );

        //- Return clone
        autoPtr<sizeGroup> clone() const;

        //- Return a pointer to a new sizeGroup created on freestore
        //  from Istream
        class iNew
        {
            const phaseModel& phase_;
            const velocityGroup& velocityGroup_;

        public:

            iNew
            (
                const phaseModel& phase,
                const velocityGroup& velocityGroup
            )
            :
                phase_(phase),
                velocityGroup_(velocityGroup)
            {}

            autoPtr<sizeGroup> operator()(Istream& is) const
            {
                dictionaryEntry ent(dictionary::null, is);
                return autoPtr<sizeGroup>
                (
                    new sizeGroup
                    (
                        ent.keyword(),
                        ent,
                        phase_,
                        velocityGroup_,
                        phase_.mesh()
                    )
                );
            }
        };


    //- Destructor
    virtual ~sizeGroup();


    // Member Functions

        //- Return const-reference to the dictionary
        inline const dictionary& dict() const;

        //- Return const-reference to the phase
        inline const phaseModel& phase() const;

        //- Return const-reference to the velocityGroup
        inline const velocityGroup& VelocityGroup() const;

        //- Return representative spherical diameter of the sizeGroup
        inline const dimensionedScalar& dSph() const;

        //- Return representative volume of the sizeGroup
        inline const dimensionedScalar& x() const;

        //- Return reference to diameterModel of the phase
        inline autoPtr<shapeModel>& shapeModelPtr();

        //- Return const-reference to diameterModel of the phase
        inline const autoPtr<shapeModel>& shapeModelPtr() const;

        //- Return label of the sizeGroup within the population balance
        const autoPtr<label>& i() const;

        //- Return representative surface area of the sizeGroup
        const tmp<volScalarField> a() const;

        //- Return representative diameter of the sizeGroup
        const tmp<volScalarField> d() const;

        //- Correct secondary properties
        void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diameterModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "sizeGroupI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
