scp -r image_inputs/ build/ &&
echo "Image samples copied." &&
./build/recognition &&
echo "Execution of tests finished."
