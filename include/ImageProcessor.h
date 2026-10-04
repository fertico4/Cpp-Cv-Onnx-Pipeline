//
// Created by Fernando Jesús Pérez Martín on 2/10/26.
//

#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace Pipeline
{
    class ImageProcessor
    {
    public:
        ImageProcessor() = default;
        ~ImageProcessor() = default;

        // Load an image from file path
        bool LoadImage(const std::string& FilePath, cv::Mat& OutImage);

        // Resize and normalize image tensor for neural network inputs
        std::vector<float> PreprocessForInterence(const cv::Mat& InputImage, int TargetWidth, int TargetHeight);

        // Apply basic filtering operations (Gaussian Blur & Edge Detection)
        cv::Mat ApplyEdgeDetection(const cv::Mat& InputImage);
    };
}