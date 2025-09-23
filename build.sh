echo "Removing existing build folder ..."
rm -rf build/ &&
echo "Clone silber sources from repo." &&
rm -rf silber/
git clone --branch "1.1" --depth 1 https://github.com/MaksymT17/silber &&
mkdir build &&
echo "preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
make -j4 &&
echo "Build complete!"
