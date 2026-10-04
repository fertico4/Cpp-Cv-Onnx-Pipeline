//
// Created by Fernando Jesús Pérez Martín on 2/10/26.
//

#include "ImageProcessor.h"
#include <iostream>

namespace Pipeline
{
    bool ImageProcessor::LoadImage(const std::string& FilePath, cv::Mat& OutImage)
    {
        OutImage = cv::imread(FilePath, cv::IMREAD_COLOR);
        if (OutImage.empty())
        {
            std::cerr << "Failed to load image: " << FilePath << std::endl;
            return false;
        }
        return true;
    }

    std::vector<float> ImageProcessor::PreprocessForInterence(const cv::Mat &InputImage, int TargetWidth, int TargetHeight)
    {
        cv::Mat ResizedImage;
        cv::resize(InputImage, ResizedImage, cv::Size(TargetWidth, TargetHeight));

        cv::Mat RGBImage;
        cv::cvtColor(ResizedImage, RGBImage, cv::COLOR_BGR2RGB);

        RGBImage.convertTo(RGBImage, CV_32FC3, 1.f / 255.f);

        // Convert HWC format to NCHW format required by ONNX models
        std::vector<float> InputTensorValues(1 * 3 * TargetHeight * TargetWidth);
        int SpatialArea = TargetWidth * TargetHeight;

        for (int h = 0; h < TargetHeight; ++h)
        {
            for (int w = 0; w < TargetWidth; ++w)
            {
                cv::Vec3f Pixel = RGBImage.at<cv::Vec3f>(h, w);
                InputTensorValues[0 * SpatialArea + h * TargetWidth + w] = Pixel[0];    // R
                InputTensorValues[1 * SpatialArea + h * TargetWidth + w] = Pixel[1];    // G
                InputTensorValues[2 * SpatialArea + h * TargetWidth + w] = Pixel[2];    // B
            }
        }

        return InputTensorValues;
    }

    cv::Mat ImageProcessor::ApplyEdgeDetection(const cv::Mat& InputImage)
    {
        cv::Mat Gray, Blurred, Edges;
        cv::cvtColor(InputImage, Gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(Gray, Blurred, cv::Size(5, 5), 1.5f);
        cv::Canny(Blurred, Edges, 100, 200);
        return Edges;
    }
}
