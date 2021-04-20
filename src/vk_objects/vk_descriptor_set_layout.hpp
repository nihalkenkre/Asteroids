#ifndef VK_DESCRIPTOR_SET_LAYOUT_HPP
#define VK_DESCRIPTOR_SET_LAYOUT_HPP

#include <vulkan/vulkan.h>
#include <vector>


class vk_descriptor_set_layout
{
public:
    vk_descriptor_set_layout () : descriptor_set_layout (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_descriptor_set_layout (
        const VkDevice& device,
        const std::vector<VkDescriptorSetLayoutBinding>& bindings
    );

    vk_descriptor_set_layout (const vk_descriptor_set_layout& other) = delete;
    vk_descriptor_set_layout& operator= (const vk_descriptor_set_layout& other) = delete;

    vk_descriptor_set_layout (vk_descriptor_set_layout&& other) noexcept;
    vk_descriptor_set_layout& operator= (vk_descriptor_set_layout&& other) noexcept;

    ~vk_descriptor_set_layout () noexcept;

    VkDescriptorSetLayout descriptor_set_layout;

private:
    VkDevice device;
};



#endif