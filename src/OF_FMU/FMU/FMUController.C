/*---------------------------------------------------------------------------*\
            Copyright (c) 2021, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the VoFLibrary source code library, which is an
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
#include "json.H"

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
    extIOList_(runTime)
{
    Info << "Foam::functionObjects::FMUController::FMUController  " << endl;
    read(dict);
    commDataLayer& data = commDataLayer::New(time_);
    data.storeObj(scalar(0.0),"step_size",commDataLayer::causality::in);
    data.storeObj(scalar(0.0),"current_time",commDataLayer::causality::in);

    auto& extFunc = extIOList_.functions();
    for(auto& f: extFunc)
    {
        f.execute();
    }

    // sync vars 
    const auto& regOut = data.getRegistry(commDataLayer::causality::out);
    json jOutput;
    forAllConstIters(regOut, iter)
    {
        const regIOobject* obj = iter.val();
        if (isType<registeredObject<scalar>>(*obj))
        {
            const registeredObject<scalar>& regScalar =
                refCast<const registeredObject<scalar> > (*obj);

            add_to_json(jOutput,regScalar);

        }
        if (isType<registeredObject<vector>>(*obj))
        {

            const registeredObject<vector>& regVector =
                refCast<const registeredObject<vector> > (*obj);

            add_to_json(jOutput,regVector);
        }
    }

    Info << "j dumped " << word(jOutput.dump()) << endl;
    sock_.write(word(jOutput.dump()));
    // read section
    const auto& regIn = data.getRegistry(commDataLayer::causality::in);

    word recv = sock_.read();
    Info << "recv " << recv << endl;
    json input = json::parse(recv);

    // input.is_number()
    for (auto& el : input.items())
    {
        std::cout << el.key() << " : " << el.value() << "\n";
        if (input[el.key()].is_number())
        {
            scalar& obj = data.getObj<scalar>(el.key(),commDataLayer::causality::in);
            obj = el.value();
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::FMUController::read(const dictionary& dict)
{
    functionObject::read(dict);

    return true;
}


bool Foam::functionObjects::FMUController::adjustTimeStep()
{
    Info << "adjustTimeStep " << endl;
    Info << "time_.timeIndex() " << time_.timeIndex() << endl;
    commDataLayer& data = commDataLayer::New(time_);
    scalar currentTime = data.getObj<scalar>("current_time",commDataLayer::causality::in);
    scalar step_size = data.getObj<scalar>("step_size",commDataLayer::causality::in);

     // sync times
    scalar newDeltaT = time_.deltaTValue();
    if (time_.timeIndex() > 0)
    {
        newDeltaT = currentTime + step_size - time_.value();
    }
    Info << "target time value " << currentTime + step_size  << " : "  << time_.value() + newDeltaT << endl;

    static label index = -1;

    if (time_.timeIndex() != index)
    {
        // Store current time so we don't get infinite recursion (since
        // setDeltaT calls adjustTimeStep() again)
        index = time_.timeIndex();

        // Set time, allow deltaT to be adjusted for writeInterval purposes
        const_cast<Time&>(time_).setDeltaT(newDeltaT, false);
        Info << "newDeltaT is " << newDeltaT << endl;
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
    Info << "target time value " << currentTime + step_size  << " currentTime: "  << time_.value() << endl;

    const auto& regOut = data.getRegistry(commDataLayer::causality::out);
    json jOutput;
    jOutput["t"]  = time_.value();
    jOutput["dt"] = time_.deltaTValue();
    forAllConstIters(regOut, iter)
    {
        const regIOobject* obj = iter.val();
        if (isType<registeredObject<scalar>>(*obj))
        {
            const registeredObject<scalar>& regScalar =
                refCast<const registeredObject<scalar> > (*obj);

            add_to_json(jOutput,regScalar);

        }
        if (isType<registeredObject<vector>>(*obj))
        {

            const registeredObject<vector>& regVector =
                refCast<const registeredObject<vector> > (*obj);

            add_to_json(jOutput,regVector);
        }
    }

    Info << "j dumped " << word(jOutput.dump()) << endl;
    sock_.write(word(jOutput.dump()));
    // read section
    const auto& regIn = data.getRegistry(commDataLayer::causality::in);

    word recv = sock_.read();
    Info << "recv " << recv << endl;
    json input = json::parse(recv);

    // input.is_number()
    for (auto& el : input.items())
    {
        std::cout << el.key() << " : " << el.value() << "\n";
        if (input[el.key()].is_number())
        {
            scalar& obj = data.getObj<scalar>(el.key(),commDataLayer::causality::in);
            obj = el.value();
        }
    }

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
