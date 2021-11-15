import os, pathlib
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

cwd = os.getcwd()
WINDOWS = (os.name == 'nt')

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        print(os.getcwd())
        print(os.getcwd())
        if not os.path.exists(os.path.join(cwd,"FMU4FOAM/FMU4FOAM-export","tmp-build")):
            os.mkdir(os.path.join(cwd,"FMU4FOAM/FMU4FOAM-export","tmp-build"))
            print("current dir",os.getcwd())
            os.chdir(os.path.join(cwd,"FMU4FOAM/FMU4FOAM-export"))
            print("current dir",os.getcwd())
            conan_args = [
                'conan',
                'install',
                '.',
                'FMU4FOAM/0.01@FMU4FOAM/FMU4FOAM '
                'build_type=Release -s compiler.libcxx=libstdc++ --install-folder=tmp-build'
            ]

            subprocess.check_call(conan_args)

            os.chdir("tmp-build")
            build_type = 'Release'
            # configure
            cmake_args = [
                'cmake',
                '..',
                '-DCMAKE_BUILD_TYPE={}'.format(build_type)
            ]
            if WINDOWS:
                cmake_args.append('-A x64')
            print(os.getcwd())
            subprocess.check_call(cmake_args)
            cmake_args_build = [
                'cmake',
                '--build',
                '.'
            ]
            if WINDOWS:
                cmake_args.append('--config Release')
            subprocess.check_call(cmake_args_build)
            os.chdir("../../..")
            print(os.getcwd())

class CMakeBuild(build_ext):
    def run(self):
        print("CMakeBuild CMakeBuild CMakeBuild")
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        pass

# See setup.cfg for Python package parameters
setup(
    ext_modules=[CMakeExtension('.')],
    cmdclass=dict(build_ext=CMakeBuild),
)