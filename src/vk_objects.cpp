#include "vk_objects.hpp"

#include <memory>
#include <cstdio>

vk_instance::vk_instance () : instance (VK_NULL_HANDLE)
{
    printf ("vk_instance::vk_instance\n");
}

vk_instance::vk_instance (vk_instance&& other) noexcept
{
    printf ("vk_instance move constructor\n");

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