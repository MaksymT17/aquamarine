` aquamarine `

## Initial idea:
Tool for recognitions/detections/analyzing images, for searching differences in pictures, can help in the detection of movement, appearances/disappearances of objects, change of weather/light conditions.

## Target: 
Find optimal solutions for implementation and figure out how to make recognition of image characteristics in an appropriate form and inside of predefined limitations in a config file. Parameters in the config file should cover all significant properties about calculation, should be taken into consideration target Platform and HW limitations. Every improvement ideas are welcome!

## How to build sources and run the program:

Ubuntu:

apt-get -y install git g++ cmake python3 valgrind

git clone https://github.com/MaksymT17/aquamarine.git

CentOS:

yum -y install apt-get -y install git g++ cmake python3 valgrind

git clone https://github.com/MaksymT17/aquamarine.git

When packages are installed just run one of scripts ./build.sh or ./rebuild_and_run.sh, to build binary or to build binary and run test with comparison.
[Docker containers](https://github.com/MaksymT17/aquamarine_docker) could be used to simplify development process.

Caution:

I complied and tested with g++ 11.2.0 and several other compilers including Microsoft Visual C++ (MSVC). Valgrind checks show that at least basic scenarion doesn't have mem leaks or other issues. Sometimes I find some compilation warnings, in case of something more specific please report me, or feel free to contribute.

## Components, sub-folders description:

### [extraction](https://github.com/MaksymT17/aquamarine/tree/master/extraction):
Extraction data of pixels(RGB channels, currently Alpha channel not required), as a result of reading - will be fulfilled Matrix with RGB values. Currently used bmp_reader as an open library to reading BMP images. For simple representation and highlighting objects on the image - added specified class which can draw object bounds as rectangle area over an input BMP image.

### [common](https://github.com/MaksymT17/aquamarine/tree/master/common):
Common types and definitions, which shared between components(constants/structs/classes/templates/functions)

### [analyze](https://github.com/MaksymT17/aquamarine/tree/master/analyze):
Set of Comparison classes, which responsible to identify the difference between images, and store it in the appropriate form(currently: matrix).

Asynchronous calls are in use for reducing load available CPU cores/threads. Multithreading needed in different forms, for example when recognitions between images started, at the same time extraction of data for next image - also can be performed, in the case for example "Detection of movement" where pattern requires to check set of images, more than 2.

### [thirdparty](https://github.com/MaksymT17/aquamarine/tree/master/thirdparty):
External open libraries that can help in implementation of all desired features.

### [inputs](https://github.com/MaksymT17/aquamarine/tree/master/inputs):
Minimal collection of files needed for starting comparison with images: a set of image samples in BMP format, a configuration file. Images provided with different resolutions from 10x10 pixels up to QHD samples, to make possible to debug and check how algorithms are working. The configuration has to be changed depending on test purposes or real recognition. 

Warning: in case of inappropriate configuration parameters, calculations/recognition can be incomplete or too long. The user has to specify parameters in the range that are applicable to his expectations(especially time-related constants). Planned integration of AI, for runtime parameter adjustments, to improve accuracy with applying timing requirements provided by the user.


More details can be found at [wiki page](https://github.com/MaksymT17/aquamarine/wiki)


### How does it works, what is result of the comparison
The result of the comparison is a rectangle area(s) that shows a possible object, an area where the color change is being detected. Aquamarine is able to detect person movement and skip such a small movement of grass leaves. Even behind branches of a tree visible that some object - person, in that case, is moved in some direction.

#### Restrictions:
_Commercial usage of sources added in this repo is forbidden without agreement with the author of the idea._

_Activity with the usage of current SW, which can be recognized as violations of privacy laws also not allowed._

_For quesstions or suggestions: benzin.pump@gmail.com_
