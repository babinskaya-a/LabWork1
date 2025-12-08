/*
* Arina Babinskaya
* st139880@student.spbu.ru
* LabWork1
*/

#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "Image.hpp"
#include "BmpHeader.hpp"
#include <vector>

class BmpImage : public Image {
	private:
		BmpHeader header(0, 0);
		std::vector<uint8_t> pixelData;

		uint32_t calculateRowSize() const;
		uint32_t calculatePadding() const;
		void readPixelData(std::ifstream& file);
		void writePixelData(std::ofstream& file) const;

	public:
		BmpImage() = default;

		bool load(const std::string& filename) override;
		bool save(const std::string& filename) override;
		std::unique_ptr<Image> rotateCW() const override;
		std::unique_ptr<Image> rotateCCW() const override;
		void gaussianBlur() override;


		uint32_t getWidth() const override {
			return header.getWidth();
		}
		uint32_t getHeight() const override {
			return header.getHeight();
		}
		uint32_t getSize() const override {
			return header.getSize();
		}

		void setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
		void getPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const;
		static uint32_t calculateRequiredMemory(uint32_t width, uint32_t height);
};

#endif
