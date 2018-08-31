scp -r image_inputs/ build/ &&
echo "Image samples copied." &&
./build/aq &&
echo "Execution of tests finished."
