// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "pch.h"
#include <cstring>
#include "ocr.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <zip.h>
#include "args.h"
#include "paddleocr.h"
#include "paddlestructure.h"

using namespace PaddleOCR;

std::string extractImagesFromOfficeFile(std::string filePath) {
    std::string image_dir = FLAGS_output + "/images";

    if (!PaddleOCR::Utility::PathExists(FLAGS_output)) {
        PaddleOCR::Utility::CreateDir(FLAGS_output);
        PaddleOCR::Utility::CreateDir(image_dir);
    }

    zip_t* file = zip_open(filePath.c_str(), ZIP_RDONLY, nullptr);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return "Error Reading File";
    }

    zip_int64_t numEntries = zip_get_num_entries(file, 0);
    for (zip_int64_t i = 0; i < numEntries; ++i) {
        struct zip_stat stat;
        zip_stat_init(&stat);
        zip_stat_index(file, i, 0, &stat);

        if (std::string(stat.name).find("media/") != std::string::npos) {
            zip_file_t* imageFile = zip_fopen_index(file, i, 0);

            if (imageFile) {
                zip_int64_t size = stat.size;
                std::vector<char> buffer(size);
                zip_fread(imageFile, buffer.data(), size);
                zip_fclose(imageFile);

                std::string out_file = image_dir + "/" + std::to_string(i) + ".png";
                FILE* outputFile;
                errno_t error = fopen_s(&outputFile, out_file.c_str(), "wb");
                if (error != 0)
                {
                    std::cerr << "Error creating output file: " << out_file << " error code: " << errno << std::endl;
                }
                else {
                    std::cout << "file path: " << out_file << std::endl;
                    fwrite(buffer.data(), 1, buffer.size(), outputFile);
                    fclose(outputFile);
                    std::cout << "Image extracted: " << out_file << std::endl;
                }
            }
            else {
                std::cerr << "Error opening image file" << std::endl;
            }
        }
    }

    zip_close(file);
    return image_dir;
}

