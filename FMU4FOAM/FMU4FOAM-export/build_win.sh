mkdir build
conan install . -s build_type=Release --install-folder=build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -A x64
cmake --build . --config Release
