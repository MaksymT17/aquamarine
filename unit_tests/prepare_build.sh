
echo "PREPARE UNIT TESTS SCRIPT:  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
scp -r ../inputs/ build/inputs &&
echo "PREPARE UNIT TESTS SCRIPT: Preparing new build with cmake..." &&
cd build/ &&
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_STANDARD=14 .. &&
echo "PREPARE UNIT TEST SRIPT: Cmake generated files for build"
