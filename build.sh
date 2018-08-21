rm -rf build/ &&
mkdir build &&
echo "clean build folder."
cd build/ &&
cmake .. &&
make -j8 &&
echo "Build complete!"
