echo "UNIT TESTS SCRIPT:  Removing existing build folder..."
rm -rf build/ &&
mkdir build &&
scp -r ../inputs/ build/ &&
echo "Cloning googletest repo..." &&
rm -rf googletest &&

if ! git clone https://github.com/google/googletest.git
then
  echo "$(tput setaf 1)Cloning of googletest has been failed. The environment is incomplete."
else
  echo "$(tput setaf 2)The environment for the unit tests is ready."
fi
