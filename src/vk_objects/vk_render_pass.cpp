#include "vk_objects/vk_render_pass.hpp"
#include "error.hpp"

#include <cstdio>

vk_render_pass::vk_render_pass (
    const VkDevice& device, 
    const std::vector <VkAttachmentDescription>& color_attachment_descriptions,
    const std::vector <VkAttachmentReference>& color_attachment_references,
    const std::vector <VkSubpassDescription>& color_subpass_descriptions) : device (device)
{
    printf ("vk_render_pass::vk_render_pass\n");

    VkRenderPassCreateInfo create_info{
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        nullptr,
        0,
        static_cast<uint32_t> (color_attachment_descriptions.size ()),
        color_attachment_descriptions.data (),
        static_cast<uint32_t> (color_subpass_descriptions.size ()),
        color_subpass_descriptions.data (),
        0,
        nullptr
    };

    VkResult result = vkCreateRenderPass (device, &create_info, nullptr, &render_pass);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_RENDER_PASS;
    }
}

vk_render_pass::vk_render_pass (vk_render_pass&& other) noexcept
{
    printf ("vk_render_pass move ctor\n");

    *this = std::move (other);
}

vk_render_pass& vk_render_pass::operator=(vk_render_pass&& other) noexcept
{
    printf ("vk_render_pass move assignment\n");

    render_pass = other.render_pass;
    device = other.device;

    other.render_pass = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_render_pass::~vk_render_pass () noexcept
{
    printf ("vk_render_pass::~vk_render_pass\n");

    if (render_pass != VK_NULL_HANDLE &&
        device != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass (device, render_pass, nullptr);
    }
}
