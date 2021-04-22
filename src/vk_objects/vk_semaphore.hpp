#ifndef VK_SEMAPHORE_HPP
#define VK_SEMAPHORE_HPP

#include <vulkan/vulkan.h>

class vk_semaphore
{
public:
    vk_semaphore () : semaphore (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_semaphore (const VkDevice& device);

    vk_semaphore (const vk_semaphore& other) = delete;
    vk_semaphore& operator= (const vk_semaphore& other) = delete;

    vk_semaphore (vk_semaphore&& other) noexcept;
    vk_semaphore& operator= (vk_semaphore&& other) noexcept;

    ~vk_semaphore () noexcept;

    VkSemaphore semaphore;

private:
    VkDevice device;

};

#endif