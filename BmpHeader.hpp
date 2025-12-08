/*
* Arina Babinskaya
* st139880@student.spbu.ru
* LabWork1
*/

#ifndef BMPHEADER_H
#define BMPHEADER_H

#include <fstream>
#include <cstdint>
#pragma pack(push,1)

struct BITMAPFILEHEADER {
	uint16_t bfType = 0x4D42; // type of file - 'BM' (2 bytes)
	uint32_t bfSize = 0; // size of the image (4 bytes)
	uint16_t bfReserved1 = 0; // for future iteretions (2 bytes)
	uint16_t bfReserved2 = 0; // for future iteretions (2 bytes)
	uint32_t bfPixel = 54; // get towards the beggining of file (pixels) (4 bytes);
};

struct BITMAPINFOHEADER {
	uint32_t biSize = 40; //struct size
	int32_t  biWidth = 0; // struct wight
	int32_t  biHeight = 0; // struct height
	uint16_t biPlanes = 1; // plane number (always 1)
	uint16_t biBitCount = 24; // 8R + 8G + 8B
	uint32_t biCompression = 0; // without compression
	uint32_t biSizeImage = 0; // image data size
	int32_t  biXPixelsPerMeter = 0; // pixels/meter (DPI)
	int32_t  biYPixelsPerMeter = 0; // pixels/meter (DPI)
	uint32_t biClrUsed = 0; // used colors
	uint32_t biClrImportant = 0; //important colors
};

#pragma pack(pop)

class BmpHeader {
	private:
		BITMAPFILEHEADER BmpFile;
		BITMAPINFOHEADER BmpInfo;

	public:
		BmpHeader(uint32_t width, uint32_t height);

		bool read(std::ifstream& file);
		bool write(std::ofstream& file) const;

		uint32_t getWidth() const {
			return BmpInfo.biWidth;
		}
		uint32_t getHeight() const {
			return BmpInfo.biHeight;
		}
		uint32_t getImageSize() const {
			return BmpInfo.biSizeImage;
		}
		uint32_t getDataOffset() const {
			return BmpFile.bfPixel;
		}
		uint16_t getBitsPerPixel() const {
			return BmpInfo.biBitCount;
		}


		void setWidth(uint32_t width) {
			BmpInfo.biWidth = width;
		}
		void setHeight(uint32_t height) {
			BmpInfo.biHeight = height;
		}
		void setImageSize(uint32_t size) {
			BmpInfo.biSizeImage = size;
		}

};

#endif
