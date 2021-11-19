import os
from conans import ConanFile, CMake, tools


class fmu4foam(ConanFile):
    requires = ["pugixml/1.11","nlohmann_json/3.9.1","cppzmq/4.7.1","libarchive/3.5.2"]
    generators = "cmake"

    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()

    def imports(self):
        
        self.copy("*.dll", os.path.join('..','..','resources','binaries','win64'), "bin")
        self.copy("*.lib", os.path.join('..','..','resources','binaries','win64'), "lib")
        self.copy("*.a", os.path.join('..','..','resources','binaries','linux64'), "lib")
        self.copy("*.so", os.path.join('..','..','resources','binaries','linux64'), "lib")