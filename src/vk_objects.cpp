#include "vk_objects.hpp"

#include <memory>
#include <cstdio>
#include <vector>
#include <map>

#include <vulkan/vulkan_win32.h>

#include "error.hpp"
#include "utils.hpp"

#include <stb_image.h>

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
        requested_extensions.size (),
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

vk_physical_device::vk_physical_device (const VkInstance& instance)
{
    printf ("vk_physical_device::vk_physical_device\n");

    uint32_t device_count;
    VkResult result = vkEnumeratePhysicalDevices (instance, &device_count, nullptr);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ENUMERATE_PHYSICAL_DEVICES;
    }

    std::vector<VkPhysicalDevice> physical_devices (device_count);
    result = vkEnumeratePhysicalDevices (instance, &device_count, physical_devices.data ());
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ENUMERATE_PHYSICAL_DEVICES;
    }

    physical_device = physical_devices[0];

    vkGetPhysicalDeviceMemoryProperties (physical_device, &memory_properties);
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties (physical_device, &device_properties);
    
    limits = device_properties.limits;
}

vk_queue_family_indices::vk_queue_family_indices (const VkPhysicalDevice& physical_device)
{
    printf ("vk_queue_family_indices::vk_queue_family_indices\n");

    uint32_t property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &property_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_family_properties (property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &property_count, queue_family_properties.data ());

    graphics_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                std::find_if (queue_family_properties.begin (), 
                                                            queue_family_properties.end (), 
                                                            [&](const VkQueueFamilyProperties& family_property) 
                                                                { 
                                                                    return (family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT);
                                                                }
                                                            )
                                                );

    auto compute_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                   queue_family_properties.end (), 
                                                   [&](const VkQueueFamilyProperties& family_property) 
                                                        { 
                                                            return (family_property.queueFlags & VK_QUEUE_COMPUTE_BIT) && (!(family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT));
                                                        }
                                                   );

    if (compute_family_index_iter != queue_family_properties.end ())
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), compute_family_index_iter);
    }
    else
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                    std::find_if (queue_family_properties.begin (), 
                                                                  queue_family_properties.end (), 
                                                                  [&](const VkQueueFamilyProperties& family_property) 
                                                                  { 
                                                                      return (family_property.queueFlags & VK_QUEUE_COMPUTE_BIT); 
                                                                  }
                                                                  )
                                                    );
    }

    auto transfer_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                    queue_family_properties.end (), 
                                                    [&](const VkQueueFamilyProperties& family_property) 
                                                        { 
                                                            return (family_property.queueFlags & VK_QUEUE_TRANSFER_BIT) && 
                                                                (!(family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT)) && 
                                                                (!(family_property.queueFlags & VK_QUEUE_COMPUTE_BIT)); 
                                                        }
                                                    );
    if (transfer_family_index_iter != queue_family_properties.end ())
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), transfer_family_index_iter);
    }
    else
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                     std::find_if (queue_family_properties.begin (), 
                                                                   queue_family_properties.end (), 
                                                                   [&](const VkQueueFamilyProperties& family_property) 
                                                                        { 
                                                                            return (family_property.queueFlags & VK_QUEUE_TRANSFER_BIT); 
                                                                        }
                                                                   )
                                                     );
    }
}

vk_surface::vk_surface (const VkInstance& instance, 
                        const VkPhysicalDevice& physical_device,
                        const HINSTANCE& h_instance,
                        const HWND& h_wnd,
                        const uint32_t& graphics_queue_family_index) : instance (instance)
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
    
    VkBool32 is_supported = VK_FALSE;
    result = vkGetPhysicalDeviceSurfaceSupportKHR (physical_device, graphics_queue_family_index, surface, &is_supported);

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

vk_surface::vk_surface (vk_surface&& other) noexcept
{
    printf ("vk_surface move ctor\n");

    *this = std::move (other);
}

