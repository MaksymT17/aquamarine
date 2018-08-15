
aquamarine

Initial idea:
tool for recogninitions/detections/analyzing images. Solution for searching differences in pictures, can help in detection of movement, 
appearences/disappearences of objects, change of weather/light conditions.

Target: 
Find optimal solution to use and figure out how to make such recognitions in a best way. Sharing it between proffessional developers help us to find best practices to make recognition fast and usefull, thats why I`m here :). 
Every improvement idea are welcome!

Components:

extraction:
extraction data of pixels(RGB channels, currently Alpha channel not required), as result of reading - will be filled Matrix with RGB values.
Currently will be used bmp_reader as open library to reading BMP images.

common:
Common types and definitions, which shared between components(constants/structs/classes/templates/functions)

module analyze:
set of Comparison classes, which responcible to identify difference between images, and store it in appropriate form(currently: matrix). 
Async calls are in use to reduce load between available CPU threads.
Planned: to add classes or some external library (AI, neural nets) which will apply patters for recognitions  movement, 
appearences/disappearences of objects, change of weather/light conditions based on collected differences between images


thirdparty:
external open libraries which can help in implementation of all desired features

next planned tasks:
add proper reading from bmp files
add logger with variadic arguments
add class for showing percentage of similarity of compared images with setting affinity treshold

