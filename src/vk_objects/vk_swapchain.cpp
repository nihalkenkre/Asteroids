#include "vk_swapchain.hpp"

#include "error.hpp"


vk_swapchain::vk_swapchain (const VkDevice& device, const vk_surface& surface) : device (device)
{
    printf ("vk_swapchain::vk_swapchain\n");

    VkSwapchainCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        nullptr,
        0,
        surface.surface,
        surface.capabilities.minImageCount + 1,
        surface.format.format,
        surface.format.colorSpace,
        surface.capabilities.currentExtent,
        1,
        surface.capabilities.supportedUsageFlags,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        {},
        surface.capabilities.currentTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        surface.present_mode
    };

    VkResult result = vkCreateSwapchainKHR (device, &create_info, nullptr, &swapchain);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SWAPCHAIN;
    }
}

vk_swapchain::vk_swapchain (vk_swapchain&& other) noexcept
{
    printf ("vk_swapchain move ctor\n");

    *this = std::move (other);
}

vk_swapchain& vk_swapchain::operator= (vk_swapchain&& other) noexcept
{
    printf ("vk_swapchain move assignment\n");

    swapchain = other.swapchain;
    device = other.device;

    other.swapchain = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_swapchain::~vk_swapchain () noexcept
{
    printf ("vk_swapchain::~vk_swapchain\n");

    if (swapchain != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR (device, swapchain, nullptr);
    }
}