echo "UNIT TESTS SCRIPT:  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
echo "UNIT TESTS SCRIPT: Preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j8 &&
echo "UNIT TESTS SCRIPT: Binaries ready, starting execution..." &&
./bmpExtraction_ut &&
./movementDetector_ut &&
echo "UNIT TESTS SCRIPT: Execution of tests finished."
