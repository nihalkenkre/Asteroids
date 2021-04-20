#ifndef VK_GRAPHICS_PIPELINE_LAYOUT_HPP
#define VK_GRAPHICS_PIPELINE_LAYOUT_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_graphics_pipeline_layout
{
public:
    vk_graphics_pipeline_layout () : graphics_pipeline_layout (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_graphics_pipeline_layout (
        const VkDevice& device,
        const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts,
        const std::vector<VkPushConstantRange>& push_constant_ranges
    );

    vk_graphics_pipeline_layout (const vk_graphics_pipeline_layout& other) = delete;
    vk_graphics_pipeline_layout& operator= (const vk_graphics_pipeline_layout& other) = delete;

    vk_graphics_pipeline_layout (vk_graphics_pipeline_layout&& other) noexcept;
    vk_graphics_pipeline_layout& operator= (vk_graphics_pipeline_layout&& other) noexcept;

    ~vk_graphics_pipeline_layout () noexcept;

    VkPipelineLayout graphics_pipeline_layout;

private:
    VkDevice device;
};


#endif