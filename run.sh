scp -r inputs/ build/ &&
echo "Image samples copied." &&
cd build/ &&
./aquamarine &&
echo "Execution of tests finished."
