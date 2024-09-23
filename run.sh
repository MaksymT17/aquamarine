scp -rp inputs/ build/inputs &&
echo "Image samples copied." &&
cd build/ &&
./aquamarine &&
echo "Execution of 'aquamarine' program is finished."
