#pragma once

using ReplaceFunction = void (*)(int*, int, int, double);

#ifdef ARRAY_HELPER_EXPORTS
extern "C" __declspec(dllexport) void replaceElements(
    int* numbers, int minIndex, int maxIndex, double average);
#endif