#ifndef VK_GRAPHICS_PIPELINE_HPP
#define VK_GRAPHICS_PIPELINE_HPP

#include <vulkan/vulkan.h>

class vk_graphics_pipeline
{
public:
    vk_graphics_pipeline () : graphics_pipeline (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_graphics_pipeline (const vk_graphics_pipeline& other) = delete;
    vk_graphics_pipeline& operator= (const vk_graphics_pipeline& other) = delete;

    vk_graphics_pipeline (vk_graphics_pipeline&& other) noexcept;
    vk_graphics_pipeline& operator= (vk_graphics_pipeline&& other) noexcept;

    ~vk_graphics_pipeline () noexcept;

    VkPipeline graphics_pipeline;

private:
    VkDevice device;
};


#endif