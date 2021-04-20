#ifndef VK_DESCRIPTOR_POOL_HPP
#define VK_DESCRIPTOR_POOL_HPP

#include <vulkan/vulkan.h>
#include <vector>


class vk_descriptor_pool
{
public:
    vk_descriptor_pool () : descriptor_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_descriptor_pool (
        const VkDevice& device,
        const std::vector<VkDescriptorPoolSize>& pool_sizes,
        const uint32_t& max_sets
    );

    vk_descriptor_pool (const vk_descriptor_pool& other) = delete;
    vk_descriptor_pool& operator= (const vk_descriptor_pool& other) = delete;

    vk_descriptor_pool (vk_descriptor_pool&& other) noexcept;
    vk_descriptor_pool& operator= (vk_descriptor_pool&& other) noexcept;

    ~vk_descriptor_pool () noexcept;


    VkDescriptorPool descriptor_pool;

private:
    VkDevice device;
};

#endif