#include "array_helper.h"

extern "C" __declspec(dllexport) void replaceElements(
    int* numbers, int minIndex, int maxIndex, double average)
{
    int value = static_cast<int>(average);
    numbers[minIndex] = value;
    numbers[maxIndex] = value;
}