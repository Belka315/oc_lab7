
#include <string>
#include <iostream>

#include "lib77.h" 

// ������� ��� ������ ������� � ������
bool StrScan(const char* str, char ch) {
    return strchr(str, ch) != nullptr;
}

// ������� procd3
extern "C" __declspec(dllexport) bool procd3(const char* a, char b) {
    std::string s = "������ \"";
    s += b;
    s += "\" � ������ \"";
    s += a;
    s += "\" ";

    bool result = StrScan(a, b);
    if (!result) {
        s += "�� ";
    }
    s += "�����������";

    MessageBoxA(NULL, s.c_str(), "Procedure 3", MB_OK | MB_ICONINFORMATION);
    return result;
}