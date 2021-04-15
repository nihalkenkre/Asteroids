#ifndef VK_OBJECTS_HPP
#define VK_OBJECTS_HPP

#include <vulkan/vulkan.h>

#include <Windows.h>
#include <memory>
#include <vector>
#include <string>

#include "utils.hpp"

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
    vk_physical_device (const VkInstance& instance);

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

private:
    VkInstance instance;
};


class vk_queue_info
{
public:
    vk_queue_info () {}
    vk_queue_info (const vk_queue_family_indices& queue_family_indices);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::vector<uint32_t> queue_indices;
};

class vk_buffer;

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

    VkBuffer create_buffer (
        const VkDeviceSize& size,
        const VkBufferUsageFlags usage_flags,
        const uint32_t& queue_family_index) const;

    void destroy_buffer (const vk_buffer& buffer) const;

    VkImage create_image (const VkExtent3D& extent) const;
    void destroy_image (const VkImage& image) const;

    VkDevice graphics_device;
};


class vk_queue
{
public:
    vk_queue () {}
    vk_queue (const VkDevice& device, const VkQueue& queue);
    
    void submit (const std::vector<VkCommandBuffer>& commands_buffers) const;

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
                      const vk_queue_family_indices& queue_family_indices, 
                      const std::vector<uint32_t>& queue_indices);

    vk_queue graphics_queue;
    vk_queue compute_queue;
    vk_queue transfer_queue;
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
    vk_swapchain (const VkDevice& device, const vk_surface& surface);
    
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


class vk_sampler
{
public:
    vk_sampler () {}
    vk_sampler (const VkDevice& device);

    vk_sampler (const vk_sampler& other) = delete;
    vk_sampler& operator= (const vk_sampler& other) = delete;

    vk_sampler (vk_sampler&& other) noexcept;
    vk_sampler& operator= (vk_sampler&& other) noexcept;

    ~vk_sampler () noexcept;

    VkSampler sampler;

private:
    VkDevice device;
};


class vk_buffer
{
public:
    vk_buffer () {}
    vk_buffer (const VkBuffer& buffer) : buffer (buffer) {}

    VkBuffer buffer;

    void copy_to_buffer (
        const VkDevice& device,
        const VkBuffer& dst_buffer,
        const VkDeviceSize& size,
        const VkCommandPool& command_pool,
        const VkQueue& transfer_queue) const;
};


class vk_buffer_unique
{
public:
    vk_buffer_unique () {}
    vk_buffer_unique (
        const VkDevice& device,
        const VkDeviceSize& size,
        const VkBufferUsageFlags usage_flags,
        const VkSharingMode& sharing_mode,
        const uint32_t& queue_family_index);

    vk_buffer_unique (const vk_buffer_unique& other) = delete;
    vk_buffer_unique& operator= (const vk_buffer_unique& other) = delete;

    vk_buffer_unique (vk_buffer_unique&& other) noexcept;
    vk_buffer_unique& operator= (vk_buffer_unique&& other) noexcept;

    ~vk_buffer_unique () noexcept;

    void copy_from_buffer (
        const VkBuffer& src_buffer,
        const VkDeviceSize& size,
        const VkCommandPool& command_pool,
        const VkQueue& transfer_queue
    ) const;

    void copy_to () const {}

    VkBuffer buffer;

private:
    VkDevice device;
};

class vk_image_unique;
class vk_image;

class vk_device_memory
{
public:
    vk_device_memory () {}
    vk_device_memory (
        const VkDevice& device, 
        const std::vector<vk_buffer_unique>& vk_buffers,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );

    vk_device_memory (
        const VkDevice& device,
        const std::vector<vk_image_unique>& vk_images,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );

    vk_device_memory (
        const VkDevice& device,
        const std::vector<vk_buffer>& vk_buffers,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );

    vk_device_memory (
        const VkDevice& device,
        const std::vector<vk_image>& vk_images,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types
    );


    vk_device_memory (const vk_device_memory& other) = delete;
    vk_device_memory& operator= (const vk_device_memory& other) = delete;

    vk_device_memory (vk_device_memory&& other) noexcept;
    vk_device_memory& operator= (vk_device_memory&& other) noexcept;

    ~vk_device_memory () noexcept;

    HANDLE map (const VkDeviceSize& offset, const VkDeviceSize& size) const;
    void unmap () const;

    VkDeviceMemory memory;

private:
    VkDevice device;
};


class vk_command_buffers
{
public:
    vk_command_buffers () {}
    vk_command_buffers (
        const VkDevice& device,
        const VkCommandPool& command_pool,
        const uint32_t& command_buffer_count
    );

    vk_command_buffers (const vk_command_buffers& other) = delete;
    vk_command_buffers& operator= (const vk_command_buffers& other) = delete;

    vk_command_buffers (vk_command_buffers&& other) noexcept;
    vk_command_buffers& operator= (vk_command_buffers&& other) noexcept;

    ~vk_command_buffers () noexcept;

    void begin (const VkCommandBufferUsageFlags& flags) const;
    void end () const;

    std::vector<VkCommandBuffer> command_buffers;

private:
    VkCommandPool command_pool;
    VkDevice device;    
};


class vk_image
{
public:
    vk_image () {}
    vk_image (const VkImage& image) : image (image) {}

    VkImage image;
};

class vk_image_unique
{
public:
    vk_image_unique () {}
    vk_image_unique (const VkDevice& device, const image& img);

    vk_image_unique (const vk_image_unique& other) = delete;
    vk_image_unique& operator= (const vk_image_unique& other) = delete;

    vk_image_unique (vk_image_unique&& other) noexcept;
    vk_image_unique& operator= (vk_image_unique&& other) noexcept;

    ~vk_image_unique () noexcept;

    VkImage vk_img;

private:
    VkDevice device;
};


#endif 
