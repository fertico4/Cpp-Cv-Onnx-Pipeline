cmake_minimum_required(VERSION 3.20)
project(cpp-cv-onnx-pipeline VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# 1. Automatic Homebrew Path Resolution (macOS / Apple Silicon / Intel)
# -----------------------------------------------------------------------------
if(APPLE)
    # Detectar brew en Apple Silicon (/opt/homebrew) o Intel (/usr/local)
    find_program(HOMEBREW_EXECUTABLE brew PATHS /opt/homebrew/bin /usr/local/bin)

    if(HOMEBREW_EXECUTABLE)
        execute_process(
                COMMAND ${HOMEBREW_EXECUTABLE} --prefix
                OUTPUT_VARIABLE HOMEBREW_PREFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Detected Homebrew Prefix: ${HOMEBREW_PREFIX}")

        # Añadir las rutas de Homebrew al motor de búsqueda de CMake
        list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
        list(APPEND CMAKE_INCLUDE_PATH "${HOMEBREW_PREFIX}/include")
        list(APPEND CMAKE_LIBRARY_PATH "${HOMEBREW_PREFIX}/lib")
    endif()
endif()

# -----------------------------------------------------------------------------
# 2. OpenCV Integration
# -----------------------------------------------------------------------------
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# -----------------------------------------------------------------------------
# 3. ONNX Runtime Integration (Cross-Platform)
# -----------------------------------------------------------------------------
set(ONNXRUNTIME_ROOT_DIR "" CACHE PATH "Path to ONNX Runtime installation directory")

if(ONNXRUNTIME_ROOT_DIR)
    list(APPEND CMAKE_INCLUDE_PATH "${ONNXRUNTIME_ROOT_DIR}/include")
    list(APPEND CMAKE_LIBRARY_PATH "${ONNXRUNTIME_ROOT_DIR}/lib")
endif()

# Buscar la cabecera principal de la API de C++ de ONNX Runtime
find_path(ONNXRUNTIME_INCLUDE_DIR
        NAMES onnxruntime_cxx_api.h
        PATH_SUFFIXES onnxruntime onnxruntime/core/session
        PATHS
        /opt/homebrew/include
        /opt/homebrew/opt/onnxruntime/include
        /usr/local/include
        /usr/local/opt/onnxruntime/include
)

# Buscar la librería de enlace dinamico/estático
find_library(ONNXRUNTIME_LIBRARY
        NAMES onnxruntime
        PATH_SUFFIXES onnxruntime
        PATHS
        /opt/homebrew/lib
        /opt/homebrew/opt/onnxruntime/lib
        /usr/local/lib
        /usr/local/opt/onnxruntime/lib
)

if(NOT ONNXRUNTIME_INCLUDE_DIR OR NOT ONNXRUNTIME_LIBRARY)
    message(FATAL_ERROR "ONNX Runtime headers or libraries not found.\n"
            "Header status: ${ONNXRUNTIME_INCLUDE_DIR}\n"
            "Library status: ${ONNXRUNTIME_LIBRARY}\n"
            "Please verify installation with 'brew install onnxruntime' or pass -DONNXRUNTIME_ROOT_DIR=/path/to/onnxruntime")
endif()

message(STATUS "ONNX Runtime include path: ${ONNXRUNTIME_INCLUDE_DIR}")
message(STATUS "ONNX Runtime library: ${ONNXRUNTIME_LIBRARY}")

# -----------------------------------------------------------------------------
# 4. Target Configuration
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${ONNXRUNTIME_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OpenCV_LIBS}
        ${ONNXRUNTIME_LIBRARY}
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)