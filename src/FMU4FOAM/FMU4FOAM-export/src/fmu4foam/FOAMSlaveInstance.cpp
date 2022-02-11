
#include "fmu4foam/FOAMSlaveInstance.hpp"

#include "extract.h"

#include "cppfmu/cppfmu_cs.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <utility>

using json = nlohmann::json;
#include <filesystem>
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
    auto modelVariables = fs::path(resources_) / "modelParameters.xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(modelVariables.c_str());

    init_variables(doc);
}

void FOAMSlaveInstance::init_variables(const pugi::xml_document& doc)
{
    // loop xml and init variables vectors
    for (pugi::xml_node tool : doc.children()) {
        for (pugi::xml_node child : tool.children()) {
            std::string name = child.attribute("name").as_string();
            std::string causality = child.attribute("causality").as_string();
            cppfmu::FMIValueReference ref = child.attribute("valueReference").as_uint();
            
            if (child.child("Real")) {
                const auto& c_real = child.child("Real");
                cppfmu::FMIReal value = c_real.attribute("start").as_double();
                fmuVariable<cppfmu::FMIReal> var = {name, causality, value};
                m_real_.insert({ref, var});
            } else if (child.child("Boolean")) {
                const auto& c_real = child.child("Boolean");
                cppfmu::FMIBoolean value = c_real.attribute("start").as_bool();
                fmuVariable<cppfmu::FMIBoolean> var = {name, causality, value};
                m_boolean_.insert({ref, var});
            } else if (child.child("Integer")) {
                const auto& c_real = child.child("Integer");
                cppfmu::FMIInteger value = c_real.attribute("start").as_int();
                fmuVariable<cppfmu::FMIInteger> var = {name, causality, value};
                m_integer_.insert({ref, var});
            } else if (child.child("String")) {
                const auto& c_real = child.child("String");
                std::string value = c_real.attribute("start").as_string();
                fmuVariable<std::string> var = {name, causality, value};
                m_string_.insert({ref, var});
            }

        }
    }
}

void FOAMSlaveInstance::clearLogBuffer() const
{
    clearLogStrBuffer();
}

void FOAMSlaveInstance::initialize(fmi2FMUstate gilState)
{
    
}

void FOAMSlaveInstance::SetupExperiment(
    cppfmu::FMIBoolean toleranceDefined,
    cppfmu::FMIReal tolerance,
    cppfmu::FMIReal tStart,
    cppfmu::FMIBoolean stopTimeDefined,
    cppfmu::FMIReal tStop)
{
    std::string address = "tcp://*";
    address += ":" + std::to_string(get_by_name(m_integer_, "port"));
    std::cout << "connecting to  " << address << std::endl;

    sock_.bind(address);

    auto OFCase = fs::path(resources_) / "of_case.tar.gz";
    std::string s_ofcase = OFCase.u8string();

    std::cout << "OFCase " << OFCase << std::endl;
    std::string outputPath = get_by_name(m_string_, "outputPath");
    ::extract(s_ofcase.c_str(), outputPath.c_str());

    json dictMod;

    dictMod["system/controlDict;endTime"] = tStop;
    add_to_json(dictMod, m_string_, "parameter");
    add_to_json(dictMod, m_real_, "parameter");
    add_to_json(dictMod, m_integer_, "parameter");
    add_to_json(dictMod, m_boolean_, "parameter");

    fs::path caseDir = fs::path(outputPath.c_str()) / fs::path("dictMod.json");
    std::cout << "caseDir " << caseDir.u8string() << std::endl;
    std::ofstream o(caseDir.u8string());
    std::cout << std::setw(4) << dictMod << std::endl;
    o << std::setw(4) << dictMod << std::endl;
}

void FOAMSlaveInstance::EnterInitializationMode()
{
    // call Allrun
    std::string outputPath = get_by_name(m_string_, "outputPath");
    std::string oscmd = get_by_name(m_string_, "oscmd");
    std::string arguments = get_by_name(m_string_, "arguments");
    std::string allrun_path = (fs::path(outputPath.c_str()) / fs::path("Allrun")).generic_string();
    std::string allrun = oscmd + " " + allrun_path + " " + arguments + " &";
    std::cout << "running  " << allrun << std::endl;
    int res = system(allrun.c_str());

    std::string recv = read_socket();
    json j_recv = json::parse(recv);

    get_from_json(j_recv, m_real_, "output");
    get_from_json(j_recv, m_integer_, "output");
    get_from_json(j_recv, m_boolean_, "output");
    get_from_json(j_recv, m_string_, "output");

    json j_out;

    j_out["current_time"] = 0.0;
    j_out["step_size"] = 0.0;
    add_to_json(j_out, m_real_, "input");
    add_to_json(j_out, m_integer_, "input");
    add_to_json(j_out, m_boolean_, "input");
    add_to_json(j_out, m_string_, "input");

    std::string send = j_out.dump();
    write_socket(send);
}

