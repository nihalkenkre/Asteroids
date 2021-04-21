#include "vk_image_view.hpp"

#include "error.hpp"
#include <cstdio>
#include <memory>


vk_image_view::vk_image_view (
    const VkDevice& device, const VkImage& image,
    const VkImageViewType& type,
    const VkFormat& format,
    const VkComponentMapping& component_mapping,
    const VkImageSubresourceRange& subresource_range) : device (device)
{
    printf ("vk_image_view::vk_image_view\n");

    VkImageViewCreateInfo create_info = {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        nullptr,
        0,
        image,
        type,
        format,
        component_mapping,
        subresource_range
    };

    VkResult result = vkCreateImageView (device, &create_info, nullptr, &image_view);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_IMAGE_VIEW;
    }
}

vk_image_view::vk_image_view (vk_image_view&& other) noexcept
{
    printf ("vk_image_view move ctor\n");

    *this = std::move (other);
}

vk_image_view& vk_image_view::operator=(vk_image_view&& other) noexcept
{
    printf ("vk_image_view move assignment\n");

    image_view = other.image_view;
    device = other.device;

    other.image_view = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_image_view::~vk_image_view () noexcept
{
    printf ("vk_image_view::~vk_image_view\n");

    if (image_view != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyImageView (device, image_view, nullptr);
    }
}
