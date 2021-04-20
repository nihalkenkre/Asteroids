#include "vk_instance.hpp"
#include "error.hpp"

#include <Windows.h>
#include <vulkan/vulkan_win32.h>

#include <vector>


vk_instance::vk_instance ()
{
    printf ("vk_instance::vk_instance\n");

    std::vector<const char*> requested_extensions;
    requested_extensions.push_back (VK_KHR_SURFACE_EXTENSION_NAME);
    requested_extensions.push_back (VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    VkApplicationInfo application_info = {
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        nullptr,
        "Asteroids",
        VK_MAKE_VERSION (1, 0, 0),
        "AGE",
        VK_MAKE_VERSION (1, 0, 0),
        VK_MAKE_VERSION (1, 2, 0)
    };

    VkInstanceCreateInfo create_info = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr,
        0,
        &application_info, 
        0,
        nullptr,
        static_cast<uint32_t> (requested_extensions.size ()),
        requested_extensions.data ()
    };

    VkResult result = vkCreateInstance (&create_info, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_INSTANCE;
    }
}

vk_instance::vk_instance (vk_instance&& other) noexcept
{
    printf ("vk_instance move ctor\n");

    *this = std::move (other);
}

vk_instance& vk_instance::operator= (vk_instance&& other) noexcept
{
    printf ("vk_instance move assignment\n");

    instance = other.instance;

    other.instance = VK_NULL_HANDLE;

    return *this;
}

vk_instance::~vk_instance ()
{
    printf ("vk_instance::~vk_instance\n");

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance (instance, nullptr);
    }
}