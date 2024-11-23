cd shaders
echo "Compiling shaders..."
./compile.sh
cd ..
cmake --build build --target run
