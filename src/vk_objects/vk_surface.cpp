#include "vk_surface.hpp"

#include "error.hpp"

#include <vulkan/vulkan_win32.h>
#include <vector>

vk_surface::vk_surface (const VkInstance& instance, 
                        const HINSTANCE& h_instance,
                        const HWND& h_wnd) : instance (instance)
{
    printf ("vk_surface::vk_surface\n");

    VkWin32SurfaceCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        nullptr,
        0,
        h_instance,
        h_wnd
    };

    VkResult result = vkCreateWin32SurfaceKHR (instance, &create_info, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SURFACE;
    }
}

vk_surface::vk_surface (vk_surface&& other) noexcept
{
    printf ("vk_surface move ctor\n");

    *this = std::move (other);
}

vk_surface& vk_surface::operator= (vk_surface&& other) noexcept
{
    printf ("vk_surface move assignment\n");

    present_mode = other.present_mode;
    capabilities = other.capabilities;
    format = other.format;

    surface = other.surface;
    instance = other.instance;

    other.surface = VK_NULL_HANDLE;
    other.instance = VK_NULL_HANDLE;

    return *this;
}

vk_surface::~vk_surface () noexcept
{
    printf ("vk_surface::~vk_surface\n");

    if (surface != VK_NULL_HANDLE && instance != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR (instance, surface, nullptr);
    }
}

void vk_surface::get_surface_details (const VkPhysicalDevice& physical_device, const uint32_t& graphics_queue_family_index)
{
    VkBool32 is_supported = VK_FALSE;
    VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR (physical_device, graphics_queue_family_index, surface, &is_supported);

    if (result != VK_SUCCESS || !is_supported)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_SUPPORT;
    }

    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physical_device, surface, &capabilities);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_CAPABILITIES;
    }
    
    uint32_t format_count = 0;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR (physical_device, surface, &format_count, nullptr);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_FORMATS;
    }

    std::vector<VkSurfaceFormatKHR> surface_formats (format_count);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR (physical_device, surface, &format_count, surface_formats.data ());
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_FORMATS;
    }
    auto format_iter = std::find_if (surface_formats.begin (), 
                                     surface_formats.end (), 
                                     [&](VkSurfaceFormatKHR format) 
                                     { 
                                         return format.format == VK_FORMAT_B8G8R8A8_UNORM;
                                     });

    if (format_iter != surface_formats.end ())
    {
        format = *format_iter;
    }

    uint32_t present_mode_count = 0;
    result = vkGetPhysicalDeviceSurfacePresentModesKHR (physical_device, surface, &present_mode_count, nullptr);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_PRESENT_MODES;
    }

    std::vector<VkPresentModeKHR> present_modes (present_mode_count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR (physical_device, surface, &present_mode_count, present_modes.data ());
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_SURFACE_PRESENT_MODES;
    }

    auto present_mode_iter = std::find_if (present_modes.begin (), 
                                           present_modes.end (), 
                                           [&](const VkPresentModeKHR& present_mode) 
                                           { 
                                               return present_mode == VK_PRESENT_MODE_MAILBOX_KHR;
                                           }
                                           );
    if (present_mode_iter != present_modes.end ())
    {
        present_mode = *present_mode_iter;
    }
    else
    {
        auto present_mode_iter = std::find_if (present_modes.begin (), 
                                               present_modes.end (), 
                                               [&](const VkPresentModeKHR& present_mode) 
                                               { 
                                                   return present_mode == VK_PRESENT_MODE_FIFO_KHR;
                                               }
                                               );
        present_mode = *present_mode_iter;
    }
}