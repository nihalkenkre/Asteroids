#ifndef VK_GRAPHICS_DEVICE_HPP
#define VK_GRAPHICS_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_graphics_device
{
public:
    vk_graphics_device () : graphics_device (VK_NULL_HANDLE) {}
    vk_graphics_device (const VkPhysicalDevice& physical_device, 
                        const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos);

    vk_graphics_device (const vk_graphics_device& other) = delete;
    vk_graphics_device& operator= (const vk_graphics_device& other) = delete;

    vk_graphics_device (vk_graphics_device&& other) noexcept;
    vk_graphics_device& operator= (vk_graphics_device&& other) noexcept;

    ~vk_graphics_device () noexcept;

    VkDevice graphics_device;
};

#endif