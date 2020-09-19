rm -rf build
mkdir build
cd build
cmake ..
make
./test/terrarium_simulation_test
cd ..
