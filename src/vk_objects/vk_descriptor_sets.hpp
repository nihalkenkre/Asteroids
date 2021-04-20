#ifndef VK_DESCRIPTOR_SETS_HPP
#define VK_DESCRIPTOR_SETS_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_descriptor_set
{
public:
    vk_descriptor_set () : descriptor_set (VK_NULL_HANDLE), descriptor_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_descriptor_set (
        const VkDevice& device,
        const VkDescriptorPool& descriptor_pool,
        const VkDescriptorSetLayout& descriptor_set_layout
    );

    vk_descriptor_set (const vk_descriptor_set& other) = delete;
    vk_descriptor_set& operator= (const vk_descriptor_set& other) = delete;

    vk_descriptor_set (vk_descriptor_set&& other) noexcept;
    vk_descriptor_set& operator= (vk_descriptor_set&& other) noexcept;

    ~vk_descriptor_set () noexcept;

    void update (const VkWriteDescriptorSet descriptor_set_write) const;

    VkDescriptorSet descriptor_set;


private:
    VkDescriptorPool descriptor_pool;
    VkDevice device;
};

class vk_descriptor_sets
{
public:
    vk_descriptor_sets () : descriptor_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_descriptor_sets (
        const VkDevice& device,
        const VkDescriptorPool& descriptor_pool,
        const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts
    );

    vk_descriptor_sets (const vk_descriptor_sets& other) = delete;
    vk_descriptor_sets& operator= (const vk_descriptor_sets& other) = delete;

    vk_descriptor_sets (vk_descriptor_sets&& other) noexcept;
    vk_descriptor_sets& operator= (vk_descriptor_sets&& other) noexcept;

    ~vk_descriptor_sets () noexcept;

    void update (const std::vector<VkDescriptorImageInfo>& descriptor_image_infos) const;

    std::vector<VkDescriptorSet> descriptor_sets;

private:
    VkDescriptorPool descriptor_pool;
    VkDevice device;
};


#endif