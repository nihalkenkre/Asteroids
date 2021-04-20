#ifndef VK_RENDER_PASS_HPP
#define VK_RENDER_PASS_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_render_pass
{
public:
    vk_render_pass () : render_pass (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_render_pass (
        const VkDevice& device,
        const std::vector<VkAttachmentDescription>& color_attachment_descriptions,
        const std::vector<VkAttachmentReference>& color_attachment_references,
        const std::vector<VkSubpassDescription>& color_subpass_descriptions
    );

    vk_render_pass (const vk_render_pass& other) = delete;
    vk_render_pass& operator= (const vk_render_pass&& other) = delete;

    vk_render_pass (vk_render_pass&& other) noexcept;
    vk_render_pass& operator= (vk_render_pass&& other) noexcept;

    ~vk_render_pass () noexcept;

    VkRenderPass render_pass;

private:
    VkDevice device;
};

#endif