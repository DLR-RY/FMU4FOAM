/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 OpenFOAM Foundation
    Copyright (C) 2017-2019 OpenCFD Ltd.
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

Application
    foamDictionary

Description
    Interrogate and manipulate dictionaries.

Usage
    \b foamDictionary [OPTION] dictionary

      - \par -entry \<name\>
        Selects an entry

      - \par -keywords
        Prints the keywords (of the selected entry or of the top level if
        no entry was selected

      - \par -add \<value\>
        Adds the entry (should not exist yet)

      - \par -set \<value\>
        Adds or replaces the entry

      - \par -remove
        Remove the selected entry

      - \par -diff \<dictionary\>
        Write differences with respect to the specified dictionary
        (or sub entry if -entry specified)

      - \par -diff-etc \<dictionary\>
        Write differences with respect to the specified dictionary
        (or sub entry if -entry specified)

      - \par -expand
        Read the specified dictionary file, expand the macros etc. and write
        the resulting dictionary to standard output.

      - \par -includes
        List the \c \#include and \c \#sinclude files to standard output

      - \par -disableFunctionEntries
        Do not expand macros or directives (\#include etc)

      - \par -precision int
        Set default write precision for IOstreams

    Example usage:
      - Change simulation to run for one timestep only:
        \verbatim
          foamDictionary system/controlDict -entry stopAt -set writeNow
        \endverbatim

      - Change solver:
        \verbatim
           foamDictionary system/fvSolution -entry solvers.p.solver -set PCG
        \endverbatim

      - Print bc type:
        \verbatim
           foamDictionary 0/U -entry boundaryField.movingWall.type
        \endverbatim

      - Change bc parameter:
        \verbatim
           foamDictionary 0/U -entry boundaryField.movingWall.value \
             -set "uniform (2 0 0)"
        \endverbatim

      - Change whole bc type:
        \verbatim
          foamDictionary 0/U -entry boundaryField.movingWall \
            -set "{type uniformFixedValue; uniformValue (2 0 0);}"
        \endverbatim

      - Write the differences with respect to a template dictionary:
        \verbatim
          foamDictionary 0/U -diff-etc templates/closedVolume/0/U
        \endverbatim

      - Write the differences in boundaryField with respect to a
        template dictionary:
        \verbatim
          foamDictionary 0/U -diff-etc templates/closedVolume/0/U \
            -entry boundaryField
        \endverbatim

      - Change patch type:
        \verbatim
          foamDictionary constant/polyMesh/boundary \
            -entry entry0.fixedWalls.type -set patch
        \endverbatim
        This uses special parsing of Lists which stores these in the
        dictionary with keyword 'entryDDD' where DDD is the position
        in the dictionary (after ignoring the FoamFile entry).

    Notes:
        - the use of '.' as the scoping symbol might conflict with
        e.g. file extensions ('.' is not really considered
        to be a valid word character). Instead use the '/' as a scoping
        character e.g.
          foamDictionary system/snappyHexMeshDict \
            -entry /geometry/motorBike.obj -remove

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "profiling.H"
#include "Time.H"
#include "Fstream.H"
#include "etcFiles.H"
#include "includeEntry.H"
#include <nlohmann/json.hpp>
#include <iomanip>
#include "stringOps.H"
#include <string> 
#include <iostream>

using namespace Foam;
using json = nlohmann::json;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Convert older ':' scope syntax to newer '.' scope syntax,
//  but leave anything with '/' delimiters untouched
bool upgradeScope(word& entryName)
{
    if
    (
        entryName.find('/') == string::npos
     && entryName.find(':') != string::npos
    )
    {
        const wordList names(fileName(entryName).components(':'));

        entryName.resize(0);

        for (const word& name : names)
        {
            if (entryName.size()) entryName.append(".");

            entryName.append(name);
        }

        return true;
    }

    // Nothing changed
    return false;
}


//- Split into dictionary name and the entry name
class dictAndKeyword
{
    word dict_;
    word key_;

public:

    dictAndKeyword(const word& scopedName)
    {
        auto i = scopedName.rfind('/');
        if (i == string::npos)
        {
            i = scopedName.rfind('.');
        }

        if (i != string::npos)
        {
            dict_ = scopedName.substr(0, i);
            key_  = scopedName.substr(i+1);
        }
        else
        {
            key_  = scopedName;
        }
    }

    inline const word& dict() const
    {
        return dict_;
    }

    inline const word& key() const
    {
        return key_;
    }
};


const dictionary& lookupScopedDict
(
    const dictionary& dict,
    const word& subDictName
)
{
    if (subDictName.empty())
    {
        return dict;
    }

    const entry* eptr = dict.findScoped(subDictName, keyType::LITERAL);

    if (!eptr || !eptr->isDict())
    {
        FatalIOErrorInFunction(dict)
            << "'" << subDictName << "' not found in dictionary "
            << dict.name() << " or is not a dictionary" << nl
            << "Known entries are " << dict.keys()
            << exit(FatalIOError);
    }

    return eptr->dict();
}

void modifyFromJson(const word& fileAndKey,const string& newValue)
{

    word delimiter = ";";
    const auto split = stringOps::splitAny(fileAndKey, delimiter);

    if (split.size() != 2 && split.size() != 1)
    {
        FatalErrorInFunction
            << "key words of the json file does not split into two string with the delimiter=;  " << nl
            << "the syntax is <filename>;keyword e.g. system/fvSchemes;gradSchemes/default" << nl
            << "if only the entry is specified gradSchemes/default it is assumed that the dict-file is " << nl
            << "system/simulationParameters" << nl
            << fileAndKey
            << exit(FatalError, 1);
    }

 
    fileName dictFileName = "system/simulationParameters";
    word scopedName = split[0].str();
    if (split.size() == 2)
    {   
        dictFileName = split[0].str(); // set from fileAndKey
        scopedName = split[1].str();
    }
    Info << "setting key " << scopedName << " in " << dictFileName << " to " << newValue << endl;
    upgradeScope(scopedName);

    autoPtr<IFstream> dictFile(new IFstream(dictFileName));
    if (!dictFile().good())
    {
        FatalErrorInFunction
            << "Cannot open file " << dictFileName
            << exit(FatalError, 1);
    }
    // Read but preserve headers
    dictionary dict(dictFile(), true);


    // Dictionary name and keyword
    const dictAndKeyword dAk(scopedName);

    // The context for the action
    const dictionary& d(lookupScopedDict(dict, dAk.dict()));

    // Create a new entry
    IStringStream str(string(dAk.key()) + ' ' + newValue + ';');
    entry* ePtr(entry::New(str).ptr());

    // overwrite entry
    const_cast<dictionary&>(d).set(ePtr);

    // write dict
    dictFile.clear();
    OFstream os(dictFileName);
    IOobject::writeBanner(os);
    IOobject::writeDivider(os);
    dict.write(os, false);
    IOobject::writeEndDivider(os);


}


int main(int argc, char *argv[])
{
    argList::addNote
    (
        "set dictionary from json"
    );

    argList::addArgument("json", "The json file to process");

    profiling::disable(); // Disable profiling (and its output)

    argList args(argc, argv);

    // does not expand the function so it will not copy the includes
    // variables in the dictionary otherwise the order of the updates
    // matters
    entry::disableFunctionEntries = true;

    // read json from file
    std::ifstream i(args[1]);
    json j;
    i >> j;

    // modify all dict-entries specified in the json
    for (auto& el : j.items()) {
        std::ostringstream ss;
        ss << el.value();
        string value = ss.str();
        if (el.value().is_string())
        {
            value = el.value();
        }
        modifyFromJson(el.key(),value);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
