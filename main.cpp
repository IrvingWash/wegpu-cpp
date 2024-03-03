#include "glfw/include/GLFW/glfw3.h"
#include "dawn/include/webgpu/webgpu.hpp"
#include "glfw3webgpu/glfw3webgpu.h"
#include <vector>
#include <iostream>

WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options) {
    struct UserData {
        WGPUAdapter adapter = nullptr;
        bool requestEnded = false;
    };
    UserData userData;

    auto onAdapterRequestEnded = [](
        WGPURequestAdapterStatus status,
        WGPUAdapter adapter,
        char const* message,
        void* pUserData
    ) {
        UserData& userData = *reinterpret_cast<UserData*>(pUserData);

        if (status == WGPURequestAdapterStatus_Success) {
            userData.adapter = adapter;
        } else {
            std::cout
                << "Could not get WebGPU adapter: "
                << message <<
                std::endl;
        }

        userData.requestEnded = true;
    };

    wgpuInstanceRequestAdapter(
        instance,
        options,
        onAdapterRequestEnded,
        (void*)&userData
    );

    assert(userData.requestEnded);

    return userData.adapter;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW!" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Learn WebGPU", NULL, NULL);

    if (!window) {
        std::cerr << "Could not open window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

    WGPUInstance instance = wgpuCreateInstance(&desc);

    if (!instance) {
        std::cerr << "Could not initialize WebGPU!" << std::endl;
        return 1;
    }

    WGPURequestAdapterOptions options = {};
    options.nextInChain = nullptr;
    WGPUSurface surface = glfwGetWGPUSurface(instance, window);
    options.compatibleSurface = surface;

    WGPUAdapter adapter = requestAdapter(instance, &options);

    std::vector<WGPUFeatureName> features;

    size_t feature_count = wgpuAdapterEnumerateFeatures(adapter, nullptr);

    features.resize(feature_count);

    wgpuAdapterEnumerateFeatures(adapter, features.data());

    std::cout << "Adapter features" << std::endl;

    for (auto feature : features) {
            std::cout << " - " << feature << std::endl;
    }

    wgpuSurfaceRelease(surface);
    wgpuInstanceRelease(instance);
    glfwDestroyWindow(window);
    glfwTerminate();
}
