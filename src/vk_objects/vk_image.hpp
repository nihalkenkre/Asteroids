#ifndef VK_IMAGE_HPP
#define VK_IMAGE_HPP

#include <vulkan/vulkan.h>
#include "utils.hpp"

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

    void change_layout (
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
        const VkQueue& dst_queue
    ) const;

private:
    VkDevice device;
};

#endif