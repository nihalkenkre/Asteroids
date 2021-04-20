#ifndef VK_INSTANCE_HPP
#define VK_INSTANCE_HPP

#include <vulkan/vulkan.h>


class vk_instance
{
public:
    vk_instance ();

    vk_instance (const vk_instance& other) = delete;
    vk_instance& operator= (const vk_instance& other) = delete;

    vk_instance (vk_instance&& other) noexcept;
    vk_instance& operator= (vk_instance&& other) noexcept;

    ~vk_instance () noexcept;

    VkInstance instance;
};


#endif