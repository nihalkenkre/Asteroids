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

    uint32_t acquire_next_swapchain_image_index (
        const VkSwapchainKHR& swapchain,
        const VkSemaphore& wait_semaphore,
        const VkFence& fence,
        const uint64_t& timeout
    ) const;

    void wait_for_fences (
        const std::vector<VkFence>& fences,
        const VkBool32& wait_all,
        const uint64_t& timeout
    ) const;

    void reset_fences (
        const std::vector<VkFence>& fences
    ) const;

    VkDevice graphics_device;
};

#endif