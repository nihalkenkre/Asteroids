#ifndef VK_OBJECTS_HPP
#define VK_OBJECTS_HPP

#include <vulkan/vulkan.h>

class vk_instance
{
public:
    vk_instance ();

    vk_instance (const vk_instance& other) = delete;
    vk_instance& operator+ (const vk_instance& other) = delete;

    vk_instance (vk_instance&& other) noexcept;
    vk_instance& operator= (vk_instance&& other) noexcept;

    ~vk_instance () noexcept;

private:
    VkInstance instance;
};


class vk_surface
{
public:
    vk_surface ();
    
    vk_surface (const vk_surface& other) = delete;
    vk_surface& operator= (const vk_surface& other) = delete;

    vk_surface (vk_surface&& other) noexcept;
    vk_surface& operator= (vk_surface&& other) noexcept;

    ~vk_surface ();

private:
    VkSurfaceKHR surface;
};

#endif 
