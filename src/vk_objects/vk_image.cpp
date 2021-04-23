#include "vk_image.hpp"

#include "vk_objects/vk_queue.hpp"
#include "vk_objects/vk_command_buffers.hpp"
#include "vk_objects/vk_semaphore.hpp"

#include "utils.hpp"

#include "error.hpp"

vk_image::vk_image (const VkDevice& device, const image_data& img) : device (device)
{
    printf ("vk_image::vk_image\n");

    VkImageCreateInfo create_info = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        { img.width, img.height, 1},
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

    VkResult result = vkCreateImage (device, &create_info, nullptr, &vk_img);
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

    vk_img = other.vk_img;
    device = other.device;

    other.vk_img = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_image::~vk_image () noexcept
{
    if (vk_img != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyImage (device, vk_img, nullptr);
    }
}

void vk_image::change_layout (
    const VkAccessFlags& src_access, 
    const VkAccessFlags& dst_access, 
    const VkImageLayout& src_layout, 
    const VkImageLayout& dst_layout, 
    const uint32_t& src_queue_family_index, 
    const uint32_t& dst_queue_family_index, 
    const VkPipelineStageFlags& src_pipeline_stage, 
    const VkPipelineStageFlags& dst_pipeline_stage, 
    const VkCommandPool& command_pool, 
    const VkQueue& queue) const
{
    VkImageSubresourceRange subresource_range = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        1,
        0,
        1
    };

    VkImageMemoryBarrier image_memory_barrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        src_access,
        dst_access,
        src_layout,
        dst_layout,
        src_queue_family_index,
        dst_queue_family_index,
        vk_img,
        subresource_range
    };

    vk_command_buffer cmd_buffer (device, command_pool);
    cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdPipelineBarrier (
        cmd_buffer.command_buffer,
        src_pipeline_stage,
        dst_pipeline_stage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &image_memory_barrier
    );

    cmd_buffer.end ();

    vk_queue submit_queue (device, queue);
    submit_queue.submit (std::vector<VkCommandBuffer>{ cmd_buffer.command_buffer });

    vkQueueWaitIdle (queue);
}

void vk_image::change_layout (
    const VkAccessFlags& src_access,
    const VkAccessFlags& dst_access,
    const VkImageLayout& src_layout,
    const VkImageLayout& dst_layout,
    const uint32_t& src_queue_family_index,
    const uint32_t& dst_queue_family_index,
    const VkPipelineStageFlags& src_pipeline_stage, 
    const VkPipelineStageFlags& dst_pipeline_stage,
    const VkCommandPool& src_command_pool,
    const VkQueue& src_queue,
    const VkCommandPool& dst_command_pool,
    const VkQueue& dst_queue) const
{
    VkImageSubresourceRange subresource_range = {
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		1,
		0,
		1
	};

	VkImageMemoryBarrier image_memory_barrier = {
		VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		nullptr,
		src_access,
		dst_access,
		src_layout,
		dst_layout,
		src_queue_family_index,
		dst_queue_family_index,
		vk_img,
		subresource_range
	};

    vk_command_buffer src_cmd_buffer (device, src_command_pool);
    src_cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdPipelineBarrier (
        src_cmd_buffer.command_buffer,
        src_pipeline_stage,
        dst_pipeline_stage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &image_memory_barrier
    );

    src_cmd_buffer.end ();

    vk_command_buffer dst_cmd_buffer (device, dst_command_pool);
    dst_cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdPipelineBarrier (
        dst_cmd_buffer.command_buffer,
        src_pipeline_stage,
        dst_pipeline_stage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &image_memory_barrier
    );

    dst_cmd_buffer.end ();

    vk_semaphore sem (device);

    vk_queue src_submit_queue (device, src_queue);
    src_submit_queue.submit ({}, 0, std::vector<VkCommandBuffer>{ src_cmd_buffer.command_buffer }, { sem.semaphore });
    
    vk_queue dst_submit_queue (device, dst_queue);
    dst_submit_queue.submit ({ sem.semaphore }, dst_pipeline_stage, std::vector<VkCommandBuffer>{ dst_cmd_buffer.command_buffer }, {});

    vkQueueWaitIdle (src_queue);
    vkQueueWaitIdle (dst_queue);
}