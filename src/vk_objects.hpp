#ifndef VK_OBJECTS_HPP
#define VK_OBJECTS_HPP

#include <vulkan/vulkan.h>

#include <Windows.h>
#include <memory>
#include <vector>
#include <string>

#include "utils.hpp"

enum class vk_type
{
    buffer,
    image,
};

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

    std::tuple<uint32_t, uint32_t, uint32_t> get_queue_family_indices () const;

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
    vk_surface () : surface (VK_NULL_HANDLE), instance (VK_NULL_HANDLE) {}
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
    vk_graphics_device () : graphics_device (VK_NULL_HANDLE) {}
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
    vk_queue () : queue (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
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


class vk_swapchain
{
public:
    vk_swapchain () : swapchain (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
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
    vk_command_pool () : command_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
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
    vk_sampler () : sampler (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
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
    vk_buffer () : buffer (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_buffer (
        const VkDevice& device,
        const VkDeviceSize& size,
        const VkBufferUsageFlags usage_flags,
        const VkSharingMode& sharing_mode,
        const uint32_t& queue_family_index);

    vk_buffer (
        const VkDevice& device,
        const buffer_data& buff
    );

    vk_buffer (const vk_buffer& other) = delete;
    vk_buffer& operator= (const vk_buffer& other) = delete;

    vk_buffer (vk_buffer&& other) noexcept;
    vk_buffer& operator= (vk_buffer&& other) noexcept;

    ~vk_buffer () noexcept;

    void copy_to_buffer (
        const VkBuffer& dst_buffer,
        const VkDeviceSize& size,
        const VkCommandPool& command_pool,
        const VkQueue& transfer_queue
    ) const;

    void copy_to_images (
        const std::vector<VkImage>& images,
        const std::vector<VkExtent3D>& extents,
        const std::vector<VkDeviceSize>& offsets,
        const VkCommandPool& command_pool, 
        const VkQueue& transfer_queue
    ) const;

    VkBuffer buffer;

private:
    VkDevice device;
};


class vk_device_memory
{
public:
    vk_device_memory () : memory (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}

    vk_device_memory (
        const VkDevice& device,
        const std::vector<uint64_t>& vk_type_objects,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types,
        const vk_type& type
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

    void allocate_bind_buffers (const VkDevice& device,
        const std::vector<VkBuffer>& buffers,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types);

    void allocate_bind_images (const VkDevice& device,
        const std::vector<VkImage>& images,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkMemoryPropertyFlags required_types);
};

class vk_command_buffer
{
public:
    vk_command_buffer () : command_buffer (VK_NULL_HANDLE), command_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_command_buffer (
        const VkDevice& device,
        const VkCommandPool& command_pool
    );

    vk_command_buffer (const vk_command_buffer& other) = delete;
    vk_command_buffer& operator= (const vk_command_buffer& other) = delete;

    vk_command_buffer (vk_command_buffer&& other) noexcept;
    vk_command_buffer& operator= (vk_command_buffer&& other) noexcept;

    ~vk_command_buffer () noexcept;

    VkCommandBuffer command_buffer;

    void begin (const VkCommandBufferUsageFlags& flags) const;
    void end () const;

private:
    VkCommandPool command_pool;
    VkDevice device;
};


class vk_command_buffers
{
public:
    vk_command_buffers () : command_pool (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
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
    vk_image () : vk_img (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_image (const VkDevice& device, const image_data& img);

    vk_image (const vk_image& other) = delete;
    vk_image& operator= (const vk_image& other) = delete;

    vk_image (vk_image&& other) noexcept;
    vk_image& operator= (vk_image&& other) noexcept;

    ~vk_image () noexcept;

    VkImage vk_img;

    void change_layout (
        const VkAccessFlags& src_access,
        const VkAccessFlags& dst_access,
        const VkImageLayout& src_layout,
        const VkImageLayout& dst_layout,
        const uint32_t& src_queue_family_index,
        const uint32_t& dst_queue_family_index,
        const VkPipelineStageFlags& src_pipeline_stage,
        const VkPipelineStageFlags& dst_pipeline_stage, 
        const VkCommandPool& command_pool,
        const VkQueue& queue
    ) const;

private:
    VkDevice device;
};


class vk_image_view
{
public:
    vk_image_view () {}
};

#endif 