void FOAMSlaveInstance::ExitInitializationMode()
{

}

std::string FOAMSlaveInstance::read_socket()
{
    zmq::message_t z_in;
    sock_.recv(z_in);
    std::string read_str = z_in.to_string();
    return read_str;
}

void FOAMSlaveInstance::write_socket(std::string w)
{
    zmq::message_t z_out(w);
    sock_.send(z_out, zmq::send_flags::none);
}


bool FOAMSlaveInstance::DoStep(cppfmu::FMIReal currentTime, cppfmu::FMIReal stepSize, cppfmu::FMIBoolean, cppfmu::FMIReal& endOfStep)
{
    bool status = true;

    std::string recv = read_socket();
    json j_recv = json::parse(recv);

    get_from_json(j_recv, m_real_, "output");
    get_from_json(j_recv, m_integer_, "output");
    get_from_json(j_recv, m_boolean_, "output");
    get_from_json(j_recv, m_string_, "output");

    json j_out;

    j_out["current_time"] = currentTime;
    j_out["step_size"] = stepSize;

    add_to_json(j_out, m_real_, "input");
    add_to_json(j_out, m_integer_, "input");
    add_to_json(j_out, m_boolean_, "input");
    add_to_json(j_out, m_string_, "input");

    std::string send = j_out.dump();
    write_socket(send);

    return status;
}

void FOAMSlaveInstance::Reset()
{
    
}

void FOAMSlaveInstance::Terminate()
{
    std::string recv = read_socket();
    json j_recv = json::parse(recv);

    json j_out;
    j_out["terminate"] = true;
    std::string send = j_out.dump();
    write_socket(send);
}

void FOAMSlaveInstance::SetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIReal* values)
{
    for (std::size_t i = 0; i < nvr; ++i) {
        if (m_real_.count(vr[i])) {
            m_real_[vr[i]].value = values[i];
        }
    }
}

void FOAMSlaveInstance::SetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIInteger* values)
{
    for (std::size_t i = 0; i < nvr; ++i) {
        if (m_integer_.count(vr[i])) {
            m_integer_[vr[i]].value = values[i];
        }
    }
}

void FOAMSlaveInstance::SetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIBoolean* values)
{
    for (std::size_t i = 0; i < nvr; ++i) {
        if (m_boolean_.count(vr[i])) {
            m_boolean_[vr[i]].value = values[i];
        }
    }
}

void FOAMSlaveInstance::SetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString const* values)
{
    for (std::size_t i = 0; i < nvr; ++i) {
        if (m_string_.count(vr[i])) {
            m_string_[vr[i]].value = values[i];
        }
    }
}

void FOAMSlaveInstance::GetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIReal* values) const
{
    for (std::size_t i = 0; i < nvr; ++i) {
        values[i] = m_real_.at(vr[i]).value;
    }
}

void FOAMSlaveInstance::GetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIInteger* values) const
{
    for (std::size_t i = 0; i < nvr; ++i) {
        values[i] = m_integer_.at(vr[i]).value;
    }
}

void FOAMSlaveInstance::GetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIBoolean* values) const
{
    for (std::size_t i = 0; i < nvr; ++i) {
        values[i] = m_boolean_.at(vr[i]).value;
    }
}

void FOAMSlaveInstance::GetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString* values) const
{
    for (std::size_t i = 0; i < nvr; ++i) {
        values[i] = m_string_.at(vr[i]).value.c_str();
    }
}

void FOAMSlaveInstance::GetFMUstate(fmi2FMUstate& state)
{

}

void FOAMSlaveInstance::SetFMUstate(const fmi2FMUstate& state)
{


void FOAMSlaveInstance::FreeFMUstate(fmi2FMUstate& state)
{

}

size_t FOAMSlaveInstance::SerializedFMUstateSize(const fmi2FMUstate& state)
{
    size_t size = 0;
    return size;
}

void FOAMSlaveInstance::SerializeFMUstate(const fmi2FMUstate& state, fmi2Byte* bytes, size_t size)
{

}

void FOAMSlaveInstance::DeSerializeFMUstate(const fmi2Byte bytes[], size_t size, fmi2FMUstate& state)
{

}


FOAMSlaveInstance::~FOAMSlaveInstance()
{

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
