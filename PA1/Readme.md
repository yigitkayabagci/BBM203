# Image Processing for Defense Applications (C++)

This C++ project enhances images and secures hidden data for defense applications. It includes noise reduction, sharpening, and steganography techniques.

## Features

### Noise Reduction
Mean & Gaussian Filters remove unwanted noise while preserving image details.

![Noise Reduction Example](noise_reduction_example.jpg)

### Image Sharpening
Unsharp Masking enhances clarity and highlights critical features.

![Sharpening Example](sharpening_example.jpg)

### Hidden Message Embedding (LSB Steganography)
Securely embed and extract messages within images.

![Steganography Demo](steganography_demo.jpg)

### Performance Optimizations
- Efficient Memory Management for handling large images
- Operator Overloading for simplified calculations
- Triangular Matrix Storage to reduce memory usage

![Memory Optimization Example](memory_optimization_example.jpg)

## Installation

```bash
git clone https://github.com/username/defense-image-processing.git
cd defense-image-processing
mkdir build && cd build
cmake ..
make
```

## Usage

### Noise Reduction Example
```cpp
#include "ImageProcessor.h"

int main() {
    ImageProcessor processor("input.jpg");
    
    // Apply Mean filter
    processor.applyMeanFilter(3); // 3x3 kernel size
    
    // Or apply Gaussian filter
    processor.applyGaussianFilter(5, 1.4); // 5x5 kernel, sigma=1.4
    
    processor.saveImage("output.jpg");
    return 0;
}
```

### Image Sharpening Example
```cpp
#include "ImageProcessor.h"

int main() {
    ImageProcessor processor("input.jpg");
    
    // Apply Unsharp masking
    processor.applyUnsharpMasking(0.6); // Sharpening factor
    
    processor.saveImage("sharpened.jpg");
    return 0;
}
```

### Steganography Example
```cpp
#include "Steganography.h"

int main() {
    Steganography steg;
    
    // Embed message
    steg.embedMessage("cover.jpg", "secret_message.txt", "output_stego.jpg");
    
    // Extract message
    steg.extractMessage("output_stego.jpg", "extracted_message.txt");
    
    return 0;
}
```

## Memory Optimization

The project uses specialized memory management techniques for efficient processing of large images:

```cpp
// Triangular matrix storage example
TriangularMatrix<float> matrix(1000); // Stores only required elements for a 1000x1000 matrix
matrix.set(i, j, value); // Set value
float value = matrix.get(i, j); // Get value
```

## Requirements

- C++17 compatible compiler
- OpenCV 4.5+
- CMake 3.10+
- Eigen 3.3+ (for matrix operations)

## License

This project is licensed under the [MIT License](LICENSE).

## Contact

For any questions or suggestions, please use the [issues](https://github.com/username/defense-image-processing/issues) section or email [email@example.com](mailto:email@example.com).
