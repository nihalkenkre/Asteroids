#include "vk_descriptor_set_layout.hpp"

#include "error.hpp"

vk_descriptor_set_layout::vk_descriptor_set_layout (
    const VkDevice& device,
    const std::vector<VkDescriptorSetLayoutBinding>& bindings) : device (device)
{
    printf ("vk_descriptor_set_layout::vk_descriptor_set_layout\n");

    VkDescriptorSetLayoutCreateInfo create_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        bindings.size (),
        bindings.data ()
    };

    VkResult result = vkCreateDescriptorSetLayout (device, &create_info, nullptr, &descriptor_set_layout);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_DESCRIPTOR_SET_LAYOUT;
    }
}

vk_descriptor_set_layout::vk_descriptor_set_layout (vk_descriptor_set_layout&& other) noexcept
{
    printf ("vk_descriptor_set_layout move ctor\n");

    *this = std::move (other);
}

vk_descriptor_set_layout& vk_descriptor_set_layout::operator= (vk_descriptor_set_layout&& other) noexcept
{
    printf ("vk_descriptor_set_layout move assignment\n");

    descriptor_set_layout = other.descriptor_set_layout;
    device = other.device;

    other.descriptor_set_layout = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_descriptor_set_layout::~vk_descriptor_set_layout () noexcept
{
    printf ("vk_descriptor_set_layout::~vk_descriptor_set_layout\n");

    if (descriptor_set_layout != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout (device, descriptor_set_layout, nullptr);
    }
}