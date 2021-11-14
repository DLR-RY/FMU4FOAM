rm -r build
mkdir build
conan install . -s build_type=Release -s compiler.libcxx=libstdc++ --install-folder=build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
