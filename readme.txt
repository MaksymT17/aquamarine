"aquamarine" 

Initial idea:
tool for recognitions/detections/analyzing images. Solution for searching differences in pictures, 
can help in detection of movement, appearances/disappearances of objects, change of weather/light 
conditions.

Target: 
Find optimal solution to implementation and figure out how to make such recognitions in a best way. 
Sharing it between professional developers help us to find best practices to make recognition fast 
and useful, thats why I`m here. Every improvement idea are welcome!

Components, sub-folders description:

extraction:
Extraction data of pixels(RGB channels, currently Alpha channel not required), as result of reading 
- will be filled Matrix with RGB values. Currently will be used bmp_reader as open library to reading 
BMP images.

common:
Common types and definitions, which shared between components(constants/structs/classes/templates/functions)

analyze:
set of Comparison classes, which responsible to identify difference between images, and store it in 
appropriate form(currently: matrix). 
Async calls are in use to reduce load between available CPU threads. Multi threading needed in different 
forms, for example when recognitions between images started, at the same time extraction of data for next 
image - also can be started, in case for example "Detection of movement" where pattern require to check 
set of images more than 2.

thirdparty:
external open libraries which can help in implementation of all desired features