vk_surface& vk_surface::operator= (vk_surface&& other) noexcept
{
    printf ("vk_surface move assignment\n");

    surface = other.surface;
    present_mode = other.present_mode;
    capabilities = other.capabilities;
    format = other.format;

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

vk_queue_info::vk_queue_info (const vk_queue_family_indices& queue_family_indices)
{
    printf ("vk_queue_info::vk_queue_info\n");

    std::vector<uint32_t> family_indices = { 
                                            queue_family_indices.graphics_queue_family_index, 
                                            queue_family_indices.compute_queue_family_index, 
                                            queue_family_indices.transfer_queue_family_index
                                        };

    std::map<uint32_t, uint32_t> family_indices_queue_count;

    for (const auto& family_index : family_indices)
    {
        family_indices_queue_count[family_index] = std::count (family_indices.begin (), family_indices.end (), family_index);
    }

    for (const auto& family_index_family_count : family_indices_queue_count)
    {
        for (uint32_t i = 0; i < family_index_family_count.second; ++i)
        {
            queue_indices.push_back (i);
        }
    }

    for (const auto& family_index_queue_count : family_indices_queue_count)
    {
        std::vector<float> queue_priorities (family_index_queue_count.second, 1);
        VkDeviceQueueCreateInfo queue_create_info = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            family_index_queue_count.first,
            family_index_queue_count.second,
            queue_priorities.data ()
        };

        queue_create_infos.emplace_back (queue_create_info);
    }
}

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
        queue_create_infos.size (),
        queue_create_infos.data (),
        0,
        nullptr,
        requested_device_extensions.size (),
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

vk_queue::vk_queue (const VkDevice& device, const VkQueue& queue) : device (device), queue (queue)
{
    printf ("vk_queue::vk_queue\n");
}

void vk_queue::submit (const std::vector<VkCommandBuffer>& commands_buffers)
{
    VkSubmitInfo submit_info = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr,
        0,
        nullptr,
        0,
        commands_buffers.size (),
        commands_buffers.data (),
        0,
        nullptr
    };
    
    VkResult vk_result = vkQueueSubmit (queue, 1, &submit_info, VK_NULL_HANDLE);
    if (vk_result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_QUEUE_SUBMIT;
    }
}

vk_device_queues::vk_device_queues (const VkPhysicalDevice& physical_device, 
                      const VkDevice& device, 
                      const vk_queue_family_indices& queue_family_indices, 
                      const std::vector<uint32_t>& queue_indices)

{
    printf ("vk_device_queues::vk_device_queues\n");

    uint32_t property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties (physical_device, &property_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_family_properties (property_count);
    vkGetPhysicalDeviceQueueFamilyProperties (physical_device, &property_count, queue_family_properties.data ());

    uint32_t graphics_queue_index = 0;
	uint32_t compute_queue_index = queue_family_indices.graphics_queue_family_index == queue_family_indices.compute_queue_family_index ? 1 : 0;
	uint32_t transfer_queue_index = queue_family_indices.transfer_queue_family_index == queue_family_indices.compute_queue_family_index ? compute_queue_index + 1 : 0;

    VkQueue graphics_queue;
    VkQueue compute_queue;
    VkQueue transfer_queue;

	vkGetDeviceQueue (device, queue_family_indices.graphics_queue_family_index, graphics_queue_index, &graphics_queue);
	vkGetDeviceQueue (device, queue_family_indices.compute_queue_family_index, compute_queue_index, &compute_queue);
	vkGetDeviceQueue (device, queue_family_indices.transfer_queue_family_index, transfer_queue_index, &transfer_queue);

    this->graphics_queue = vk_queue (device, graphics_queue);
    this->compute_queue = vk_queue (device, compute_queue);
    this->transfer_queue = vk_queue (device, transfer_queue);
}

vk_swapchain::vk_swapchain (const VkDevice& device, const vk_surface& surface) : device (device)
{
    printf ("vk_swapchain::vk_swapchain\n");

    VkSwapchainCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        nullptr,
        0,
        surface.surface,
        surface.capabilities.minImageCount + 1,
        surface.format.format,
        surface.format.colorSpace,
        surface.capabilities.currentExtent,
        1,
        surface.capabilities.supportedUsageFlags,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        {},
        surface.capabilities.currentTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        surface.present_mode
    };

    VkResult result = vkCreateSwapchainKHR (device, &create_info, nullptr, &swapchain);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SWAPCHAIN;
    }
}

