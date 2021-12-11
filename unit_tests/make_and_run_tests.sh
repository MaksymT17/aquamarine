
# mv: in case of modification existing tests use this script
# to reconfigure build use prepare_build.sh
cd build &&
make -j8 &&
echo "UNIT TESTS SCRIPT: Binaries ready, starting execution..." &&
./bmpExtraction_ut &&
./movementDetector_ut &&
./objectDetection_ut &&
./matrix_ut &&
echo "UNIT TESTS SCRIPT: Execution of tests finished." &&
if [ $(dpkg-query -W -f='${Status}' valgrind 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
echo "Valgrind tool not installed, memory check has not been performed."
fi
if [ $(dpkg-query -W -f='${Status}' valgrind 2>/dev/null | grep -c "ok installed") -eq 1 ];
then
echo "memcheck start..." 
valgrind --leak-check=full --expensive-definedness-checks=yes ./bmpExtraction_ut 
valgrind --leak-check=full --expensive-definedness-checks=yes ./movementDetector_ut 
valgrind --leak-check=full --expensive-definedness-checks=yes ./objectDetection_ut
valgrind --leak-check=full --expensive-definedness-checks=yes ./matrix_ut
echo "all tests were performed."
fi
