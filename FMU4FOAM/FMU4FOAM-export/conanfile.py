from conans import ConanFile, CMake, tools


class fmu4foam(ConanFile):
    requires = "pugixml/1.11"
    generators = "cmake"

    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()