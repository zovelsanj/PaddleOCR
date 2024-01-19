// ConsoleApplicationCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

extern "C" __declspec(dllexport) void call_ocr(const char* input_file, const char* output_path);

int main() {
    HMODULE dllHandle = LoadLibrary(L"Ocr_dll_test2.dll"); //Replace with your .dll path

    if (dllHandle != nullptr) {
        typedef void(*call_ocr)(const char* input_file, const char* output_path);
        call_ocr eu_norm = (call_ocr)GetProcAddress(dllHandle, "call_ocr");
        std::cout << "DLL Loaded!!" << std::endl;

        if (eu_norm != nullptr) {
            eu_norm("C:\\Users\\ekser\\OneDrive\\Documents\\OCR\\data\\3.png", "C:\\Users\\ekser\\OneDrive\\Documents\\OCR\\data"); //Replace with your input and output path
        }
        else {
            std::cout << "eu_norm is Null!!" << std::endl;
        }
        FreeLibrary(dllHandle); // Unload the DLL
    }
    else {
        DWORD errorcode = GetLastError(); // Handle error (DLL not loaded)
        std::cout << "dll not loaded!! with error "<< errorcode << std::endl;
    }

    return 0;
}
