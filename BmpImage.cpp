/*
* Arina Babinskaya
* st139880@student.spbu.ru
* LabWork1
*/

#include <iostream>
#include "BmpImage.hpp"
#include <fstream>
#include <algorithm>
#include <cmath>

//length of row with padding
uint32_t BmpImage::calculateRowSize() const {
	uint32_t w = getWidth();
	return ((w * 3 + 3) / 4) * 4;
}

//the amount of zeros in the end
uint32_t BmpImage::calculatePadding() const {
	uint32_t w = getWidth();
	return (4 - (w * 3) % 4) % 4;
}

bool BmpImage::load(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}
	if (!header.read(file)) {
		return false;
	}
	file.seekg(header.getDataOffset(), std::ios::beg); //going to proper position

	uint32_t h = getHeight();
	uint32_t rowSize = calculateRowSize();
	pixelData.assign(rowSize * h, 0);

	for (uint32_t y = 0; y < h; y++) {
		uint32_t rowIndex = (h - 1 - y) * rowSize;

		if (!file.read(reinterpret_cast<char*>(&pixelData[rowIndex]), rowSize)) {
			return false;
		}
	}

	return true;
}

bool BmpImage::save(const std::string& filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}
	if (!header.write(file)) {
		return false;
	}

	uint32_t h = getHeight();
	uint32_t rowSize = calculateRowSize();

	for (uint32_t y = 0; y < h; y++) {
		uint32_t rowIndex = (h - 1 - y) * rowSize;

		if (!file.write(reinterpret_cast<const char*>(&pixelData[rowIndex]), rowSize)) {
			return false;
		}
	}
	return true;
}

void BmpImage::getPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const {
	//check boaders: if they are bigger then it's padding and pixels are zeros
	if (x >= getWidth() || y >= getHeight()) {
		r = g = b = 0;
		return;
	}
	uint32_t rowSize = calculateRowSize();
	uint32_t index = y * rowSize + x * 3;
	b = pixelData[index]; //blue color
	g = pixelData[index+1]; //green color
	r = pixelData[index+2]; //red color
}

void BmpImage::setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
	//check boaders: if they are bigger we do nithing
	if (x >= getWidth() || y >= getHeight()) {
		return;
	}

	uint32_t rowSize = calculateRowSize();
	uint32_t index = y * rowSize + x * 3;
	pixelData[index] = b;     //blue color
	pixelData[index+1] = g;   //green color
	pixelData[index+2] = r;   //red color
}

//Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCW() const {
	uint32_t oldW = getWidth();
	uint32_t oldH = getHeight();

	auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
	newImg->header = header;
	newImg->header.updateForRotation(oldH, oldW); //change width and height

	uint32_t oldRowSize = calculateRowSize();
	uint32_t newRowSize = newImg->calculateRowSize();
	newImg->pixelData.assign(newRowSize * oldW, 0);

	for (uint32_t y = 0; y < oldH; y++) {
		for (uint32_t x = 0; x < oldW; x++) {
			uint32_t oldIndex = y * oldRowSize + x * 3;
			uint32_t newX = oldH - 1 - y;
			uint32_t newY = x;
			uint32_t newIndex = newY * newRowSize + newX * 3;
			newImg->pixelData[newIndex] = pixelData[oldIndex];
			newImg->pixelData[newIndex+1] = pixelData[oldIndex+1];
			newImg->pixelData[newIndex+2] = pixelData[oldIndex+2];
		}
	}
	return newImg;
}

//Counter Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCCW() const {
	uint32_t oldW = getWidth();
        uint32_t oldH = getHeight();

        auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
        newImg->header = header;
        newImg->header.updateForRotation(oldH, oldW); //change width and height

        uint32_t oldRowSize = calculateRowSize();
        uint32_t newRowSize = newImg->calculateRowSize();
        newImg->pixelData.assign(newRowSize * oldW, 0);

        for (uint32_t y = 0; y < oldH; y++) {
                for (uint32_t x = 0; x < oldW; x++) {
                        uint32_t oldIndex = y * oldRowSize + x * 3;
                        uint32_t newX = y;
                        uint32_t newY = oldW - 1 - x;
                        uint32_t newIndex = newY * newRowSize + newX * 3;
                        newImg->pixelData[newIndex] = pixelData[oldIndex];
                        newImg->pixelData[newIndex+1] = pixelData[oldIndex+1];
                        newImg->pixelData[newIndex+2] = pixelData[oldIndex+2];
                }
        }
        return newImg;
}

//Gaussian Blur
void BmpImage::gaussianBlur() {
	uint32_t w = getWidth();
	uint32_t h = getHeight();

	if (w < 3 || h < 3) {
		std::cout << "picture is too small for 3x3 kernel\n";
		return;
	}

	uint32_t rowSize = calculateRowSize();
	std::vector<uint8_t> newData = pixelData;

	for (uint32_t y = 1; y < h-1; y++) {
		for (uint32_t x = 1; x < w-1; x++) {
			float sumB = 0, sumG = 0, sumR = 0;

			for (uint32_t dy = -1; dy <= 1; dy++) {
				for (uint32_t dx = -1; dx <= 1; dx++) {
					uint32_t nx = x + dx;
					uint32_t ny = y + dy;
					uint32_t index = ny * rowSize + nx * 3;
					sumB += pixelData[index];
					sumG += pixelData[index+1];
					sumR += pixelData[index+2];
				}
			}

			uint32_t newIndex = y * rowSize + x * 3;
			newData[newIndex] = static_cast<uint8_t>(std::clamp(sumB / 9.0f, 0.0f, 255.0f));
			newData[newIndex + 1] = static_cast<uint8_t>(std::clamp(sumG / 9.0f, 0.0f,255.0f));
			newData[newIndex + 2] = static_cast<uint8_t>(std::clamp(sumR / 9.0f, 0.0f, 255.0f));

		}
	}

	pixelData = newData;
}

uint32_t BmpImage::calculateRequiredMemory(uint32_t width, uint32_t height) {
	uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + rowSize * height;
}
