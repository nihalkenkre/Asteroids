#ifndef VK_PHYSICAL_DEVICE_HPP
#define VK_PHYSICAL_DEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <tuple>

class vk_physical_device
{
public:
    vk_physical_device () {}
    vk_physical_device (const VkInstance& instance);

    std::tuple<uint32_t, uint32_t, uint32_t> get_queue_family_indices () const;

    VkPhysicalDevice physical_device;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceLimits limits;
};

#endif