#ifndef VK_GRAPHICS_PIPELINE_HPP
#define VK_GRAPHICS_PIPELINE_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class vk_graphics_pipeline
{
public:
    vk_graphics_pipeline () : graphics_pipeline (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_graphics_pipeline (
        const VkDevice& device,
        const std::vector<VkShaderModule>& shader_modules,
        const std::vector<std::string>& main_functions,
        const std::vector<VkShaderStageFlagBits>& stage_flags,
        const VkPipelineLayout& graphics_pipeline_layout,
        const VkRenderPass& render_pass,
        const VkExtent2D& extent
    );

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