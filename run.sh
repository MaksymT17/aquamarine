scp -r image_inputs/ build/ &&
echo "Image sample copied." &&
./build/recognition &&
echo "Execution of tests finished."
