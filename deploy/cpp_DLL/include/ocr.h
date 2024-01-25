#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include "utility.h"

#ifdef  OCR_DLL_TEST2_EXPORTS
#define OCR_DLL_TEST2_API __declspec(dllexport)
#else   
#define OCR_DLL_TEST2_API __declspec(dllimport)
#endif

void check_params();
std::string get_extension(std::string );
std::string extractImagesFromOfficeFile(std::string );
std::string get_time();

void structure(std::vector<cv::String>& );
void write_results(std::vector<PaddleOCR::OCRPredictResult>& );
void main_ocr(int, char**);
void ocr(std::vector<cv::String>&);
extern "C" OCR_DLL_TEST2_API const char* extract_text(const char* input_file, const char* output_path);
extern "C" OCR_DLL_TEST2_API void free_string(const char* cstr);
