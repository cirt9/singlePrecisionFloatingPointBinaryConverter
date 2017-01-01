#ifndef FLOATING_POINT_H_INCLUDED
#define FLOATING_POINT_H_INCLUDED

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

class floatingPoint
{
private:
    float decimalRepresentation;

    static const int sizeOfBinaryRepresentation = 32;
    static const int mantissaSize = 23;
    int binaryRepresentation[sizeOfBinaryRepresentation];

    void specifyTheSign();
    std::vector<int> getBinaryExponent();
    int getExponent(float decimal);

    std::vector<int> getMantissa();
    void mergeMantissa(std::vector<int> & mantissa, std::vector<int> & integerPart, std::vector<int> & fractionalPart);
    void setMantissaToZero(std::vector<int> & mantissa);

    std::vector<int> convertIntegerPartToBinary();
    std::vector<int> convertFractionalPartToBinary();
    std::vector<int> normalizeMantissa(std::vector<int> mantissa);

    float vectorToFloat(std::vector<int> & ToConvert);
    void fillMantissa(std::vector<int> & mantissa, std::vector<int> & fractional, size_t startFrom);

    int computeDivisor(int number);
    int getFractionalPartAsInt();
    void fillFractionalPart(std::vector<int> & fractional, size_t start);

    std::string floatToString(float numberToConvert);
    std::string truncateFractional(std::string floatNumber);
    std::string truncateString(std::string toTruncate, size_t start, size_t stop);
    bool checkDot(std::string & str);
    int stringToInt(std::string & stringToConvert);

    void convertIntegerToBinary(std::vector<int> & binaryInteger, int decimal);

    template<typename T>
    void swapFieldsInVector(std::vector<T> & vect, T firstPlace, T secondPlace)
    {
        T temp = vect[firstPlace];
        vect[firstPlace] = vect[secondPlace];
        vect[secondPlace] = temp;
    }

    template<typename T>
    void reverseVector(std::vector<T> & toReverse)
    {
        for(size_t i=0; i<toReverse.size() / 2; i++)
        {
            T field = toReverse[i];
            toReverse[i] = toReverse[toReverse.size() - 1 - i];
            toReverse[toReverse.size() - 1 - i] = field;
        }
    }

public:
    floatingPoint();
    ~floatingPoint() {};

    void computeBinaryRepresentation();

    void setDecimalRepresentation(float decimal);
    void showFloatingPoint() const;
};


#endif // FLOATING_POINT_H_INCLUDED
