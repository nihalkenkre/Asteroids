#include "vk_graphics_device.hpp"

#include "error.hpp"

vk_graphics_device::vk_graphics_device (const VkPhysicalDevice& physical_device, 
                                        const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos)
{
    printf ("vk_device::vk_device\n");

    std::vector<const char*> requested_device_extensions;
    requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo create_info = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        nullptr,
        0,
        static_cast<uint32_t> (queue_create_infos.size ()),
        queue_create_infos.data (),
        0,
        nullptr,
        static_cast<uint32_t> (requested_device_extensions.size ()),
        requested_device_extensions.data (),
        nullptr
    };

    VkResult result = vkCreateDevice (physical_device, &create_info, nullptr, &graphics_device);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_GRAPHICS_DEVICE;   
    }
}

vk_graphics_device::vk_graphics_device (vk_graphics_device&& other) noexcept
{
    printf ("vk_device::move ctor\n");

    *this = std::move (other);
}

vk_graphics_device& vk_graphics_device::operator= (vk_graphics_device&& other) noexcept
{
    printf ("vk_device::move assignment\n");

    graphics_device = other.graphics_device;

    other.graphics_device = VK_NULL_HANDLE;

    return *this;
}

vk_graphics_device::~vk_graphics_device () noexcept
{
    printf ("vk_device::~vk_device\n");

    if (graphics_device != VK_NULL_HANDLE)
    {
        vkDestroyDevice (graphics_device, nullptr);
    }
}

uint32_t vk_graphics_device::acquire_next_swapchain_image_index (
    const VkSwapchainKHR& swapchain,
    const VkSemaphore& wait_semaphore,
    const VkFence& fence,
    const uint64_t& timeout) const
{
    uint32_t image_index = 0;

    VkResult result = vkAcquireNextImageKHR (graphics_device, swapchain, timeout, wait_semaphore, fence, &image_index);
    if (result != VK_SUCCESS &&
        result != VK_SUBOPTIMAL_KHR &&
        result != VK_ERROR_OUT_OF_DATE_KHR &&
        result != VK_TIMEOUT &&
        result != VK_NOT_READY)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ACQUIRE_NEXT_IMAGE;
    }

    return image_index;
}

void vk_graphics_device::wait_for_fences (const std::vector<VkFence>& fences, const VkBool32& wait_all, const uint64_t& timeout) const
{
    VkResult result = vkWaitForFences (
        graphics_device,
        static_cast<uint32_t> (fences.size ()),
        fences.data (),
        wait_all,
        timeout
    );

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_WAIT_FOR_FENCES;
    }
}

void vk_graphics_device::reset_fences (const std::vector<VkFence>& fences) const
{
    VkResult result = vkResetFences (graphics_device, static_cast<uint32_t> (fences.size ()), fences.data ());

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_RESET_FENCES;
    }
}
