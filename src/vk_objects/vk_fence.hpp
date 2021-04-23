#ifndef VK_FENCE_HPP
#define VK_FENCE_HPP

#include <vulkan/vulkan.h>

class vk_fence
{
public:
    vk_fence () : fence (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_fence (const VkDevice& device);

    vk_fence (const vk_fence& other) = delete;
    vk_fence& operator= (const vk_fence& other) = delete;

    vk_fence (vk_fence&& other) noexcept;
    vk_fence& operator= (vk_fence&& other) noexcept;

    ~vk_fence () noexcept;

    VkFence fence;

private:
    VkDevice device;

};

#endif