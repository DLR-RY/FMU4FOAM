
#include "fmu4foam/FOAMSlaveInstance.hpp"


#include "cppfmu/cppfmu_cs.hpp"

#include <fstream>
#include <functional>
#include <regex>
#include <sstream>
#include <utility>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include "pugixml.hpp"

namespace fmu4foam
{


FOAMSlaveInstance::FOAMSlaveInstance(std::string instanceName, std::string resources, const cppfmu::Logger& logger, const bool visible)
    : instanceName_(std::move(instanceName))
    , resources_(std::move(resources))
    , logger_(logger)
    , visible_(visible)
    // , m_real_(0)
    // , m_integer_(0)
    // , m_boolean_(0)
    // , m_string_(0)

{
    std::cout << "constructor " << std::endl;
    std::cout << "instanceName_ " << instanceName_ << std::endl;
    std::cout << "instanceName_ " << resources_ << std::endl;
 
    
    auto modelVariables = fs::path(resources_) / "modelVariables.xml";
    // for (const auto & entry : fs::directory_iterator(fs::path(resources_).parent_path() ))
    // {
    //     std::cout << entry.path() << std::endl;
    // }
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(modelVariables.c_str());
    doc.print(std::cout);
    std::cout << "modelVariables " << modelVariables << std::endl;
    std::cout << "try looping "  << std::endl;
    pugi::xml_node node = doc.child("modelVariables");

    for (pugi::xml_node tool = node.child("ScalarVariable"); node; node = node.next_sibling("ScalarVariable"))
    {
        std::cout << "node " << node.attribute("name").value() << "\n";
    }

    std::cout << "still try looping "  << std::endl;
    
    
    for (pugi::xml_node tool: doc.children())
    {
        std::cout << "Tool:";

        for (pugi::xml_node child: tool.children())
        {
            std::cout << "child " << child.name() << std::endl;
            for (pugi::xml_attribute attr: child.attributes())
            {
                std::cout << " " << attr.name() << "=" << attr.value() << std::endl;
                for (pugi::xml_node c2: child.children())
                {
                    std::cout << "c2 " << c2.name() << std::endl;
                }
            }
        }

        std::cout << std::endl;
    }

    std::cout << "modelVariables " << result << std::endl;
    std::cout << "modelVariables " << result.description() << std::endl;
    std::cout << "modelVariables " << result.description() << std::endl;
    // std::string path = "/path/to/directory";

    
    // if (!result)
    //     return -1;
}

void FOAMSlaveInstance::clearLogBuffer() const
{
    clearLogStrBuffer();

    std::cout << "clearLogBuffer " << std::endl;
}

void FOAMSlaveInstance::initialize(fmi2FMUstate gilState)
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
}


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

    return cppfmu::AllocateUnique<fmu4foam::FOAMSlaveInstance>(
        memory, instanceName, resources, logger, visible);
}