vk_swapchain::vk_swapchain (vk_swapchain&& other) noexcept
{
    printf ("vk_swapchain move ctor\n");

    *this = std::move (other);
}

vk_swapchain& vk_swapchain::operator= (vk_swapchain&& other) noexcept
{
    printf ("vk_swapchain move assignment\n");

    swapchain = other.swapchain;
    device = other.device;

    other.swapchain = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_swapchain::~vk_swapchain () noexcept
{
    printf ("vk_swapchain::~vk_swapchain\n");

    if (swapchain != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR (device, swapchain, nullptr);
    }
}

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

vk_sampler::vk_sampler (const VkDevice& device) : device (device)
{
    printf ("vk_sampler::vk_sampler\n");

    VkSamplerCreateInfo create_info = {
        VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        nullptr,
        0,
        VK_FILTER_LINEAR,
        VK_FILTER_LINEAR,
        VK_SAMPLER_MIPMAP_MODE_LINEAR,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        0,
        VK_FALSE,
        0,
        VK_FALSE,
        VK_COMPARE_OP_NEVER,
        0,
        0,
        VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        VK_FALSE
    };

    VkResult result = vkCreateSampler (device, &create_info, nullptr, &sampler);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SAMPLER;
    }
}

vk_sampler::vk_sampler (vk_sampler&& other) noexcept
{
    printf ("vk_sampler move ctor\n");

    *this = std::move (other);
}

vk_sampler& vk_sampler::operator= (vk_sampler&& other) noexcept
{
    printf ("vk_sampler move assignment\n");

    sampler = other.sampler;
    device = other.device;

    other.sampler = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_sampler::~vk_sampler () noexcept
{
    printf ("vk_sampler::~vk_sampler\n");
    
    if (sampler != VK_NULL_HANDLE)
    {
        vkDestroySampler (device, sampler, nullptr);
    }
}

vk_buffer::vk_buffer (
    const VkDevice& device,
    const VkDeviceSize& size,
    const VkBufferUsageFlags usage_flags,
    const VkSharingMode& sharing_mode,
    const uint32_t& queue_family_index) : device (device)
{
    printf ("vk_buffer::vk_buffer\n");

    VkBufferCreateInfo create_info = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        nullptr,
        0,
        size,
        usage_flags,
        sharing_mode,
        1,
        &queue_family_index
    };

    VkResult result = vkCreateBuffer (device, &create_info, nullptr, &buffer);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_BUFFER;
    }
}

vk_buffer::vk_buffer (vk_buffer&& other) noexcept
{
    printf ("vk_buffer move ctor\n");
    *this = std::move (other);
}

vk_buffer& vk_buffer::operator=(vk_buffer&& other) noexcept
{
    printf ("vk_buffer move assignment\n");

    buffer = other.buffer;
    device = other.device;

    other.buffer = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_buffer::~vk_buffer () noexcept
{
    printf ("vk_buffer::~vk_buffer\n");

    if (buffer != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyBuffer (device, buffer, nullptr);
    }
}

void vk_buffer::bind_memory (const VkDeviceMemory& device_memory, const VkDeviceSize& offset)
{
    VkResult result = vkBindBufferMemory (device, buffer, device_memory, offset);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_BIND_BUFFER_MEMORY;
    }
}

vk_device_memory::vk_device_memory (
    const VkDevice& device, 
    const VkBuffer& buffer, 
    const VkPhysicalDeviceMemoryProperties& memory_properties, 
    const VkMemoryPropertyFlags required_types) : device (device)
{
    printf ("vk_device_memory::vk_device_memory\n");

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements (device, buffer, &memory_requirements);

    uint32_t memory_type_index = 0;

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
        {
            memory_type_index = i;
            break;
        }
    }

    VkMemoryAllocateInfo allocate_info = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memory_requirements.size,
        memory_type_index
    };

    VkResult result = vkAllocateMemory (device, &allocate_info, nullptr, &memory);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_SYSTEM_ALLOCATE_MEMORY;
    }
}

vk_device_memory::vk_device_memory (vk_device_memory&& other) noexcept
{
    printf ("vk_device_memory move ctor\n");

    *this = std::move (other);
}

