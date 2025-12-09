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
	header.read(file);
	file.seekg(header.getDataOffset(), std::ios::beg); //going to proper position

	uint32_t w = getWidth();
	uint32_t h = getHeight();
	uint32_t rowSize = calculateRowSize();
	pixelData.resize(rowSize * h); //calculating size with paddings

	for (int y = 0; y < h; y++) {
		int rowIndex = (h - 1 - y) * rowSize;
		file.read((char*)&pixelData[rowIndex], rowSize); //read row to row in memory

		int padding = calculatePadding();
		file.seekg(padding, std::ios::cur); //replace cursor
	}

	return true;
}

bool BmpImage::save(const std::string& filename) const {
	std::ofstream file(filename, std::ios::binary);
	header.write(file);

	uint32_t w = getWidth();
	uint32_t h = getHeight();
	uint32_t rowSize = calculateRowSize();
	int padding = calculatePadding();

	for (int y = 0; y < h; y++) {
		int rowIndex = (h - 1 - y) * rowSize;
		file.write((char*)&pixelData[rowIndex], rowSize);
		for (int p = 0; p < padding; p++) {
			file.put(0); //adding zeros in the end
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

