/*
* Arina Babinskaya
* st139880@student.spbu.ru
* LabWork1
*/

#include "BmpHeader.hpp"
BmpHeader::BmpHeader(uint32_t width, uint32_t height) {
	BmpInfo.biWidth = width;
	BmpInfo.biHeight = height;

	uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
	BmpInfo.biSizeImage = rowSize * height;
	BmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + BmpInfo.biSizeImage;
}


bool BmpHeader::read(std::ifstream& file) {
	if (!file.read(reinterpret_cast<char*>(&BmpFile), sizeof(BmpFile))) {
		return false;
	}
	if (BmpFile.bfType != 0x4D42) {
		return false;
	}
	if (!file.read(reinterpret_cast<char*>(&BmpInfo), sizeof(BmpInfo))) {
		return false;
	}
	return true;
}

bool BmpHeader::write(std::ofstream& file) const {
	if (!file.write(reinterpret_cast<const char*>(&BmpFile), sizeof(BmpFile))) {
		return false;
	}
	if (!file.write(reinterpret_cast<const char*>(&BmpInfo), sizeof(BmpInfo))) {
		return false;
	}
	return true;
}
void BmpHeader::updateForRotation(uint32_t newW, uint32_t newH) {
	setWidth(newW);
	setHeight(newH);
	uint32_t rowSize = ((newW * 3 + 3) / 4) * 4;
	setImageSize(rowSize * newH);
	BmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + getImageSize();
}
