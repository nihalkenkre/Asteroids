#include "vk_command_buffers.hpp"

#include "error.hpp"

vk_command_buffer::vk_command_buffer (
    const VkDevice& device,
    const VkCommandPool& command_pool) : command_pool (command_pool), device (device)
{
    printf ("vk_command_buffer::vk_command_buffer\n");

    VkCommandBufferAllocateInfo allocate_info = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        nullptr,
        command_pool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        1
    };

    VkResult result = vkAllocateCommandBuffers (device, &allocate_info, &command_buffer);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_COMMAND_BUFFERS;
    }
}

vk_command_buffer::vk_command_buffer (vk_command_buffer&& other) noexcept
{
    printf ("vk_command_buffer move ctor\n");

    *this = std::move (other);
}

vk_command_buffer& vk_command_buffer::operator= (vk_command_buffer&& other) noexcept
{
    printf ("vk_command_buffer move assignment\n");

    command_buffer = other.command_buffer;
    command_pool = other.command_pool;
    device = other.device;

    other.command_buffer = VK_NULL_HANDLE;
    other.command_pool = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_command_buffer::~vk_command_buffer () noexcept
{
    printf ("vk_command_buffer::~vk_command_buffer\n");

    if (command_buffer != VK_NULL_HANDLE &&
        command_pool != VK_NULL_HANDLE &&
        device != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers (device, command_pool, 1, &command_buffer);
    }
}

void vk_command_buffer::reset (const VkCommandBufferResetFlags& flags) const
{
    VkResult result = vkResetCommandBuffer (command_buffer, flags);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_RESET_COMMAND_BUFFER;
    }
}

void vk_command_buffer::begin (const VkCommandBufferUsageFlags& flags) const
{
    VkCommandBufferBeginInfo begin_info = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        flags,
        nullptr
    };

    VkResult result = vkBeginCommandBuffer (command_buffer, &begin_info);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_BEGIN_COMMAND_BUFFER;
    }
}

void vk_command_buffer::begin_render_pass (
    const VkRenderPassBeginInfo& begin_info, 
    const VkSubpassContents& subpass_contents) const
{
    vkCmdBeginRenderPass (command_buffer, &begin_info, subpass_contents);
}

void vk_command_buffer::end_render_pass () const
{
    vkCmdEndRenderPass (command_buffer);
}

void vk_command_buffer::end () const
{
    VkResult result = vkEndCommandBuffer (command_buffer);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_END_COMMAND_BUFFER;
    }
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

    command_buffers.reserve (command_buffer_count);

    for (uint32_t c = 0; c < command_buffer_count; ++c)
    {
        command_buffers.emplace_back (vk_command_buffer (device, command_pool));
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

    command_buffers.clear ();
}

void vk_command_buffers::reset (const VkCommandBufferResetFlags& flags) const
{
    for (const auto& command_buffer : command_buffers)
    {
        command_buffer.reset (flags);
    }
}

void vk_command_buffers::begin (const VkCommandBufferUsageFlags& flags) const
{
    for (const auto& command_buffer : command_buffers)
    {
        command_buffer.begin (flags);
    }
}

void vk_command_buffers::end () const
{

    for (const auto& command_buffer : command_buffers)
    {
        command_buffer.end ();
    }
}
