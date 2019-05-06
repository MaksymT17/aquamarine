echo "removing existing build folder..."
rm -rf build/ &&
mkdir build &&
echo "preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j8 &&
echo "Build complete!"
