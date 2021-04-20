#ifndef VK_BUFFER_HPP
#define VK_BUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "utils.hpp"

class vk_buffer
{
public:
    vk_buffer () : buffer (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_buffer (
        const VkDevice& device,
        const VkDeviceSize& size,
        const VkBufferUsageFlags usage_flags,
        const VkSharingMode& sharing_mode,
        const uint32_t& queue_family_index);

    vk_buffer (
        const VkDevice& device,
        const buffer_data& buff
    );

    vk_buffer (const vk_buffer& other) = delete;
    vk_buffer& operator= (const vk_buffer& other) = delete;

    vk_buffer (vk_buffer&& other) noexcept;
    vk_buffer& operator= (vk_buffer&& other) noexcept;

    ~vk_buffer () noexcept;

    void copy_to_buffer (
        const VkBuffer& dst_buffer,
        const VkDeviceSize& size,
        const VkCommandPool& command_pool,
        const VkQueue& transfer_queue
    ) const;

    void copy_to_images (
        const std::vector<VkImage>& images,
        const std::vector<VkExtent3D>& extents,
        const std::vector<VkDeviceSize>& offsets,
        const VkCommandPool& command_pool, 
        const VkQueue& transfer_queue
    ) const;

    VkBuffer buffer;

private:
    VkDevice device;
};

#endif