#include "vk_fence.hpp"

#include "error.hpp"

#include <cstdio>
#include <memory>

vk_fence::vk_fence (const VkDevice& device) : device (device)
{
    printf ("vk_fence::vk_fence\n");

    VkFenceCreateInfo create_info{
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        nullptr,
        0
    };

    VkResult result = vkCreateFence (device, &create_info, nullptr, &fence);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_FENCE;
    }
}

vk_fence::vk_fence (vk_fence&& other) noexcept
{
    printf ("vk_fence move ctor\n");

    *this = std::move (other);
}

vk_fence& vk_fence::operator=(vk_fence&& other) noexcept
{
    printf ("vk_fence move assignment\n");

    fence = other.fence;
    device = other.device;

    other.fence = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_fence::~vk_fence () noexcept
{
    printf ("vk_fence::~vk_fence\n");

    if (fence != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyFence (device, fence, nullptr);
    }
}


