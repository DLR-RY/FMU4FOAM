from conans import ConanFile, CMake, tools


class fmu4foam(ConanFile):
    requires = "pugixml/1.11"
    generators = "cmake"

    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", "", "lib")
        self.copy("*.dylib", "", "lib")
        self.copy("*.a", "../../resources/binaries/linux64", "lib")
        self.copy("*.so", "../../resources/binaries/linux64", "lib")