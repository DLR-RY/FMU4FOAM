/*---------------------------------------------------------------------------*\
            Copyright (c) 2021, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the FMU4FOAM source code library, which is an
	unofficial extension to OpenFOAM.
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


Author
    Henning Scheufler, DLR, all rights reserved.

Class
    Foam::functionObjects::FMUSimulator

Group
    grpUtilitiesFunctionObjects

Description


Usage


Note


See also
    Foam::functionObject
    Foam::functionObjects::timeControl

SourceFiles
    FMUSimulator.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_FMUSimulator_H
#define functionObjects_FMUSimulator_H

#include "functionObject.H"
#include "stringList.H"
#include "pyFMUSim.H"
#include "externalIOObjectList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class FMUSimulator Declaration
\*---------------------------------------------------------------------------*/

class FMUSimulator
:
    public functionObject
{
private:

    // Private data

    //- 
    pyFMUSim oms_;

    //- time
    const Time& time_;

    externalIOObjectList extIOList_;

    // Private Member Functions
    template<class T>
    void sync_regObjects(objectRegistry& obj);

    //- No copy construct
    FMUSimulator(const FMUSimulator&) = delete;

    //- No copy assignment
    void operator=(const FMUSimulator&) = delete;


public:

    //- Runtime type information
    TypeName("FMUSimulator");


    // Constructors

        //- Construct from Time and dictionary
        FMUSimulator
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~FMUSimulator() = default;


    // Member Functions

        //- Read the system calls
        virtual bool read(const dictionary& dict);

        //- Execute the "executeCalls" at each time-step
        virtual bool execute();

        //- Write, execute the "writeCalls"
        virtual bool write();

        //- Execute the "endCalls" at the final time-loop
        virtual bool end();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
