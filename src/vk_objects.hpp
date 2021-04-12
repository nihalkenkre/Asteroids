#ifndef VK_OBJECTS_HPP
#define VK_OBJECTS_HPP

#include <vulkan/vulkan.h>

#include <Windows.h>
#include <memory>
#include <vector>

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

class vk_physical_device
{
public:
    vk_physical_device () {}
    vk_physical_device(const VkInstance& instance);

    VkPhysicalDevice physical_device;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceLimits limits;
};

class vk_queue_family_indices
{
public:
    vk_queue_family_indices () {}
    vk_queue_family_indices (const VkPhysicalDevice& physical_device);

    uint32_t graphics_queue_family_index;
    uint32_t compute_queue_family_index;
    uint32_t transfer_queue_family_index;
};

class vk_surface
{
public:
    vk_surface () {}
    vk_surface (const VkInstance& instance, 
                const VkPhysicalDevice& physical_device,
                const HINSTANCE& h_instance,
                const HWND& h_wnd,
                const uint32_t& graphics_queue_family_index);
    
    vk_surface (const vk_surface& other) = delete;
    vk_surface& operator= (const vk_surface& other) = delete;

    vk_surface (vk_surface&& other) noexcept;
    vk_surface& operator= (vk_surface&& other) noexcept;

    ~vk_surface () noexcept;

    VkSurfaceKHR surface;
    VkPresentModeKHR present_mode;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR format;
    VkExtent2D extent;

private:
    VkInstance instance;
};

class vk_queue_info
{
public:
    vk_queue_info () {}
    vk_queue_info (const vk_queue_family_indices* queue_family_indices);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::vector<uint32_t> queue_indices;
};

class vk_graphics_device
{
public:
    vk_graphics_device () {}
    vk_graphics_device (const VkPhysicalDevice& physical_device, 
                        const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos);

    vk_graphics_device (const vk_graphics_device& other) = delete;
    vk_graphics_device& operator= (const vk_graphics_device& other) = delete;

    vk_graphics_device (vk_graphics_device&& other) noexcept;
    vk_graphics_device& operator= (vk_graphics_device&& other) noexcept;

    ~vk_graphics_device () noexcept;

    VkDevice graphics_device;
};

class vk_queue
{
public:
    vk_queue () {}
    vk_queue (const VkQueue& queue, const VkDevice& device);
    
    VkQueue queue;

private:
    VkDevice device;
};

class vk_device_queues
{
public:
    vk_device_queues () {}
    vk_device_queues (const VkPhysicalDevice& physical_device, 
                      const VkDevice& device, 
                      const vk_queue_family_indices* queue_family_indices, 
                      const std::vector<uint32_t>& queue_indices);

    std::unique_ptr<vk_queue> graphics_queue;
    std::unique_ptr<vk_queue> compute_queue;
    std::unique_ptr<vk_queue> transfer_queue;                      
};

class vk_image_view
{
public:
    vk_image_view () {}
};

class vk_swapchain
{
public:
    vk_swapchain () {}
    vk_swapchain (const VkDevice& device, const vk_surface* surface);
    
    vk_swapchain (const vk_swapchain& other) = delete;
    vk_swapchain& operator= (const vk_swapchain& other) = delete;

    vk_swapchain (vk_swapchain&& other) noexcept;
    vk_swapchain& operator= (vk_swapchain&& other) noexcept;

    ~vk_swapchain () noexcept;

    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<vk_image_view> image_views;

private:
    VkDevice device;
};


class vk_command_pool
{
public:
    vk_command_pool () {}
    vk_command_pool (const VkDevice& device, 
                        const uint32_t& queue_family_index,
                        const VkCommandPoolCreateFlags& flags
                    );

    vk_command_pool (const vk_command_pool& other) = delete;
    vk_command_pool& operator= (const vk_command_pool& other) = delete;

    vk_command_pool (vk_command_pool&& other) noexcept;
    vk_command_pool& operator= (vk_command_pool&& other) noexcept;

    ~vk_command_pool () noexcept;

    VkCommandPool command_pool;

private:
    VkDevice device;
};

#endif 
