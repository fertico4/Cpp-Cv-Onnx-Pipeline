//
// Created by Fernando Jesús Pérez Martín on 3/10/26.
//

#include "InferenceEngine.h"
#include <iostream>

namespace Pipeline
{
    InferenceEngine::InferenceEngine() : m_env(ORT_LOGGING_LEVEL_WARNING, "InferenceEngine")
    {
        m_sessionOptions.SetIntraOpNumThreads(1);
        m_sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    }

    bool InferenceEngine::LoadModel(const std::string &ModelPath)
    {
        try
        {
            m_session = std::make_unique<Ort::Session>(m_env, ModelPath.c_str(), m_sessionOptions);
            std::cout << "[InferenceEngine] Successfully loaded ONNX model: " << ModelPath << std::endl;
            return true;
        } catch (const Ort::Exception& Exception)
        {
            std::cerr << "[InferenceEngine] ONNX Runtime Error: " << Exception.what() << std::endl;
            return false;
        }
    }

    std::vector<float> InferenceEngine::RunInference(const std::vector<float> &InputTensor, const std::vector<int64_t> &InputDims)
    {
        if (!m_session)
        {
            std::cerr << "[InferenceEngine] Error: Model not initialized." << std::endl;
            return {};
        }

        Ort::MemoryInfo MemoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        Ort::Value InputTensorValue = Ort::Value::CreateTensor<float>(
                MemoryInfo,
                const_cast<float*>(InputTensor.data()),
                InputTensor.size(),
                InputDims.data(),
                InputDims.size()
            );

        const char* InputNames[] = {"input"};
        const char* OutputNames[] = {"output"};

        try
        {
            auto OutputTensors = m_session->Run(
                Ort::RunOptions{nullptr},
                InputNames,
                &InputTensorValue,
                1,
                OutputNames,
                1
            );

            float* FloatArray = OutputTensors[0].GetTensorMutableData<float>();
            size_t TotalElements = OutputTensors[0].GetTensorTypeAndShapeInfo().GetElementCount();

            return std::vector<float>(FloatArray, FloatArray + TotalElements);
        } catch (const Ort::Exception& Exception)
        {
            std::cerr << "[InferenceEngine] Inference execution error: " << Exception.what() << std::endl;
            return {};
        }
    }
}
