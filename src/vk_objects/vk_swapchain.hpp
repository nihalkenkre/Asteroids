#ifndef VK_SWAPCHAIN_HPP
#define VK_SWAPCHAIN_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "vk_objects/vk_image_view.hpp"
#include "vk_objects/vk_surface.hpp"

class vk_swapchain
{
public:
    vk_swapchain () : swapchain (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_swapchain (const VkDevice& device, const vk_surface& surface);
    
    vk_swapchain (const vk_swapchain& other) = delete;
    vk_swapchain& operator= (const vk_swapchain& other) = delete;

    vk_swapchain (vk_swapchain&& other) noexcept;
    vk_swapchain& operator= (vk_swapchain&& other) noexcept;

    ~vk_swapchain () noexcept;

    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<vk_image_view> image_views;

private:
    VkDevice device;
};


#endif