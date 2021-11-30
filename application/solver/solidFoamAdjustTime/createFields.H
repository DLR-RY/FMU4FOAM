Info<< "Reading thermophysical properties\n" << endl;

autoPtr<solidThermo> pThermo(solidThermo::New(mesh));
solidThermo& thermo = pThermo();

thermo.correct();

tmp<volScalarField> trho = thermo.rho();

autoPtr<coordinateSystem> coordinatesPtr;
autoPtr<volSymmTensorField> taniAlpha;

if (!thermo.isotropic())
{
    Info<< "Adding coordinateSystem\n" << endl;
    coordinatesPtr = coordinateSystem::New
    (
        mesh,
        thermo,
        coordinateSystem::typeName_()
    );

    tmp<volVectorField> tkappaByCp = thermo.Kappa()/thermo.Cp();

    taniAlpha.reset
    (
        new volSymmTensorField
        (
            IOobject
            (
                "Anialpha",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionedSymmTensor(tkappaByCp().dimensions(), Zero),
            zeroGradientFvPatchSymmTensorField::typeName
        )
    );
    volSymmTensorField& aniAlpha = *taniAlpha;

    aniAlpha.primitiveFieldRef() =
        coordinatesPtr->transformPrincipal
        (
            mesh.cellCentres(),
            tkappaByCp()
        );
    aniAlpha.correctBoundaryConditions();
}


IOobject betavSolidIO
(
    "betavSolid",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
);

autoPtr<volScalarField> betavPtr;
if (betavSolidIO.typeHeaderOk<volScalarField>(true))
{
    betavPtr.reset
    (
        new volScalarField
        (
            betavSolidIO,
            mesh
        )
    );
}
else
{
    betavPtr.reset
    (
        new volScalarField
        (
            IOobject
            (
                "betavSolid",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionedScalar("1", dimless, scalar(1))
        )
    );
}

#include "createRadiationModel.H"
#include "createFvOptions.H"
