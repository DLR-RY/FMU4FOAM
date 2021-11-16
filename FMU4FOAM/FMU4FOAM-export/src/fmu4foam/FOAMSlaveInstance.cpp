
#include "fmu4foam/FOAMSlaveInstance.hpp"


#include "cppfmu/cppfmu_cs.hpp"

#include <fstream>
#include <functional>
#include <regex>
#include <iostream>
#include <sstream>

#include <utility>
#include <filesystem>
#include <cstring>


using json = nlohmann::json;
namespace fs = std::filesystem;

namespace fmu4foam
{



FOAMSlaveInstance::FOAMSlaveInstance(std::string instanceName, std::string resources, const cppfmu::Logger& logger, const bool visible)
    : instanceName_(std::move(instanceName))
    , resources_(std::move(resources))
    , logger_(logger)
    , visible_(visible)
    , m_real_()
    , m_integer_()
    , m_boolean_()
    , m_string_()
    , ctx_()
    , sock_(ctx_, zmq::socket_type::rep)
{
    std::cout << "constructor " << std::endl;
    std::cout << "instanceName_ " << instanceName_ << std::endl;
    std::cout << "instanceName_ " << resources_ << std::endl;

    
    auto modelVariables = fs::path(resources_) / "modelVariables.xml";
        
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(modelVariables.c_str());
    doc.print(std::cout);
    std::cout << "modelVariables " << modelVariables << std::endl;

    init_variables(doc);

    // std::cout << "m_real_ " << m_real_ << endl;
    for (const auto& [key, value] : m_real_) {
        std::cout << "r " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_integer_) {
        std::cout << "i " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_boolean_) {
        std::cout << "b " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_string_) {
        std::cout << "s " << key << " = " << value << "; " << std::endl;
    }

    // std::cout << "m_boolean_ " << m_boolean_ << endl;
    // std::cout << "m_integer_ " << m_integer_ << endl;
    // std::cout << "m_string_ " << m_string_ << endl;
        
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

void FOAMSlaveInstance::init_variables(const pugi::xml_document& doc)
{
    // loop xml and init variables vectors
    for (pugi::xml_node tool: doc.children())
    {
        for (pugi::xml_node child: tool.children())
        {
            if (child.child("Real"))
            {
                cppfmu::FMIValueReference ref = child.attribute("valueReference").as_uint();
                m_real_.insert({ref,0.0});
            }
            else if (child.child("Boolean"))
            {
                cppfmu::FMIValueReference ref = child.attribute("valueReference").as_uint();
                m_boolean_.insert({ref,false});
            }
            else if (child.child("Integer"))
            {
                cppfmu::FMIValueReference ref = child.attribute("valueReference").as_uint();
                m_integer_.insert({ref,0});
            }
            else if (child.child("String"))
            {
                cppfmu::FMIValueReference ref = child.attribute("valueReference").as_uint();
                m_string_.insert({ref,""});
            }

            std::cout << "child " << child.name() << std::endl;
            for (pugi::xml_attribute attr: child.attributes())
            {
                std::cout << " " << attr.name() << "=" << attr.value() << std::endl;
                // for (pugi::xml_node c2: child.children())
                // {
                //     std::cout << "c2 " << c2.name() << std::endl;
                // }
            }
        }

    }

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
    sock_.bind("tcp://127.0.0.1:8000")

}

void FOAMSlaveInstance::ExitInitializationMode()
{
    std::cout << "ExitInitializationMode " << std::endl;
}

bool FOAMSlaveInstance::DoStep(cppfmu::FMIReal currentTime, cppfmu::FMIReal stepSize, cppfmu::FMIBoolean, cppfmu::FMIReal& endOfStep)
{
    bool status = true;
    std::cout << "DoStep " << std::endl;

    for (const auto& [key, value] : m_real_) {
        std::cout << "r " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_integer_) {
        std::cout << "i " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_boolean_) {
        std::cout << "b " << key << " = " << value << "; " << std::endl;
    }

    for (const auto& [key, value] : m_string_) {
        std::cout << "s " << key << " = " << value << "; " << std::endl;
    }

    // receive
    json_dumped = self.socket_.recv().decode()
    print(json_dumped)
    d = json.loads(json_dumped)
    t = d["t"]
    print(d)
    
    print("current_time",current_time)
    print("step_size",step_size)
    self.dTout = d["dTout"]
    self.Tout = d["Tout"]

    print("self.dTout",self.dTout)
    print("self.Tout",self.Tout)

    // reply
    d_in ={  
        "Qin": self.Qin,
        "current_time": current_time,
        "step_size": step_size
    }
    print(d_in)
    
    json_object = json.dumps(d_in)
    print(json_object)

    self.socket_.send_string(json_object)


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
    for (std::size_t i = 0; i < nvr; ++i)
    {
        if (m_real_.count(vr[i]))
        {
            m_real_[vr[i]] = values[i];
        }
    }
}

void FOAMSlaveInstance::SetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIInteger* values)
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        m_integer_[vr[i]] = values[i];
        if (m_integer_.count(vr[i]))
        {
            m_integer_[vr[i]] = values[i];
        }
    }
}

void FOAMSlaveInstance::SetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIBoolean* values)
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        if (m_boolean_.count(vr[i]))
        {
            m_boolean_[vr[i]] = values[i];
        }
    }
}

void FOAMSlaveInstance::SetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString const* values)
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        if (m_string_.count(vr[i]))
        {
            m_string_[vr[i]] = values[i];
        }
    }
}

void FOAMSlaveInstance::GetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIReal* values) const
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        values[i] = m_real_.at(vr[i]);
    }
}

void FOAMSlaveInstance::GetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIInteger* values) const
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        values[i] = m_integer_.at(vr[i]);
    }
}

void FOAMSlaveInstance::GetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIBoolean* values) const
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        values[i] = m_boolean_.at(vr[i]);
    }
}

void FOAMSlaveInstance::GetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString* values) const
{
    for (std::size_t i = 0; i < nvr; ++i)
    {
        values[i] = m_string_.at(vr[i]).c_str();
    }
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
