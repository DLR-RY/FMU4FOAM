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

#include "pyFMUSim.H"
#include "pyInterp.H"
#include "sigFpe.H"

namespace py = pybind11;
// using namespace py::literals;

namespace Foam
{
    defineTypeNameAndDebug(pyFMUSim, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
Foam::pyFMUSim::pyFMUSim
(
    const Time& time,
    word pymodule,
    word pyclass
)
: 
    FMUs_()
{
    pyInterp::New(time);

    // numpy causes a float point exception of loaded with OpenFOAM 
    // sigFpe so we temporally deactivate sigFpe we will only loose the 
    // stacktrace if deactivated
    sigFpe::unset(false);
    py::object pyC = py::module_::import(pymodule.c_str()).attr(pyclass.c_str());
    
    // construct FMUs with endTime add margin
    // if stepUntil is bigger than endTime the FMU may not exit
    scalar endTime = time.endTime().value()*1.1;
    FMUs_ = pyC(endTime,"FMU.json");
    sigFpe::set(false);
}
// ************************************************************************* //
