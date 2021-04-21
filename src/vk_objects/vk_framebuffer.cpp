#include "vk_framebuffer.hpp"

#include "error.hpp"

#include <cstdio>
#include <memory>

vk_framebuffer::vk_framebuffer (
    const VkDevice& device, 
    const VkRenderPass& render_pass, 
    const VkExtent2D& extent,
    const VkImageView& image_view) : device (device)
{
    printf ("vk_framebuffer::vk_framebuffer\n");

    VkFramebufferCreateInfo create_info{
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        nullptr,
        0,
        render_pass,
        1,
        &image_view,
        extent.width,
        extent.height,
        1
    };

    VkResult result = vkCreateFramebuffer (device, &create_info, nullptr, &framebuffer);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_FRAMEBUFFER;
    }
}

vk_framebuffer::vk_framebuffer (vk_framebuffer&& other) noexcept
{
    printf ("vk_frambuffer move ctor\n");

    *this = std::move (other);
}

vk_framebuffer& vk_framebuffer::operator=(vk_framebuffer&& other) noexcept
{
    printf ("vk_framebuffer move assignment\n");

    framebuffer = other.framebuffer;
    device = other.device;

    other.framebuffer = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_framebuffer::~vk_framebuffer () noexcept
{
    printf ("vk_frambuffer::~vk_frambuffer\n");

    if (framebuffer != VK_NULL_HANDLE && 
        device != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer (device, framebuffer, nullptr);
    }
}
