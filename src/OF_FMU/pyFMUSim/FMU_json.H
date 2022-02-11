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

Description
    Defines the conversion from json to registered objects 

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/
#include <nlohmann/json.hpp>
#include "registeredObject.H"

using json = nlohmann::json;

namespace Foam
{

    
    void to_json(json& j, const registeredObject<scalar>& s)
    {
        j[s.name()] = s.ref();
    }


    void from_json(const json& j, registeredObject<scalar>& s)
    {
        s.ref() = j[s.name()];
    }


    void add_to_json(json& j, const registeredObject<scalar>& s)
    {
        j[s.name()] = s.ref();
    }


    void to_json(json& j, const registeredObject<vector>& s)
    {
        const vector& sref = s.ref();
        j[s.name()] = {sref.x(),sref.y(),sref.z()};
    }


    void from_json(const json& j, registeredObject<vector>& s)
    {
        vector& sref = s.ref();
        sref.x() = j[s.name() + "_x"];
        sref.y() = j[s.name() + "_y"];
        sref.z() = j[s.name() + "_z"];
    }


    void add_to_json(json& j, const registeredObject<vector>& s)
    {
        const vector& sref = s.ref();
        j[s.name() + "_x"] = sref.x();
        j[s.name() + "_y"] = sref.y();
        j[s.name() + "_z"] = sref.z();
    }


    template<class T>
    void add_to_json(json& j,const objectRegistry& obj)
    {
        forAllConstIters(obj, iter)
        {
            const regIOobject* obj = iter.val();
            if (isType<registeredObject<T>>(*obj))
            {
                const registeredObject<T>& regObj =
                    refCast<const registeredObject<T> > (*obj);

                add_to_json(j,regObj);

            }
        }
    }


    template<class T>
    void get_from_json(const json& j,objectRegistry& obj)
    {
        forAllIters(obj, iter)
        {
            regIOobject* obj = iter.val();
            if (isType<registeredObject<T>>(*obj))
            {
                registeredObject<T>& regObj =
                    refCast<registeredObject<T> > (*obj);

                from_json(j,regObj);

            }
        }
    }

}
