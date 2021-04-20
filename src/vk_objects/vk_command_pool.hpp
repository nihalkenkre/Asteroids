#ifndef VK_COMMAND_POOL_HPP
#define VK_COMMAND_POOL_HPP

#include <vulkan/vulkan.h>


class vk_command_pool
{
public:
    vk_command_pool () : command_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_command_pool (const VkDevice& device, 
                        const uint32_t& queue_family_index,
                        const VkCommandPoolCreateFlags& flags
                    );

    vk_command_pool (const vk_command_pool& other) = delete;
    vk_command_pool& operator= (const vk_command_pool& other) = delete;

    vk_command_pool (vk_command_pool&& other) noexcept;
    vk_command_pool& operator= (vk_command_pool&& other) noexcept;

    ~vk_command_pool () noexcept;

    VkCommandPool command_pool;

private:
    VkDevice device;
};


#endif