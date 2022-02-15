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
    Foam::functionObjects::FMUController

Group
    grpUtilitiesFunctionObjects

Description
    connects to the FMU instance and is responsible for the communication

Usage
    add to system/controlDict.functions
    also the dictionary include the dictionary simulationParameters
    at the top of system controlDict
    #include       "simulationParameters"

    \verbatim
    in system/controlDict.functions
    FMUController
    {
        type            FMUController;
        libs            (FMUController);
        host            $host;
        port            $port;
    }
    in system/simulationParameters (can be changed by the FMU)
    host            "127.0.0.1";
    port            8000;

    \endverbatim

Note


See also
    Foam::functionObject
    Foam::functionObjects::timeControl

SourceFiles
    FMUController.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_FMUController_H
#define functionObjects_FMUController_H

#include "functionObject.H"
#include "stringList.H"
#include "zmq_socket.H"
#include "externalIOObjectList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class FMUController Declaration
\*---------------------------------------------------------------------------*/

class FMUController
:
    public functionObject
{
private:

    // Private data
    //- zmq socket for network comms
    zmq_socket sock_;

    //- time 
    const Time& time_;

    //- next communication point
    scalar targetTime_; 

    //- List of externalIOObject 
    externalIOObjectList extIOList_;

    // Private Member Functions

        template<class T>
        void sync_regObjects(objectRegistry& obj);

        //- No copy construct
        FMUController(const FMUController&) = delete;

        //- No copy assignment
        void operator=(const FMUController&) = delete;


public:

    //- Runtime type information
    TypeName("FMUController");


    // Constructors

        //- Construct from Time and dictionary
        FMUController
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~FMUController() = default;


    // Member Functions

        //- Read the system calls
        virtual bool read(const dictionary& dict);

        //- Called at the end of Time::adjustDeltaT() if adjustTime is true
        virtual bool adjustTimeStep();

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
