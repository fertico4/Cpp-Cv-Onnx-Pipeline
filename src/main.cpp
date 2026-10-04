//
// Created by Fernando Jesús Pérez Martín on 3/10/26.
//

#include "ImageProcessor.h"
#include "InferenceEngine.h"
#include <iostream>
#include <chrono>

#if defined(__APPLE__)
#include <array>
#endif

// Helper to open native selection dialog inside OS files
std::string OpenFileDialog()
{
    std::string FilePath = "";

#if defined(__APPLE__)
    // Call native AppleScript from macOS to open files selector
    const char* Cmd = "osascript -e 'POSIX path of (choose file with prompt \"Select an image for the CV pipeline:\" of type {\"public.image\"})' 2>/dev/null";
    std::array<char, 256> Buffer;
    std::unique_ptr<FILE, decltype(&pclose)> Pipe(popen(Cmd, "r"), pclose);

    if (Pipe)
    {
        while (fgets(Buffer.data(), Buffer.size(), Pipe.get()) != nullptr)
        {
            FilePath += Buffer.data();
        }
    }

    // Remove final line enter if exists
    if (!FilePath.empty() && FilePath.back() == '\n')
    {
        FilePath.pop_back();
    }
#elif defined(_WIN32)
    // Basic implementation for Windows console (or use GetOpenFileNameA)
    std::cout << "[UI] Enter absolute file path to image: ";
    std::getline(std::cin, FilePath);
#else
    // Linux fallback via zenity/kdialog
    const char* Cmd = "zenity --file-selection --title=\"Select Image\" 2>/dev/null";
    std::array<char, 256> Buffer;
    std::unique_ptr<FILE, decltype(&pclose)> Pipe(popen(Cmd, "r"), pclose);

    if (Pipe)
    {
        while (fgets(Buffer.data(), Buffer.size(), Pipe.get()) != nullptr)
        {
            FilePath += Buffer.data();
        }
    }
    if (!FilePath.empty() && FilePath.back() == '\n')
    {
        FilePath.pop_back();
    }
#endif

    return FilePath;
}


struct UIContext
{
    cv::Mat OriginalImage;
    cv::Mat ProcessedEdges;
    int LowThreshold = 100;
    int HighThreshold = 200;
};

void OnTrackbarChange(int, void* UserData)
{
    UIContext* Context = static_cast<UIContext*>(UserData);
    if (Context->OriginalImage.empty())
        return;

    cv::Mat Gray, Blurred;
    cv::cvtColor(Context->OriginalImage, Gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(Gray, Blurred, cv::Size(5, 5), 1.5f);
    cv::Canny(Blurred, Context->ProcessedEdges, Context->LowThreshold, Context->HighThreshold);

    cv::imshow("C++ CV Pipeline - Interactive Preview", Context->ProcessedEdges);
}

int main(int argc, char** argv)
{
    Pipeline::ImageProcessor ImageProc;
    UIContext Context;

    std::cout << "[UI] Opening native file dialog to select image..." << std::endl;
    std::string SelectedPath = OpenFileDialog();

    // 1. Process Image
    if (SelectedPath.empty() || !ImageProc.LoadImage(SelectedPath, Context.OriginalImage))
    {
        std::cout << "[Pipeline] Creating synthetic placeholder image for demo...";
        Context.OriginalImage = cv::Mat(480, 640, CV_8UC3, cv::Scalar(128, 128, 128));
    }

    // Create graphic OpenCV window
    const std::string WindowName = "C++ CV Pipeline - Interactive Preview";
    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);

    // Interactive controlls (Trackbars) to adjust Canny umbrals
    cv::createTrackbar("Low Threshold", WindowName, &Context.LowThreshold, 255, OnTrackbarChange, &Context);
    cv::createTrackbar("High Threshold", WindowName, &Context.HighThreshold, 255, OnTrackbarChange, &Context);

    // Initial render
    OnTrackbarChange(0, &Context);

    std::cout << "================================================" << std::endl;
    std::cout << " C++ Computer Vision & ONNX Inference Pipeline  " << std::endl;
    std::cout << "UI interface controls:  " << std::endl;
    std::cout << "  - Adjust Sliders to modify live detection  " << std::endl;
    std::cout << "  - Press 'S' to save (output_edges.png)  " << std::endl;
    std::cout << "  - Press 'ESC' or 'Q' to quit  " << std::endl;
    std::cout << "================================================" << std::endl;

    while (true)
    {
        char Key = static_cast<char>(cv::waitKey(30));
        if (Key == 27 || Key == 'q' || Key == 'Q') // ESC or Q to quit
        {
            break;
        }
        if (Key == 's' || Key == 'S') // S to save
        {
            cv::imwrite("assets/output_edges.png", Context.ProcessedEdges);
            std::cout << "[UI] Image saved successfully in assets/output_edges.png!" << std::endl;
        }
    }

    cv::destroyAllWindows();
    return 0;
}