#include <iostream>
#include "floating_point.h"

int main()
{
    std::cout << "\t   Single precision floating point binary converter\n" << std::endl;
    std::cout << "Type 0 to exit converter." << std::endl;

    floatingPoint floatingNumber;
    float decimal;

    std::cout << "Enter decimal floating point number: ";
    std::cin >> decimal;

    while(decimal != 0.0)
    {
        std::cout << std::endl;
        floatingNumber.setDecimalRepresentation(decimal);
        floatingNumber.computeBinaryRepresentation();
        floatingNumber.showFloatingPoint();

        std::cout << std::endl << std::endl;

        std::cout << "Enter decimal floating point number: ";
        std::cin >> decimal;
    }
}
