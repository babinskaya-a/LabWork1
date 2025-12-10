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

//Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCW() const {
	int oldW = getWidth();
	int oldH = getHeight();

	auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
	newImg->header = header;
	newImg->header.updateForRotation(oldH, oldW); //change width and height

	int oldRowSize = calculateRowSize();
	int newRowSize = newImg->calculateRowSize();
	newImg->pixelData.resize(newRowSize * oldW);

	for (int y = 0; y < oldH; y++) {
		for (int x = 0; x < oldW; x++) {
			int oldIndex = y * oldRowSize + x * 3;
			int newX = oldH - 1 - y;
			int newY = x;
			int newIndex = newY * newRowSize + newX * 3;
			newImg->pixelData[newIndex] = pixelData[oldIndex];
			newImg->pixelData[newIndex+1] = pixelData[oldIndex+1];
			newImg->pixelData[newIndex+2] = pixelData[oldIndex+2];
		}
	}
	return newImg;
}

//Counter Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCCW() const {
	int oldW = getWidth();
        int oldH = getHeight();

        auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
        newImg->header = header;
        newImg->header.updateForRotation(oldH, oldW); //change width and height

        int oldRowSize = calculateRowSize();
        int newRowSize = newImg->calculateRowSize();
        newImg->pixelData.resize(newRowSize * oldW);

        for (int y = 0; y < oldH; y++) {
                for (int x = 0; x < oldW; x++) {
                        int oldIndex = y * oldRowSize + x * 3;
                        int newX = y;
                        int newY = oldW - 1 - x;
                        int newIndex = newY * newRowSize + newX * 3;
                        newImg->pixelData[newIndex] = pixelData[oldIndex];
                        newImg->pixelData[newIndex+1] = pixelData[oldIndex+1];
                        newImg->pixelData[newIndex+2] = pixelData[oldIndex+2];
                }
        }
        return newImg;
}

//Gaussian Blur
void BmpImage::gaussianBlur() {
	int w = getWidth();
	int h = getHeight();

	if (w < 3 || h < 3) {
		std::cout << "picture is too small for 3x3 kernel\n";
		return;
	}

	int rowSize = calculateRowSize();
	std::vector<uint8_t> newData = pixelData;

	for (int y = 1; y < h-1; y++) {
		for (int x = 1; x < w-1; x++) {
			float sumB = 0, sumG = 0, sumR = 0;

			for (int dy = -1; dy <= 1; dy++) {
				for (int dx = -1; dx <= 1; dx++) {
					int nx = x + dx;
					int ny = y + dy;
					int index = ny * rowSize + nx * 3;
					sumB += pixelData[index];
					sumG += pixelData[index+1];
					sumR += pixelData[index+2];
				}
			}

			int newIndex = y * rowSize + x * 3;
			newData[newIndex] = static_cast<uint8_t>(std::max(0.0f, std::min(255.0f, sumB)));
			newData[newIndex + 1] = static_cast<uint8_t>(std::max(0.0f, std::min(255.0f, sumG)));
			newData[newIndex + 2] = static_cast<uint8_t>(std::max(0.0f, std::min(255.0f, sumR)));

		}
	}

	pixelData = newData;
}

