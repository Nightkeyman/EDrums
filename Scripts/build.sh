# Doesn't work yet
cd ../
cmake -S . -B ./build -G "Unix Makefiles"
cd ./build
make
cd ./EdrumController
./drumtests.exe
