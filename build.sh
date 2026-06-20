echo "Removing existing build folder ..."
rm -rf build/ &&
mkdir build &&
echo "preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j$(sysctl -n hw.ncpu) &&
echo "Build complete!"
