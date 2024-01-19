# Configure PADDLEOCR C++ in Visual Studio for DLL
This documentation is a guide for all the necessary setups required to generate a DLL from C++ deployment code of PaddleOCR. The paths here are kept absolute, so please consider adding your corresponding paths where necessary. 

To create a DLL, you will first need to create this `cpp_DLL` source code as `Dynamic-Link Library (DLL) C++` project from Visual Studio.
## Include Paths: C/C++>>General>>AdditionalIncludeDirectories
```
<AdditionalIncludeDirectories>
C:\Users\ekser\OneDrive\Documents\OCR\test\Ocr_dll_test2;
C:\Users\ekser\OneDrive\Documents\OCR\dependencies\opencv\build\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\paddle\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\gflags\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\glog\include;
C:\Users\ekser\OneDrive\Documents\OCR\test\Ocr_dll_test2\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\xxhash\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\utf8proc\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\protobuf\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\paddle2onnx\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\onnxruntime\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\mklml\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\mkldnn\include;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\cryptopp\include;
%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
```
      
## Library Paths: Linker>>General>>AdditionalLibraryDirectories
```
<AdditionalLibraryDirectories>
C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64;
C:\Users\ekser\OneDrive\Documents\OCR\dependencies\opencv\build\x64\vc15\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\paddle\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\glog\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\gflags\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\utf8proc\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\paddle2onnx\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\protobuf\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\xxhash\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\cryptopp\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\mkldnn\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\mklml\lib;
C:\Users\ekser\OneDrive\Documents\OCR\paddleOCR\paddle_inference\third_party\install\onnxruntime\lib;
%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
```

## Libraries: Linker>>Input>>AdditionalDependencies
```
<AdditionalDependencies>
gflags_static.lib;
glog.lib;
opencv_world455.lib;
opencv_world455d.lib;
libpaddle_inference.lib;
paddle_inference.lib;
mkldnn.lib;
libiomp5md.lib;
mklml.lib;
onnxruntime.lib;
paddle2onnx.lib;
libprotobuf.lib;
cryptopp-static.lib;
utf8proc_static.lib;
xxhash.lib;
ShLwApi.Lib;
%(AdditionalDependencies)</AdditionalDependencies>
```
For `ShLwApi.Lib`, you may need to include it from `C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64`.

      
## Runtime Library: C/C++>>Code Generation
To solve [LNK2005](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2005?view=msvc-170&f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(LNK2005)%26rd%3Dtrue) error due to mismatched Runtime Library. ([StackOverflow](https://stackoverflow.com/questions/3007312/resolving-lnk4098-defaultlib-msvcrt-conflicts-with))
```
Runtime Library: Multi-threaded (/MT)
```

## Force File Output: Linker>>General
To solve multiple definitions of a symbol. ([LNK1169](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk1169?view=msvc-170&f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(LNK1169)%26rd%3Dtrue))
```
Force File Output: Enabled (/FORCE)
```

## Changes in `cpp_infer` Source Code
Due to multiple definitions error with identifier `benchmark` in `args.cpp` and `paddle/fluid/place.cc`, we renamed the `benchmark` flag in all the `cpp_infer` files to `benchmark_`.

## Console Application
To test the DLL, you will need to create a Console Application project as `Console App C++`. You **may** need to copy all the OCR dependencies in the ConsoleApplication's `x64>>Debug` (depending on the compiler and compile mode) path. The required dependencies are as follows:
```
paddle2onnx.dll 
libiomp5md.dll 
mkldnn.dll 
mklml.dll 
onnxruntime.dll 
opencv_world455.dll 
paddle_inference.dll
```