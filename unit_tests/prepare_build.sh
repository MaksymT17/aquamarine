
echo "PREPARE UNIT TESTS SCRIPT:  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
scp -r ../inputs/ build/ &&
echo "PREPARE UNIT TESTS SCRIPT: Preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
echo "PREPARE UNIT TEST SRIPT: Cmake generated files for build"
