` aquamarine `

## Initial idea:

Tool for recognitions/detections/analyzing images, for searching differences in pictures, can help in detection of movement, appearances/disappearances of objects, change of weather/light conditions.

## Target: 
Find optimal solutions for implementation and figure out how to make recognition of image characteristics in approprite form and inside of predefined limitations in config file. Parameters in config file should cover all significant properties about calculation, should be taken in consideration target Platform and HW limitations.

In simple words, goal is: to collect most important information based on picture data and use it for further analysis, paying into attention that collecting of every particular properties of image are time/CPU/power consuming operations. Every
improvement idea are welcome!

## Components, sub-folders description:

### [extraction](https://github.com/MaksymT17/aquamarine/tree/master/extraction):
Extraction data of pixels(RGB channels, currently Alpha channel not required), as result of reading - will be fulfilled Matrix with RGB values. Currently used bmp_reader as open library to reading BMP images. For simple represntation and highlighting objects on image - added specified class which can draw object bounds as rectangled area over input BMP image.

### [common](https://github.com/MaksymT17/aquamarine/tree/master/common):
Common types and definitions, which shared between components(constants/structs/classes/templates/functions)

### [analyze](https://github.com/MaksymT17/aquamarine/tree/master/analyze):
Set of Comparison classes, which responsible to identify difference between images, and store it in appropriate form(currently: matrix).

Asynchronous calls are in use for reducing load available available CPU cores/threads. Multi threading needed in different forms, for example when recognitions between images started, at the same time extraction of data for next image - also can be started, in case for example "Detection of movement" where pattern require to check set of images more than 2.

### [thirdparty](https://github.com/MaksymT17/aquamarine/tree/master/thirdparty):
External open libraries which can help in implementation of all desired features

### [inputs](https://github.com/MaksymT17/aquamarine/tree/master/inputs):
Minimal collection of files needed for starting comparison with images: set of image samples in BMP format, configuration file. Images provided with different resolutions from 10x10 pixels up to QHD samples, to make possible to debug and check how algorithms are working. Configuration has to be changed depending on test purposes or real recognition. 

Warning: in case of inappropriate configuration parameters calculations/recognition can be incomplete or too long. User has to specify parameters in range what is applicable to his expectations(especially time related constants). Planned integration of AI, for runtime parameters adjustments to improve accuracy with apllying timing requirements provided by user.

### plans:
as predelivery version and after all testing stages based on code provided in this public repo shall be created shared library focused on performance of execution, with minimized logs prints and exceptions handling.

More details can be found at [wiki page](https://github.com/MaksymT17/aquamarine/wiki)
