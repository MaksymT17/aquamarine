scp -r image_inputs/ build/ &&
echo "Image samples copied." &&
./build/aquamarine &&
echo "Execution of tests finished."
