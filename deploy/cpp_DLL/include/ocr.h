#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>

#ifdef  OCR_DLL_TEST2_EXPORTS
#define OCR_DLL_TEST2_API __declspec(dllexport)
#else   
#define OCR_DLL_TEST2_API __declspec(dllimport)
#endif

void check_params();
//void ocr(std::vector<cv::String>& );
void structure(std::vector<cv::String>& );
extern "C" OCR_DLL_TEST2_API int main_ocr(int, char**);
extern "C" OCR_DLL_TEST2_API void ocr(std::vector<cv::String>&);
extern "C" OCR_DLL_TEST2_API void call_ocr(const char* input_file, const char* output_path);
