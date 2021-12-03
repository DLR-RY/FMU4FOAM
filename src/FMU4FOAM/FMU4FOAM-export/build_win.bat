mkdir build
conan install . -s build_type=Release --install-folder=build -s compiler="Visual Studio" -s compiler.version=15
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -A x64
cmake --build . --config Release
cd ..
