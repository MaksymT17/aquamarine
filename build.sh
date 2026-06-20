echo "Removing existing build folder ..."
rm -rf build/ &&
if [ ! -d "silber" ]; then
    echo "Clone silber sources from repo."
    git clone --branch "1.2" --depth 1 https://github.com/MaksymT17/silber
fi
mkdir build &&
echo "preparing new build with cmake..." &&
cd build/ &&
cmake .. &&
if command -v sysctl >/dev/null 2>&1; then CORES=$(sysctl -n hw.ncpu); else CORES=$(nproc); fi
make -j$CORES &&
echo "Build complete!"
