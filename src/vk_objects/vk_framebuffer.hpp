#ifndef VK_FRAMEBUFFER_HPP
#define VK_FRAMEBUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_framebuffer
{
public:
    vk_framebuffer () : framebuffer (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_framebuffer (
        const VkDevice& device,
        const VkRenderPass& render_pass,
        const VkExtent2D& extent,
        const VkImageView& image_view
    );

    vk_framebuffer (const vk_framebuffer& other) = delete;
    vk_framebuffer& operator= (const vk_framebuffer& other) = delete;

    vk_framebuffer (vk_framebuffer&& other) noexcept;
    vk_framebuffer& operator= (vk_framebuffer&& other) noexcept;

    ~vk_framebuffer () noexcept;

    VkFramebuffer framebuffer;

private:
    VkDevice device;
};


#endif