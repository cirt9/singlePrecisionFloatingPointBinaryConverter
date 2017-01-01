#include "floating_point.h"

floatingPoint::floatingPoint()
{
    decimalRepresentation = 0;
    for(size_t i=0; i<sizeOfBinaryRepresentation; i++)
        binaryRepresentation[i] = 0;
}

void floatingPoint::computeBinaryRepresentation()
{
    specifyTheSign();
    std::vector<int> mantissa = getMantissa();
    std::vector<int> binaryExponent = getBinaryExponent();

    for(size_t i=1; i<9; i++)
        binaryRepresentation[i] = binaryExponent[i-1];

    for(size_t i=9, j=0; i<sizeOfBinaryRepresentation; i++, j++)
        binaryRepresentation[i] = mantissa[j];
}

void floatingPoint::specifyTheSign()
{
    if(decimalRepresentation > 0)
        binaryRepresentation[0] = 0;
    else
        binaryRepresentation[0] = 1;
}

std::vector<int> floatingPoint::getBinaryExponent()
{
    int exponent;
    if(decimalRepresentation == 0)
        exponent = 0;
    else
        exponent = getExponent(decimalRepresentation);

    std::vector<int>binaryExponent;
    convertIntegerToBinary(binaryExponent,exponent);

    if(exponent == 127)
    {
        binaryExponent[0] = 0;
        binaryExponent.push_back(1);
    }
    return binaryExponent;
}

int floatingPoint::getExponent(float decimal)
{
    if(decimal < 0)
        decimal *= -1;

    int exponent = 127;
    while(decimal < 1.0 || decimal >= 2.0)
    {
        decimal /= 2;
        exponent++;
    }
    return exponent;
}

std::vector<int> floatingPoint::getMantissa()
{
    std::vector<int> integerPart = convertIntegerPartToBinary();
    std::vector<int> fractionalPart = convertFractionalPartToBinary();
    std::vector<int> mantissa;

    if(decimalRepresentation == 0.0)
        setMantissaToZero(mantissa);

    else
    mergeMantissa(mantissa, integerPart, fractionalPart);

    return mantissa;
}

void floatingPoint::mergeMantissa(std::vector<int> & mantissa, std::vector<int> & integerPart, std::vector<int> & fractionalPart)
{
    for(size_t i=0; i<integerPart.size(); i++)
        mantissa.push_back(integerPart[i]);

    for(size_t i=0; i<fractionalPart.size() - integerPart.size(); i++)
        mantissa.push_back(fractionalPart[i]);

    mantissa = normalizeMantissa(mantissa);
    fillMantissa(mantissa, fractionalPart, mantissa.size());
}

void floatingPoint::setMantissaToZero(std::vector<int> & mantissa)
{
    for(size_t i=0; i<mantissaSize; i++)
        mantissa.push_back(0);
}

std::vector<int> floatingPoint::convertIntegerPartToBinary()
{
    std::vector<int> binaryIntegerPart;
    int integerPart = decimalRepresentation;

    if(integerPart < 0)
        integerPart *= -1;

    convertIntegerToBinary(binaryIntegerPart, integerPart);
    return binaryIntegerPart;
}

std::vector<int> floatingPoint::convertFractionalPartToBinary()
{
    std::vector<int> binaryFractionalPart;
    int fractionalPart = getFractionalPartAsInt();
    int divisor = computeDivisor(fractionalPart);
    size_t sizeOfFractionalPart = 0;

    while(fractionalPart != 0 && binaryFractionalPart.size() != mantissaSize)
    {
        fractionalPart *= 2;
        if(fractionalPart >= divisor)
            binaryFractionalPart.push_back(1);
        else
            binaryFractionalPart.push_back(0);
        fractionalPart %= divisor;
        sizeOfFractionalPart++;
    }

    if(sizeOfFractionalPart != mantissaSize)
        fillFractionalPart(binaryFractionalPart, sizeOfFractionalPart);

    return binaryFractionalPart;
}

