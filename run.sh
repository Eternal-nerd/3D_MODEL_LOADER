echo "formatting code..."
clang-format -style=llvm -i src/*.cpp
#clang-format -style=llvm -i src/*.h

cd shaders
echo "compiling shaders..."
./compile.sh
cd ..

cmake --build build --target run
