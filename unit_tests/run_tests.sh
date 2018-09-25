echo "Unit tests.  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
echo "preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j8 &&
./aquamarine_ut &&
echo "Execution of Unit tests finished."
