# LabWork 1
## Author
Арина Бабинская 25.Б81-мм
## Contacts st139880@stdudent.spbu.ru
## Description
This lab focuses on loading and processing uncompressed 24-bit BMP raster images using only the C++ Standard Library. 

The structure is:
1. Image.hpp - pure abstract class Image (general interface for images)
2. BmpHeader.hpp and BmpHeader.cpp - headers' structure and class BmpHeader for reading and writing headers
3. BmpImge.hpp and BmpImage.cpp - realization of load, save, rotation clockwise, rotation counter clockwise, gausian blur, ect.
4. test1.bmp and test2.bmp - test images
5. test.cpp - google tests for methods in the project

## Build
make 
## Run
make run
## Test
make test
## Clean
make clean
