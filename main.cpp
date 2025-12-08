#include "BmpHeader.hpp"
#include <iostream>

int main() {
    // Создайте простой тест
    std::ifstream file("test.bmp", std::ios::binary);
    BmpHeader header(0, 0);
    if (header.read(file)) {
        std::cout << "Width: " << header.getWidth() << std::endl;
        std::cout << "Height: " << header.getHeight() << std::endl;
    }
    return 0;
}
