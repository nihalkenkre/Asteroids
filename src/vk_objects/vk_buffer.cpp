#include "vk_buffer.hpp"
#include "vk_queue.hpp"
#include "error.hpp"
#include "utils.hpp"
#include "vk_command_buffers.hpp"

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

vk_buffer::vk_buffer (const VkDevice& device, const buffer_data& buff) : device (device)
{
    VkBufferCreateInfo create_info = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        nullptr,
        0,
        buff.size,
        buff.usage_flags,
        buff.sharing_mode,
        1,
        &buff.queue_family_index
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

void vk_buffer::copy_to_buffer (
    const VkBuffer& dst_buffer,
    const VkDeviceSize& size, 
    const VkCommandPool& command_pool, 
    const VkQueue& transfer_queue) const
{
    vk_command_buffer copy_cmd_buffer (device, command_pool);

    copy_cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VkBufferCopy buffer_copy = { 0, 0, size };

    vkCmdCopyBuffer (copy_cmd_buffer.command_buffer, buffer, dst_buffer, 1, &buffer_copy);
    copy_cmd_buffer.end ();

    vk_queue queue (device, transfer_queue);
    queue.submit (std::vector<VkCommandBuffer> {copy_cmd_buffer.command_buffer});
    vkQueueWaitIdle (queue.queue);
}

void vk_buffer::copy_to_images (
    const std::vector<VkImage>& images,
    const std::vector<VkExtent3D>& extents,
    const std::vector<VkDeviceSize>& offsets,
    const VkCommandPool& command_pool, 
    const VkQueue& transfer_queue) const
{
    VkOffset3D img_offset = { 0,0,0 };

	VkImageSubresourceLayers subresource_layers = {
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		0,
		1
	};

	VkBufferImageCopy buffer_image_copy = {
		0,
		0,
		0,
		subresource_layers,
		img_offset,
		{0,0,0}
	};

    vk_command_buffer copy_cmd_buffer (device, command_pool);
    copy_cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    uint32_t current_index = 0;

    for (const auto& i : images)
    {
        buffer_image_copy.bufferOffset = offsets[current_index];
        buffer_image_copy.imageExtent = extents[current_index];

        vkCmdCopyBufferToImage (
            copy_cmd_buffer.command_buffer,
            buffer,
            i,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &buffer_image_copy
        );

        ++current_index;
    }

    copy_cmd_buffer.end ();

    vk_queue one_time_submit (device, transfer_queue);
    one_time_submit.submit ({ copy_cmd_buffer.command_buffer });
}

