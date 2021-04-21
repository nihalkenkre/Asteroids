#ifndef VK_IMAGE_VIEW_HPP
#define VK_IMAGE_VIEW_HPP

#include <vulkan/vulkan.h>


class vk_image_view
{
public:
    vk_image_view () : image_view (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_image_view (
        const VkDevice& device, const VkImage& image,
        const VkImageViewType& type,
        const VkFormat& format,
        const VkComponentMapping& component_mapping,
        const VkImageSubresourceRange& subresource_range
    );

    vk_image_view (const vk_image_view& other) = delete;
    vk_image_view& operator= (const vk_image_view& other) = delete;

    vk_image_view (vk_image_view&& other) noexcept;
    vk_image_view& operator= (vk_image_view&& other) noexcept;

    ~vk_image_view () noexcept;

    VkImageView image_view;

private:
    VkDevice device;
};


#endif