//
// Created by Fernando Jesús Pérez Martín on 2/10/26.
//
#pragma once

#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>
#include <memory>

namespace Pipeline
{
    class InferenceEngine
    {
    public:
        InferenceEngine();
        ~InferenceEngine() = default;

        bool LoadModel(const std::string& ModelPath);
        std::vector<float> RunInference(const std::vector<float>& InputTensor, const std::vector<int64_t>& InputDims);

    private:
        Ort::Env m_env;
        Ort::SessionOptions m_sessionOptions;
        std::unique_ptr<Ort::Session> m_session;

        std::vector<std::string> m_inputNodeNames;
        std::vector<std::string> m_outputNodeNames;
    };
}