std::vector<int> floatingPoint::normalizeMantissa(std::vector<int> mantissa)
{
    int dotPosition = 1, dot = 2;
    std::vector<int> floating;
    floating.push_back(dot);

    for(auto mantissaPart : mantissa)
        floating.push_back(mantissaPart);

    swapFieldsInVector(floating, 0, 1);

    while(floating[dotPosition-1] != 1)
        swapFieldsInVector(floating, dotPosition, dotPosition+1);

    std::vector<int> normalized;
    for(size_t i = dotPosition; i<floating.size()-1; i++)
        normalized.push_back(floating[i+1]);

    return normalized;
}

float floatingPoint::vectorToFloat(std::vector<int> & ToConvert)
{
    std::stringstream stream;
    for(auto vectorPart : ToConvert)
        stream << vectorPart;
    float converted;
    stream >> converted;
    return converted;
}

void floatingPoint::fillMantissa(std::vector<int> & mantissa, std::vector<int> & fractional, size_t startFrom)
{
    for(size_t i=startFrom; i<mantissaSize; i++)
    {
        mantissa.push_back(fractional[i]);
        if(mantissa.size() == 23)
            break;
    }
}

int floatingPoint::computeDivisor(int number)
{
    int divisor = 10;
    while(number > divisor)
        divisor *= 10;
    return divisor;
}

int floatingPoint::getFractionalPartAsInt()
{
    std::string decimalRepresentationString = floatToString(decimalRepresentation);
    decimalRepresentationString = truncateFractional(decimalRepresentationString);
    int fractional = stringToInt(decimalRepresentationString);
    return fractional;
}

void floatingPoint::fillFractionalPart(std::vector<int> & fractional, size_t start)
{
    for(size_t i=start; i<mantissaSize; i++)
        fractional.push_back(0);
}

std::string floatingPoint::floatToString(float numberToConvert)
{
    std::stringstream stream;
    stream << numberToConvert;
    std::string decimalRepresentationString;
    stream >> decimalRepresentationString;
    return decimalRepresentationString;
}

std::string floatingPoint::truncateFractional(std::string floatNumber)
{
    bool seekForDot = checkDot(floatNumber);
    if(seekForDot == false)
        return "0";

    size_t startTruncating = 0;
    while(floatNumber[startTruncating] != '.')
        startTruncating++;
    startTruncating++;

    size_t stopTruncating = floatNumber.size();
    while(floatNumber[stopTruncating] == 0)
        stopTruncating--;
    stopTruncating++;

    std::string fractional = truncateString(floatNumber, startTruncating, stopTruncating);
    return fractional;
}

bool floatingPoint::checkDot(std::string & str)
{
    for(size_t i=0; i<str.size(); i++)
    {
        if(str[i] == '.')
            return true;
    }
    return false;
}

std::string floatingPoint::truncateString(std::string toTruncate, size_t start, size_t stop)
{
    std::string truncated;
    truncated.resize(stop - start +1);
    for(size_t i = start, j = 0; i<stop; i++, j++)
        truncated[j] = toTruncate[i];
    return truncated;
}

int floatingPoint::stringToInt(std::string & stringToConvert)
{
    std::stringstream stream;
    stream << stringToConvert;
    int convertedString;
    stream >> convertedString;
    return convertedString;
}

void floatingPoint::convertIntegerToBinary(std::vector<int> & binaryInteger, int decimal)
{
    if(decimal < 0)
        decimal *= -1;

    while(decimal != 0)
    {
        binaryInteger.push_back(decimal % 2);
        decimal /= 2;
    }
    reverseVector(binaryInteger);
}

void floatingPoint::setDecimalRepresentation(float decimal)
{
    decimalRepresentation = decimal;
}

void floatingPoint::showFloatingPoint() const
{
    std::cout << "Decimal: " << decimalRepresentation << std::endl
              << "Binary: ";
    for(size_t i=0; i<sizeOfBinaryRepresentation; i++)
        std::cout << binaryRepresentation[i];
    std::cout << std::endl;
}
