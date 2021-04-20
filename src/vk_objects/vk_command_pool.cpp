#include "vk_command_pool.hpp"

#include <error.hpp>
#include <cstdio>
#include <memory>


vk_command_pool::vk_command_pool (const VkDevice& device,
                                    const uint32_t& queue_family_index,
                                    const VkCommandPoolCreateFlags& flags) : device (device)
{
    printf ("vk_command_pool::vk_command_pool\n");

    VkCommandPoolCreateInfo create_info = {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        nullptr,
        flags,
        queue_family_index
    };
    
    VkResult result = vkCreateCommandPool (device, &create_info, nullptr, &command_pool);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_COMMAND_POOL;
    }
}

vk_command_pool::vk_command_pool (vk_command_pool&& other) noexcept
{
    printf ("vk_command_pool move ctor\n");
    *this = std::move (other);
}

vk_command_pool& vk_command_pool::operator= (vk_command_pool&& other) noexcept
{
    printf ("vk_command_pool move assignment\n");
    command_pool = other.command_pool;
    device = other.device;

    other.command_pool = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_command_pool::~vk_command_pool () noexcept
{
    printf ("vk_command_pool::~vk_command_pool\n");
    if (command_pool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool (device, command_pool, nullptr);
    }
}
