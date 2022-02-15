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

\*---------------------------------------------------------------------------*/

#include "FMUController.H"
#include "Time.H"
#include "dynamicCode.H"
#include "foamVersion.H"
#include "addToRunTimeSelectionTable.H"
#include "commDataLayer.H"
#include <nlohmann/json.hpp>
#include "registeredObject.H"
#include "FMU_json.H"

using json = nlohmann::json;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(FMUController, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        FMUController,
        dictionary
    );
}
}

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T>
void Foam::functionObjects::FMUController::sync_regObjects(objectRegistry& obj)
{
    forAllIters(obj, iter)
    {
        regIOobject* obj = iter.val();
        if (isType<registeredObject<T>>(*obj))
        {
            registeredObject<T>& regObj =
                refCast<registeredObject<T> > (*obj);

            Pstream::scatter(regObj.ref());

        }
    }
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::FMUController::FMUController
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    sock_(dict),
    time_(runTime),
    targetTime_(-GREAT),
    extIOList_(runTime)
{
    read(dict);
    commDataLayer& data = commDataLayer::New(time_);
    data.storeObj(scalar(0.0),"step_size",commDataLayer::causality::in);
    data.storeObj(scalar(0.0),"current_time",commDataLayer::causality::in);

    auto& extFunc = extIOList_.functions();
    for(auto& f: extFunc)
    {
        f.execute();
    }

    if (Pstream::master())
    {
        // send
        const auto& regOut = data.getRegistry(commDataLayer::causality::out);
        json jOutput;
        jOutput["t"]  = time_.value();
        jOutput["dt"] = time_.deltaTValue();

        // build output json
        add_to_json<scalar>(jOutput,regOut);
        add_to_json<vector>(jOutput,regOut);

        sock_.write(word(jOutput.dump()));
        
        // receive
        auto& regIn = data.getRegistry(commDataLayer::causality::in);

        word recv = sock_.read();
        json input = json::parse(recv);

        // update input data
        get_from_json<scalar>(input,regIn);
        get_from_json<vector>(input,regIn);

    }

    auto& regIn = data.getRegistry(commDataLayer::causality::in);

    sync_regObjects<scalar>(regIn);
    sync_regObjects<vector>(regIn);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::FMUController::read(const dictionary& dict)
{
    functionObject::read(dict);

    return true;
}


bool Foam::functionObjects::FMUController::adjustTimeStep()
{
    commDataLayer& data = commDataLayer::New(time_);
    scalar currentTime = data.getObj<scalar>("current_time",commDataLayer::causality::in);
    scalar step_size = data.getObj<scalar>("step_size",commDataLayer::causality::in);

    if (targetTime_ >= time_.value() + time_.deltaTValue())
    {
        return true;
    }

    // sync times
    scalar newDeltaT = time_.deltaTValue();
    if (time_.timeIndex() > 0)
    {
        if (newDeltaT > step_size)
        {
            newDeltaT = step_size;
        }
    }

    static label index = -1;

    if (time_.timeIndex() != index)
    {
        // Store current time so we don't get infinite recursion (since
        // setDeltaT calls adjustTimeStep() again)
        index = time_.timeIndex();

        // Set time, allow deltaT to be adjusted for writeInterval purposes
        const_cast<Time&>(time_).setDeltaT(newDeltaT, true);
    }

    return true;
}

bool Foam::functionObjects::FMUController::execute()
{
    auto& extFunc = extIOList_.functions();
    for(auto& f: extFunc)
    {
        f.execute();
    }

    commDataLayer& data = commDataLayer::New(time_);

    scalar currentTime = data.getObj<scalar>("current_time",commDataLayer::causality::in);
    scalar step_size = data.getObj<scalar>("step_size",commDataLayer::causality::in);
    targetTime_ = currentTime + step_size;
    if (targetTime_ >= time_.value() + time_.deltaTValue())
    {
        return true;
    }

    if (Pstream::master())
    {
        // send
        const auto& regOut = data.getRegistry(commDataLayer::causality::out);
        json jOutput;
        jOutput["t"]  = time_.value();
        jOutput["dt"] = time_.deltaTValue();

        // build output json
        add_to_json<scalar>(jOutput,regOut);
        add_to_json<vector>(jOutput,regOut);

        sock_.write(word(jOutput.dump()));
        
        // receive
        auto& regIn = data.getRegistry(commDataLayer::causality::in);

        word recv = sock_.read();
        json input = json::parse(recv);
        if (input.contains("terminate"))
        {
            time_.stopAt(Foam::Time::saWriteNow);
        }
        else
        {
            // update input data
            get_from_json<scalar>(input,regIn);
            get_from_json<vector>(input,regIn);
        }
    }

    auto& regIn = data.getRegistry(commDataLayer::causality::in);

    sync_regObjects<scalar>(regIn);
    sync_regObjects<vector>(regIn);

    return true;
}


bool Foam::functionObjects::FMUController::write()
{
    return true;
}


bool Foam::functionObjects::FMUController::end()
{
    return true;
}


// ************************************************************************* //
