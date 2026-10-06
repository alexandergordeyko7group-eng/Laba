 #include <windows.h>
 #include <cstdio>
 #include <iostream>
 #include <vector>
 #include "array_helper.h"

struct ThreadData
{
    std::vector<int> numbers;
    int minIndex = 0;
    int maxIndex = 0;
    double average = 0;
};

DWORD WINAPI min_max(LPVOID parameter)
{
    ThreadData* data = static_cast<ThreadData*>(parameter);

    for (int i = 1; i < static_cast<int>(data->numbers.size()); ++i)
    {
        if (data->numbers[i] < data->numbers[data->minIndex])
            data->minIndex = i;
        Sleep(7);

        if (data->numbers[i] > data->numbers[data->maxIndex])
            data->maxIndex = i;
        Sleep(7);
    }

    std::printf("Minimum: %d, maximum: %d\n",
                data->numbers[data->minIndex], data->numbers[data->maxIndex]);
    return 0;
}

DWORD WINAPI average(LPVOID parameter)
{
    ThreadData* data = static_cast<ThreadData*>(parameter);
    double sum = 0;

    for (int number : data->numbers)
    {
        sum += number;
        Sleep(12);
    }

    data->average = sum / data->numbers.size();
    std::printf("Average: %.6f\n", data->average);
    return 0;
}

int main()
{
    int size;
    std::cout << "Array size: ";
    if (!(std::cin >> size) || size <= 0)
    {
        std::cerr << "Invalid array size.\n";
        return 1;
    }

    ThreadData data;
    data.numbers.resize(size);
    std::cout << "Enter " << size << " integers: ";
    for (int& number : data.numbers)
    {
        if (!(std::cin >> number))
        {
            std::cerr << "Invalid integer.\n";
            return 1;
        }
    }

    HANDLE hMinMax = CreateThread(nullptr, 0, min_max, &data, 0, nullptr);
    if (!hMinMax)
    {
        std::cerr << "Cannot create min_max thread.\n";
        return 1;
    }

    HANDLE hAverage = CreateThread(nullptr, 0, average, &data, 0, nullptr);
    if (!hAverage)
    {
        WaitForSingleObject(hMinMax, INFINITE);
        CloseHandle(hMinMax);
        std::cerr << "Cannot create average thread.\n";
        return 1;
    }

    DWORD minResult = WaitForSingleObject(hMinMax, INFINITE);
    DWORD averageResult = WaitForSingleObject(hAverage, INFINITE);
    if (minResult != WAIT_OBJECT_0 || averageResult != WAIT_OBJECT_0)
    {
        std::cerr << "Cannot wait for threads.\n";
        ExitProcess(1);
    }
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    HMODULE library = LoadLibraryA("array_helper.dll");
    if (!library)
    {
        std::cerr << "Cannot load array_helper.dll. Error: " << GetLastError() << '\n';
        return 1;
    }

    ReplaceFunction replace = reinterpret_cast<ReplaceFunction>(
        GetProcAddress(library, "replaceElements"));
    if (!replace)
    {
        std::cerr << "Cannot find replaceElements.\n";
        FreeLibrary(library);
        return 1;
    }

    replace(data.numbers.data(), data.minIndex, data.maxIndex, data.average);
    FreeLibrary(library);

    std::cout << "Result: ";
    for (int number : data.numbers)
        std::cout << number << ' ';
    std::cout << '\n';
    return 0;
}