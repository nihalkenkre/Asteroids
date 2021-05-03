#ifndef VK_COMMAND_BUFFERS_HPP
#define VK_COMMAND_BUFFERS_HPP

#include <vulkan/vulkan.h>
#include <vector>

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

    void reset (const VkCommandBufferResetFlags& flags) const;
    void begin (const VkCommandBufferUsageFlags& flags) const;

    void begin_render_pass (
        const VkRenderPassBeginInfo& begin_info, 
        const VkSubpassContents& subpass_contents
    ) const;

    void end_render_pass () const;

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

    void reset (const VkCommandBufferResetFlags& flags) const;
    void begin (const VkCommandBufferUsageFlags& flags) const;
    void end () const;

    std::vector<vk_command_buffer> command_buffers;

private:
    VkCommandPool command_pool;
    VkDevice device;
};

#endif