void check_params() {
  if (FLAGS_det) {
    if (FLAGS_det_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[det]: ./ppocr "
                   "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_rec) {
    std::cout
        << "In PP-OCRv3, rec_image_shape parameter defaults to '3, 48, 320',"
           "if you are using recognition model with PP-OCRv2 or an older "
           "version, "
           "please set --rec_image_shape='3,32,320"
        << std::endl;
    if (FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[rec]: ./ppocr "
                   "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_cls && FLAGS_use_angle_cls) {
    if (FLAGS_cls_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[cls]: ./ppocr "
                << "--cls_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_table) {
    if (FLAGS_table_model_dir.empty() || FLAGS_det_model_dir.empty() ||
        FLAGS_rec_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[table]: ./ppocr "
                << "--det_model_dir=/PATH/TO/DET_INFERENCE_MODEL/ "
                << "--rec_model_dir=/PATH/TO/REC_INFERENCE_MODEL/ "
                << "--table_model_dir=/PATH/TO/TABLE_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_layout) {
    if (FLAGS_layout_model_dir.empty() || FLAGS_image_dir.empty()) {
      std::cout << "Usage[layout]: ./ppocr "
                << "--layout_model_dir=/PATH/TO/LAYOUT_INFERENCE_MODEL/ "
                << "--image_dir=/PATH/TO/INPUT/IMAGE/" << std::endl;
      exit(1);
    }
  }
  if (FLAGS_precision != "fp32" && FLAGS_precision != "fp16" &&
      FLAGS_precision != "int8") {
    std::cout << "precison should be 'fp32'(default), 'fp16' or 'int8'. "
              << std::endl;
    exit(1);
  }
    std::string basename = Utility::basename(FLAGS_image_dir);
    std::size_t dotPos = basename.rfind('.');
    std::string extension;
    if (dotPos != std::string::npos) 
    {
        extension = basename.substr(dotPos);
    }
    std::cout << "extension " << extension << std::endl;
  if (extension==".docx" || extension==".xlsx" || extension==".pptx")
  {
      std::string image_dir;
      image_dir = extractImagesFromOfficeFile(FLAGS_image_dir);
      std::cout << "image: " << image_dir << std::endl;
      FLAGS_image_dir = image_dir;
  }
}

void write_results(std::vector<OCRPredictResult>& ocr_results, std::string inputfile) {
    if (!Utility::PathExists(FLAGS_output)) {
        Utility::CreateDir(FLAGS_output);
    }

    std::string basename = Utility::basename(inputfile);
    std::string out_path = FLAGS_output + "/" + basename + ".txt";

    std::fstream file(out_path, std::ios::app);

    if (!file.is_open()) {
        std::ofstream createFile(out_path);
        createFile.close();

        file.open(out_path, std::ios::app);
    }
    for (int i = 0; i < ocr_results.size(); i++) {
        file << ocr_results[i].text << std::ends;
    }
    file << std::endl;
    file.close();
    std::cout << "Data has been written to the file in append mode." << std::endl;
}

void ocr(std::vector<cv::String> &cv_all_img_names, std::string inputfile) {
  PPOCR ocr;

  if (FLAGS_benchmark_) {
    ocr.reset_timer();
  }

  std::vector<cv::Mat> img_list;
  std::vector<cv::String> img_names;
  for (int i = 0; i < cv_all_img_names.size(); ++i) {
    cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
    if (!img.data) {
      std::cerr << "[ERROR] image read failed! image path: "
                << cv_all_img_names[i] << std::endl;
      continue;
    }
    img_list.push_back(img);
    img_names.push_back(cv_all_img_names[i]);
  }

  std::vector<std::vector<OCRPredictResult>> ocr_results =
      ocr.ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);

  for (int i = 0; i < img_names.size(); ++i) {
    std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
    Utility::print_result(ocr_results[i]);
    write_results(ocr_results[i], inputfile);

    if (FLAGS_visualize && FLAGS_det) {
      std::string file_name = Utility::basename(img_names[i]);
      cv::Mat srcimg = img_list[i];
      Utility::VisualizeBboxes(srcimg, ocr_results[i],
                               FLAGS_output + "/" + file_name);
    }
  }
  if (FLAGS_benchmark_) {
    ocr.benchmark_log(cv_all_img_names.size());
  }
}

void structure(std::vector<cv::String> &cv_all_img_names) {
  PaddleOCR::PaddleStructure engine;

  if (FLAGS_benchmark_) {
    engine.reset_timer();
  }

  for (int i = 0; i < cv_all_img_names.size(); i++) {
    std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
    cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
    if (!img.data) {
      std::cerr << "[ERROR] image read failed! image path: "
                << cv_all_img_names[i] << std::endl;
      continue;
    }

    std::vector<StructurePredictResult> structure_results = engine.structure(
        img, FLAGS_layout, FLAGS_table, FLAGS_det && FLAGS_rec);

    for (int j = 0; j < structure_results.size(); j++) {
      std::cout << j << "\ttype: " << structure_results[j].type
                << ", region: [";
      std::cout << structure_results[j].box[0] << ","
                << structure_results[j].box[1] << ","
                << structure_results[j].box[2] << ","
                << structure_results[j].box[3] << "], score: ";
      std::cout << structure_results[j].confidence << ", res: ";

      if (structure_results[j].type == "table") {
        std::cout << structure_results[j].html << std::endl;
        if (structure_results[j].cell_box.size() > 0 && FLAGS_visualize) {
          std::string file_name = Utility::basename(cv_all_img_names[i]);

          Utility::VisualizeBboxes(img, structure_results[j],
                                   FLAGS_output + "/" + std::to_string(j) +
                                       "_" + file_name);
        }
      } else {
        std::cout << "count of ocr result is : "
                  << structure_results[j].text_res.size() << std::endl;
        if (structure_results[j].text_res.size() > 0) {
          std::cout << "********** print ocr result "
                    << "**********" << std::endl;
          Utility::print_result(structure_results[j].text_res);
          std::cout << "********** end print ocr result "
                    << "**********" << std::endl;
        }
      }
    }
  }
  if (FLAGS_benchmark_) {
    engine.benchmark_log(cv_all_img_names.size());
  }
}

int main_ocr(int argc, char **argv) {
  // Parsing command-line
  google::ParseCommandLineFlags(&argc, &argv, true);
  std::string inputfile = FLAGS_image_dir;
  check_params();

  if (!Utility::PathExists(FLAGS_image_dir)) {
    std::cerr << "[ERROR] image path not exist! image_dir: " << FLAGS_image_dir
              << std::endl;
    exit(1);
  }

  std::vector<cv::String> cv_all_img_names;
  cv::glob(FLAGS_image_dir, cv_all_img_names);
  std::cout << "total images num: " << cv_all_img_names.size() << std::endl;

  if (!Utility::PathExists(FLAGS_output)) {
    Utility::CreateDir(FLAGS_output);
  }
  if (FLAGS_type == "ocr") {
    ocr(cv_all_img_names, inputfile);
  } else if (FLAGS_type == "structure") {
    structure(cv_all_img_names);
  } else {
    std::cout << "only value in ['ocr','structure'] is supported" << std::endl;
  }
}

void call_ocr(const char* input_file, const char* output_path)
{
    const char* initials = "--image_dir=";
    size_t lenInitials = std::strlen(initials);
    size_t lenInput = std::strlen(input_file);
    size_t totalLen = lenInitials + lenInput;

    // Allocate memory for the concatenated string
    char* result = new char[totalLen + 1];
    // Copy the initials
    strcpy_s(result, totalLen + 1, initials);

    // Concatenate the input
    strcat_s(result, totalLen + 1, input_file);

    const char* args[] = {"system", 
                            result, 
                            "--use_angle_cls=true", 
                            "--det=true",
                            "--rec=true",
                            "--cls=true"
                            };

    int argc = sizeof(args) / sizeof(args[0]);  
    std::cout << "argc  value: " << argc << std::endl;
    main_ocr(argc, const_cast<char**>(args));
}
