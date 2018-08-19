rm -rf build/ &&
mkdir build &&
scp -r image_inputs/ build/ &&
echo "sample images copied."
cd build/ &&
cmake .. &&
make -j8 &&
echo "Build complete!"
