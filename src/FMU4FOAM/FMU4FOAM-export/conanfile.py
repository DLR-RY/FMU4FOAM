import os
from conans import ConanFile, CMake, tools


class fmu4foam(ConanFile):
    requires = ["pugixml/1.11","nlohmann_json/3.9.1","cppzmq/4.7.1","libarchive/3.5.2"]
    generators = "cmake"

    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()