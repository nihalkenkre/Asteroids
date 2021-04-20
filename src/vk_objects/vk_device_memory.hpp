#ifndef VK_DEVICE_MEMORY_HPP
#define VK_DEVICE_MEMORY_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include "vk_objects/vk_types.hpp"

typedef void* HANDLE;

class vk_device_memory
{
public:
    vk_device_memory () : memory (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_device_memory (
        const VkDevice& device,
        const std::vector<VkBuffer>& vk_type_objects,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );

    vk_device_memory (
        const VkDevice& device,
        const std::vector<VkImage>& vk_type_objects,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );

    vk_device_memory (const vk_device_memory& other) = delete;
    vk_device_memory& operator= (const vk_device_memory& other) = delete;

    vk_device_memory (vk_device_memory&& other) noexcept;
    vk_device_memory& operator= (vk_device_memory&& other) noexcept;

    ~vk_device_memory () noexcept;

    HANDLE map (const VkDeviceSize& offset, const VkDeviceSize& size) const;
    void unmap () const;

    VkDeviceMemory memory;

private:
    VkDevice device;

    void allocate_bind_buffers (const VkDevice& device,
        const std::vector<VkBuffer>& buffers,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types);

    void allocate_bind_images (const VkDevice& device,
        const std::vector<VkImage>& images,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types);
};


#endif