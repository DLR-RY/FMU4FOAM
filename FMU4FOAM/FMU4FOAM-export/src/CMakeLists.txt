
set(headers
        cppfmu/cppfmu_cs.hpp
        cppfmu/cppfmu_common.hpp

        fmi/fmi2Functions.h
        fmi/fmi2FunctionTypes.h
        fmi/fmi2TypesPlatform.h

        fmu4foam/FOAMSlaveInstance.hpp
        )

set(sources
        cppfmu/cppfmu_cs.cpp
        cppfmu/fmi_functions.cpp
        fmu4foam/FOAMSlaveInstance.cpp
        )

add_library(fmu4foam-export ${sources} ${headers})
target_compile_features(fmu4foam-export PUBLIC "cxx_std_17")

target_include_directories(fmu4foam-export
        PRIVATE
        "${PUGIXML_INCLUDE_DIRS}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_link_libraries(fmu4foam-export
        PRIVATE
        ${CONAN_LIBS}
        )
        
if (WIN32)
  set_target_properties(fmu4foam-export
          PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../resources/binaries/${TARGET_PLATFORM}"
          )
else ()
  set_target_properties(fmu4foam-export
          PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../resources/binaries/${TARGET_PLATFORM}"
          )
endif ()
