/*
* Arina Babinskaya
* st139880@student.spbu.ru
* LabWork1
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory> // use it to creat smart pointer

class Image {
	public:
		virtual ~Image() = default;

		//pure virtual methods
		virtual bool load(const std::string& filename) = 0;
		virtual bool save(const std::string& filename) = 0;

		virtual std::unique_ptr<Image> rotateCW() const = 0; // Clock Wise rotation
		virtual std::unique_ptr<Image> rotateCCW() const = 0; // Counter Clock Wise rotation
		virtual void gaussianBlur() = 0;

		//virtual getters
		virtual uint32_t width() const = 0;
		virtual uint32_t height() const = 0;
		virtual uint32_t size() const = 0;
};

#endif
