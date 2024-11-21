clang-format -style=llvm -i src/*.cpp
clang-format -style=llvm -i src/*.h
cmake --build build --target run
