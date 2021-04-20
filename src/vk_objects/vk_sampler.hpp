#ifndef VK_SAMPLER_HPP
#define VK_SAMPLER_HPP

#include <vulkan/vulkan.h>

class vk_sampler
{
public:
    vk_sampler () : sampler (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_sampler (const VkDevice& device);

    vk_sampler (const vk_sampler& other) = delete;
    vk_sampler& operator= (const vk_sampler& other) = delete;

    vk_sampler (vk_sampler&& other) noexcept;
    vk_sampler& operator= (vk_sampler&& other) noexcept;

    ~vk_sampler () noexcept;

    VkSampler sampler;

private:
    VkDevice device;
};

#endif