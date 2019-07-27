echo "UNIT TESTS SCRIPT:  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
scp -r ../inputs/ build/ &&
echo "UNIT TESTS SCRIPT: Preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j8 &&
echo "UNIT TESTS SCRIPT: Binaries ready, starting execution..." &&
./bmpExtraction_ut &&
./movementDetector_ut &&
./objectDetection_ut &&
echo "UNIT TESTS SCRIPT: Execution of tests finished." &&
if [ $(dpkg-query -W -f='${Status}' valgrind 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
echo "Valgrind tool not installed, memory check not performed."
fi
if [ $(dpkg-query -W -f='${Status}' valgrind 2>/dev/null | grep -c "ok installed") -eq 1 ];
then
echo "memcheck start..." 
valgrind --leak-check=full --expensive-definedness-checks=yes ./bmpExtraction_ut 
valgrind --leak-check=full --expensive-definedness-checks=yes ./movementDetector_ut 
valgrind --leak-check=full --expensive-definedness-checks=yes ./objectDetection_ut
echo "all tests were performed."
fi
