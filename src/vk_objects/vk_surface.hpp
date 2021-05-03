#ifndef VK_SURFACE_HPP
#define VK_SURFACE_HPP

#include <Windows.h>
#include <vulkan/vulkan.h>

class vk_surface
{
public:
    vk_surface () : surface (VK_NULL_HANDLE), instance (VK_NULL_HANDLE) {}
    vk_surface (const VkInstance& instance, 
                const HINSTANCE& h_instance,
                const HWND& h_wnd);
    
    vk_surface (const vk_surface& other) = delete;
    vk_surface& operator= (const vk_surface& other) = delete;

    vk_surface (vk_surface&& other) noexcept;
    vk_surface& operator= (vk_surface&& other) noexcept;

    ~vk_surface () noexcept;

    void get_surface_details (const VkPhysicalDevice& physical_device, const uint32_t& graphics_queue_family_index);

    VkSurfaceKHR surface;
    VkPresentModeKHR present_mode;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR format;

private:
    VkInstance instance;
};


#endif