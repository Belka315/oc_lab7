
#include <string>
#include <iostream>

#include "lib77.h" 

// Функция для поиска символа в строке
bool StrScan(const char* str, char ch) {
    return strchr(str, ch) != nullptr;
}

// Функция procd3
extern "C" __declspec(dllexport) bool procd3(const char* a, char b) {
    std::string s = "Символ \"";
    s += b;
    s += "\" в строке \"";
    s += a;
    s += "\" ";

    bool result = StrScan(a, b);
    if (!result) {
        s += "не ";
    }
    s += "встречается";

    MessageBoxA(NULL, s.c_str(), "Procedure 3", MB_OK | MB_ICONINFORMATION);
    return result;
}