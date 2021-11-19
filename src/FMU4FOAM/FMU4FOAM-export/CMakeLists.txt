cmake_minimum_required(VERSION 3.15)
project(fmu4foam-export VERSION 0.0.1)

# ==============================================================================
# Build settings
# ==============================================================================

set(BUILD_SHARED_LIBS ON)

# set(CMAKE_SKIP_BUILD_RPATH FALSE)

# # when building, don't use the install RPATH already
# # (but later on when installing)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# set(CMAKE_INSTALL_RPATH "")

# # don't add the automatically determined parts of the RPATH
# # which point to directories outside the build tree to the install RPATH
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)


set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON) # 
# ==============================================================================
# Global internal configuration
# ==============================================================================

set(CONAN_INSTALL_CMD "conan install")
MESSAGE("COMMAND: ${CONAN_INSTALL_CMD} ${CMAKE_CURRENT_SOURCE_DIR}")
execute_process(COMMAND bash -c "${CONAN_INSTALL_CMD} ${CMAKE_CURRENT_SOURCE_DIR}"
   RESULT_VARIABLE CMD_ERROR)

MESSAGE( STATUS "CMD_ERROR:" ${CMD_ERROR})

message("CMAKE_BINARY_DIR is ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_BINARY_DIR is ${CMAKE_CURRENT_BINARY_DIR}")

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS KEEP_RPATHS)
else ()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
endif ()

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (MSVC)
  # https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
  set(CompilerFlags
          CMAKE_CXX_FLAGS
          CMAKE_CXX_FLAGS_DEBUG
          CMAKE_CXX_FLAGS_RELEASE
          CMAKE_C_FLAGS
          CMAKE_C_FLAGS_DEBUG
          CMAKE_C_FLAGS_RELEASE
          )
  foreach (CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach ()
endif ()

# Automatically export all symbols in Windows DLLs.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ==============================================================================
# Dependencies
# ==============================================================================

# Force to use stable Python ABI https://docs.python.org/3/c-api/stable.html
# add_compile_definitions(Py_LIMITED_API)
# find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# set(Python3_FIND_STRATEGY LOCATION)
# if (WIN32)
#   set(Python3_LIBRARIES ${Python3_LIBRARY_DIRS}/python3.lib)
# endif ()
# find_package(PUGIXML)




if (WIN32)
  set(TARGET_PLATFORM win)
elseif (APPLE)
  set(TARGET_PLATFORM darwin)
else ()
  set(TARGET_PLATFORM linux)
endif ()

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
  set(TARGET_PLATFORM ${TARGET_PLATFORM}64)
else ()
  set(TARGET_PLATFORM ${TARGET_PLATFORM}32)
endif ()

message("Building fmu4foam-export for platform ${TARGET_PLATFORM}")

add_subdirectory(src)
