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

#include "FMUSimulator.H"
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
    defineTypeNameAndDebug(FMUSimulator, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        FMUSimulator,
        dictionary
    );
}
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::FMUSimulator::FMUSimulator
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    oms_
    (
        runTime,
        dict.get<word>("pyFileName").c_str(),
        dict.get<word>("pyClassName").c_str()
    ),
    time_(runTime),
    extIOList_(runTime)
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::FMUSimulator::read(const dictionary& dict)
{
    functionObject::read(dict);

    return true;
}


bool Foam::functionObjects::FMUSimulator::execute()
{
    auto& extFunc = extIOList_.functions();
    for(auto& f: extFunc)
    {
        f.execute();
    }
    Info << "execute" << endl;

    commDataLayer& data = commDataLayer::New(time_);

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

    oms_.from_OF(word(jOutput.dump()));

    oms_.stepUntil(time_.value());
    word out  = oms_.to_OF();
    auto jout = json::parse(out);

    for (auto& el : jout.items())
    {
        scalar& var = data.getObj<scalar>(el.key(),commDataLayer::causality::in);
        var = el.value();
    }

    return true;
}


bool Foam::functionObjects::FMUSimulator::write()
{
    return true;
}


bool Foam::functionObjects::FMUSimulator::end()
{
    return true;
}


// ************************************************************************* //
