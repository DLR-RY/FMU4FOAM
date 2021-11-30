
#ifndef fmu4foam_SLAVEINSTANCE_HPP
#define fmu4foam_SLAVEINSTANCE_HPP

#include "cppfmu/cppfmu_cs.hpp"
#include <string>
#include <vector>
#include <map>
#include <stdio.h>

#include "pugixml.hpp"
#include <nlohmann/json.hpp>
#include "zmq_addon.hpp"
#include <iostream>

using json = nlohmann::json;


namespace fmu4foam
{


#if defined(_WIN32) || defined(_WIN64)
        const char* os = "Windows";
#else
#ifdef __linux
        const char* os = "Linux";
#else
        const char* os = "Unknown";
#endif
#endif

template <class T>
struct fmuVariable{
    std::string name;
    std::string causality; // input output parameter
    T value;
};


template <class T>
void add_to_json
(
    json& j,
    const std::map<cppfmu::FMIValueReference,fmuVariable<T>>& map_var,
    std::string causality
)
{ 
    if ( !map_var.size() ){
        return;
    }
    for (auto& [key, value] : map_var) {
        if (value.causality == causality)
        {
            j[value.name] = value.value;
        }
    }

}


template <class T>
void get_from_json
(
    const json& j,
    std::map<cppfmu::FMIValueReference,fmuVariable<T>>& map_var,
    std::string causality
)
{

    if ( !map_var.size() ){
        return;
    }
    for (auto& [key, value] : map_var) {
        if (value.causality == causality)
        {
            value.value = j[value.name];
        }
    }
}


template <class T>
const T& get_by_name
(
    const std::map<cppfmu::FMIValueReference,fmuVariable<T>>& map_var,
    std::string name
)
{
    for (const auto& [key, value] : map_var) {
        if (value.name == name)
        {
            return value.value;
        }
    }
} 


class FOAMSlaveInstance : public cppfmu::SlaveInstance
{

public:
    FOAMSlaveInstance(std::string instanceName, std::string resources, const cppfmu::Logger& logger, bool visible);

    void initialize(fmi2FMUstate gilState);

    void SetupExperiment
    (
        cppfmu::FMIBoolean toleranceDefined,
        cppfmu::FMIReal tolerance,
        cppfmu::FMIReal tStart,
        cppfmu::FMIBoolean stopTimeDefined,
        cppfmu::FMIReal tStop
    ) override;
    void EnterInitializationMode() override;
    void ExitInitializationMode() override;
    void Terminate() override;
    void Reset() override;
    bool DoStep(cppfmu::FMIReal currentCommunicationPoint, cppfmu::FMIReal communicationStepSize, cppfmu::FMIBoolean newStep, cppfmu::FMIReal& endOfStep) override;

    void SetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIReal* value) override;
    void SetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIInteger* value) override;
    void SetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, const cppfmu::FMIBoolean* value) override;
    void SetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString const* value) override;

    void GetReal(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIReal* value) const override;
    void GetInteger(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIInteger* value) const override;
    void GetBoolean(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIBoolean* value) const override;
    void GetString(const cppfmu::FMIValueReference* vr, std::size_t nvr, cppfmu::FMIString* value) const override;

    void GetFMUstate(fmi2FMUstate& state) override;
    void SetFMUstate(const fmi2FMUstate& state) override;
    void FreeFMUstate(fmi2FMUstate& state) override;

    size_t SerializedFMUstateSize(const fmi2FMUstate& state) override;
    void SerializeFMUstate(const fmi2FMUstate& state, fmi2Byte bytes[], size_t size) override;
    void DeSerializeFMUstate(const fmi2Byte bytes[], size_t size, fmi2FMUstate& state) override;

    void clearLogBuffer() const;

    

    ~FOAMSlaveInstance() override;

private:

    const bool visible_;
    const std::string instanceName_;
    const std::string resources_;
    const cppfmu::Logger& logger_;

    //- map with real values
    std::map<cppfmu::FMIValueReference,fmuVariable<cppfmu::FMIReal>> m_real_;

    //- map with integer values
    std::map<cppfmu::FMIValueReference,fmuVariable<cppfmu::FMIInteger>> m_integer_;

    //- map with boolean values
    std::map<cppfmu::FMIValueReference,fmuVariable<cppfmu::FMIBoolean>> m_boolean_;

    //- map with string values
    std::map<cppfmu::FMIValueReference,fmuVariable<std::string>> m_string_;

    //- zmq context
    zmq::context_t ctx_;

    //- zmq req socket 
    zmq::socket_t sock_;


    inline void clearStrBuffer() const
    {

    }

    inline void clearLogStrBuffer() const
    {

    }

    void init_variables(const pugi::xml_document& doc);

    std::string read_socket();

    void write_socket(std::string w);

    
};

} // namespace fmu4foam

#endif