vk_device_memory& vk_device_memory::operator= (vk_device_memory&& other) noexcept
{
    printf ("vk_device_memory move assignment\n");

    memory = other.memory;
    device = other.device;

    other.memory = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_device_memory::~vk_device_memory () noexcept
{
    printf ("vk_device_memory::~vk_device_memory\n");

    if (memory != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkFreeMemory (device, memory, nullptr);
    }
}

void vk_device_memory::bind_buffer (const VkBuffer& buffer, const VkDeviceSize& offset)
{
    VkResult result = vkBindBufferMemory (device, buffer, memory, offset);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_BIND_BUFFER_MEMORY;
    }
}

HANDLE vk_device_memory::map (const VkDeviceSize& offset, const VkDeviceSize& size)
{
    HANDLE data = nullptr;
    
    VkResult result = vkMapMemory (device, memory, offset, size, 0, &data);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_MAP_MEMORY;
    }

    return data;
}

void vk_device_memory::unmap ()
{
    vkUnmapMemory (device, memory);
}

vk_command_buffers::vk_command_buffers (
    const VkDevice& device, 
    const VkCommandPool& command_pool, 
    const uint32_t& command_buffer_count) : command_pool (command_pool), device (device)
{
    printf ("vk_command_buffers::vk_command_buffers\n");

    VkCommandBufferAllocateInfo allocate_info = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        nullptr,
        command_pool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        command_buffer_count
    };

    command_buffers.resize (command_buffer_count);

    VkResult result = vkAllocateCommandBuffers (device, &allocate_info, command_buffers.data ());
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_COMMAND_BUFFERS;
    }
}

vk_command_buffers::vk_command_buffers (vk_command_buffers&& other) noexcept
{
    printf ("vk_command_buffers move ctor\n");

    *this = std::move (other);
}

vk_command_buffers& vk_command_buffers::operator= (vk_command_buffers&& other) noexcept
{
    printf ("vk_command_buffers move assignment\n");

    command_buffers = std::move (other.command_buffers);
    command_pool = other.command_pool;
    device = other.device;

    other.command_pool = VK_NULL_HANDLE;;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_command_buffers::~vk_command_buffers () noexcept
{
    printf ("vk_command_buffers::~vk_command_buffers\n");

    if (command_buffers.size () > 0 && device != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers (device, command_pool, command_buffers.size (), command_buffers.data ());
    }
}

void vk_command_buffers::begin (const VkCommandBufferUsageFlags& flags)
{
    VkCommandBufferBeginInfo begin_info = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        flags,
        nullptr
    };

    VkResult result = VK_SUCCESS;

    for (auto command_buffer : command_buffers)
    {
        result = vkBeginCommandBuffer (command_buffer, &begin_info);
        if (result != VK_SUCCESS)
        {
            throw AGE_RESULT::ERROR_GRAPHICS_BEGIN_COMMAND_BUFFER;
        }
    }
}

void vk_command_buffers::end ()
{
    VkResult result = VK_SUCCESS;

    for (auto command_buffer : command_buffers)
    {
        result = vkEndCommandBuffer (command_buffer);
        if (result != VK_SUCCESS)
        {
            throw AGE_RESULT::ERROR_GRAPHICS_END_COMMAND_BUFFER;
        }
    }
}

vk_image::vk_image (const VkDevice& device, const image& img) : device (device)
{
    printf ("vk_image::vk_iamge\n");

    VkImageCreateInfo create_info = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        {img.width, img.height, 1},
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr,
        VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkResult result = vkCreateImage (device, &create_info, nullptr, &vk_img_obj);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_IMAGE;
    }
}

vk_image::vk_image (vk_image&& other) noexcept
{
    printf ("vk_image move ctor\n");

    *this = std::move (other);
}

vk_image& vk_image::operator=(vk_image&& other) noexcept
{
    printf ("vk_image move assignment\n");

    vk_img_obj = other.vk_img_obj;
    device = other.device;

    other.vk_img_obj = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_image::~vk_image () noexcept
{
    printf ("vk_image::~vk_image\n");

    if (vk_img_obj != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyImage (device, vk_img_obj, nullptr);
    }
}
