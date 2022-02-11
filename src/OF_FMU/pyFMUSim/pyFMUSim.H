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

Class
    Foam::pyFMUSim

Description

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef pyFMUSim_H
#define pyFMUSim_H

#include <pybind11/embed.h>
#include "typeInfo.H"
#include "word.H"
#include "Time.H"

namespace py = pybind11;
// using namespace py::literals;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam 
{

class pyFMUSim
{
private:
    py::object FMUs_;


public:

    //- Runtime type information
    TypeName("pyFMUSim");

    // Constructors
    pyFMUSim
    (
        const Time& time,
        word pymodule,
        word pyclass
    );

    virtual ~pyFMUSim() = default;


    py::object& FMUs()
    {
        return FMUs_;
    }


    void stepUntil(scalar t)
    {
        FMUs_.attr("stepUntil")(t);
    }
    

    void from_OF(word dumped_json)
    {
        FMUs_.attr("from_OF")(dumped_json.c_str());
    }


    word to_OF()
    {
        return FMUs_.attr("to_OF")().cast<std::string>();
    }
    

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
