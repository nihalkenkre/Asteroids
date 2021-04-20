#include "vk_sampler.hpp"

#include "error.hpp"

#include <cstdio>
#include <memory>


vk_sampler::vk_sampler (const VkDevice& device) : device (device)
{
    printf ("vk_sampler::vk_sampler\n");

    VkSamplerCreateInfo create_info = {
        VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        nullptr,
        0,
        VK_FILTER_LINEAR,
        VK_FILTER_LINEAR,
        VK_SAMPLER_MIPMAP_MODE_LINEAR,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        0,
        VK_FALSE,
        0,
        VK_FALSE,
        VK_COMPARE_OP_NEVER,
        0,
        0,
        VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        VK_FALSE
    };

    VkResult result = vkCreateSampler (device, &create_info, nullptr, &sampler);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SAMPLER;
    }
}

vk_sampler::vk_sampler (vk_sampler&& other) noexcept
{
    printf ("vk_sampler move ctor\n");

    *this = std::move (other);
}

vk_sampler& vk_sampler::operator= (vk_sampler&& other) noexcept
{
    printf ("vk_sampler move assignment\n");

    sampler = other.sampler;
    device = other.device;

    other.sampler = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_sampler::~vk_sampler () noexcept
{
    printf ("vk_sampler::~vk_sampler\n");
    
    if (sampler != VK_NULL_HANDLE)
    {
        vkDestroySampler (device, sampler, nullptr);
    }
}
