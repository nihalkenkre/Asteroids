#include "vk_graphics_pipeline_layout.hpp"

#include "error.hpp"

vk_graphics_pipeline_layout::vk_graphics_pipeline_layout (
    const VkDevice& device, 
    const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts, 
    const std::vector<VkPushConstantRange>& push_constant_ranges) : device (device)
{
    printf ("vk_graphics_pipeline_layout::vk_graphics_pipeline_layout\n");

    VkPipelineLayoutCreateInfo create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        static_cast<uint32_t> (descriptor_set_layouts.size ()),
        descriptor_set_layouts.data (),
        static_cast<uint32_t> (push_constant_ranges.size ()),
        push_constant_ranges.data ()
    };

    VkResult result = vkCreatePipelineLayout (device, &create_info,
        nullptr, &graphics_pipeline_layout);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_PIPELINE_LAYOUT;
    }
}

vk_graphics_pipeline_layout::vk_graphics_pipeline_layout (vk_graphics_pipeline_layout&& other) noexcept
{
    printf ("vk_graphics_pipeline_layout move ctor\n");

    *this = std::move (other);
}

vk_graphics_pipeline_layout& vk_graphics_pipeline_layout::operator=(vk_graphics_pipeline_layout&& other) noexcept
{
    printf ("vk_graphics_pipeline_layout move assignment\n");

    graphics_pipeline_layout = other.graphics_pipeline_layout;
    device = other.device;

    other.graphics_pipeline_layout = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_graphics_pipeline_layout::~vk_graphics_pipeline_layout () noexcept
{
    printf ("vk_graphics_pipeline_layout::~vk_graphics_pipeline_layout\n");

    if (graphics_pipeline_layout != VK_NULL_HANDLE && 
        device != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout (device, graphics_pipeline_layout, nullptr);
    }
}


