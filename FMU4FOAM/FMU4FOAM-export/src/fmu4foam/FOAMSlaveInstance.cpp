
#include "fmu4foam/FOAMSlaveInstance.hpp"

#include "fmu4foam/PyState.hpp"

#include "cppfmu/cppfmu_cs.hpp"

#include <fstream>
#include <functional>
#include <regex>
#include <sstream>
#include <utility>

namespace fmu4foam
{


FOAMSlaveInstance::FOAMSlaveInstance(std::string instanceName, std::string resources, const cppfmu::Logger& logger, const bool visible)
    : instanceName_(std::move(instanceName))
    , resources_(std::move(resources))
    , logger_(logger)
    , visible_(visible)
{
    std::cout << "constructor " << std::endl;
}

void FOAMSlaveInstance::clearLogBuffer() const
{
    clearLogStrBuffer();

    std::cout << "clearLogBuffer " << std::endl;
}

void FOAMSlaveInstance::initialize(PyGILState_STATE gilState)
{
    std::cout << "initialize " << std::endl;
}

void FOAMSlaveInstance::SetupExperiment(cppfmu::FMIBoolean, cppfmu::FMIReal, cppfmu::FMIReal startTime, cppfmu::FMIBoolean, cppfmu::FMIReal)
{
    std::cout << "SetupExperiment " << std::endl;
}

void FOAMSlaveInstance::EnterInitializationMode()
{
    std::cout << "SetupExperEnterInitializationModeiment " << std::endl;
}

void FOAMSlaveInstance::ExitInitializationMode()
{
    std::cout << "ExitInitializationMode " << std::endl;
}

bool FOAMSlaveInstance::DoStep(cppfmu::FMIReal currentTime, cppfmu::FMIReal stepSize, cppfmu::FMIBoolean, cppfmu::FMIReal& endOfStep)
{
    bool status = true;
    std::cout << "DoStep " << std::endl;

    return status;
}

void FOAMSlaveInstance::Reset()
{
    std::cout << "Reset " << std::endl;
}

void FOAMSlaveInstance::Terminate()
{
    std::cout << "Terminate " << std::endl;
}

void FOAMSlaveInstance::SetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIReal* values)
{
    std::cout << "SetReal " << std::endl;
}

void FOAMSlaveInstance::SetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIInteger* values)
{
    std::cout << "SetInteger " << std::endl;
}

void FOAMSlaveInstance::SetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIBoolean* values)
{
    std::cout << "SetBoolean " << std::endl;
}

void FOAMSlaveInstance::SetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString const* values)
{
    std::cout << "SetString " << std::endl;
}

void FOAMSlaveInstance::GetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIReal* values) const
{
    std::cout << "GetReal " << std::endl;
}

void FOAMSlaveInstance::GetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIInteger* values) const
{
    std::cout << "GetInteger " << std::endl;
}

void FOAMSlaveInstance::GetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIBoolean* values) const
{
    std::cout << "GetBoolean " << std::endl;
}

void FOAMSlaveInstance::GetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString* values) const
{
    std::cout << "GetString " << std::endl;
}

void FOAMSlaveInstance::GetFMUstate(fmi2FMUstate& state)
{
    std::cout << "GetFMUstate " << std::endl;
}

void FOAMSlaveInstance::SetFMUstate(const fmi2FMUstate& state)
{
    std::cout << "SetFMUstate " << std::endl;
}

void FOAMSlaveInstance::FreeFMUstate(fmi2FMUstate& state)
{
    std::cout << "FreeFMUstate " << std::endl;
}

size_t FOAMSlaveInstance::SerializedFMUstateSize(const fmi2FMUstate& state)
{
    size_t size = 0;
    std::cout << "SerializedFMUstateSize " << std::endl;
    return size;
}

void FOAMSlaveInstance::SerializeFMUstate(const fmi2FMUstate& state, fmi2Byte* bytes, size_t size)
{
    std::cout << "SerializeFMUstate " << std::endl;
}

void FOAMSlaveInstance::DeSerializeFMUstate(const fmi2Byte bytes[], size_t size, fmi2FMUstate& state)
{
    std::cout << "DeSerializeFMUstate " << std::endl;



FOAMSlaveInstance::~FOAMSlaveInstance()
{
    std::cout << "Destructor FOAMSlaveInstance " << std::endl;
}

} // namespace fmu4foam



cppfmu::UniquePtr<cppfmu::SlaveInstance> CppfmuInstantiateSlave(
    cppfmu::FMIString instanceName,
    cppfmu::FMIString,
    cppfmu::FMIString fmuResourceLocation,
    cppfmu::FMIString,
    cppfmu::FMIReal,
    cppfmu::FMIBoolean visible,
    cppfmu::FMIBoolean,
    cppfmu::Memory memory,
    const cppfmu::Logger& logger)
{

    auto resources = std::string(fmuResourceLocation);
    auto find = resources.find("file://");

    if (find != std::string::npos) {
#ifdef _MSC_VER
        resources.replace(find, 8, "");
#else
        resources.replace(find, 7, "");
#endif
    }

    if (pyState == nullptr) {
        pyState = std::make_unique<fmu4foam::PyState>();
    }

    return cppfmu::AllocateUnique<fmu4foam::FOAMSlaveInstance>(
        memory, instanceName, resources, logger, visible);
}
