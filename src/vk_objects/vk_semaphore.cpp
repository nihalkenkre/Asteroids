#include "vk_semaphore.hpp"

#include "error.hpp"

#include <cstdio>
#include <memory>

vk_semaphore::vk_semaphore (const VkDevice& device) : device (device)
{
    printf ("vk_semaphore::vk_semaphore\n");

    VkSemaphoreCreateInfo create_info{
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        nullptr,
        0
    };

    VkResult result = vkCreateSemaphore (device, &create_info, nullptr, &semaphore);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SEMAPHORE;
    }
}

vk_semaphore::vk_semaphore (vk_semaphore&& other) noexcept
{
    printf ("vk_semaphore move ctor\n");

    *this = std::move (other);
}

vk_semaphore& vk_semaphore::operator= (vk_semaphore&& other) noexcept
{
    printf ("vk_semaphore move assignment\n");

    semaphore = other.semaphore;
    device = other.device;

    other.semaphore = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_semaphore::~vk_semaphore () noexcept
{
    printf ("vk_semaphore::~vk_semaphore\n");

    if (semaphore != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroySemaphore (device, semaphore, nullptr);
    }
}
