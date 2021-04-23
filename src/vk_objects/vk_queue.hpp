#ifndef VK_QUEUE_HPP
#define VK_QUEUE_HPP

#include <vulkan/vulkan.h>
#include <vector>


class vk_queue
{
public:
    vk_queue () : queue (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_queue (const VkDevice& device, const VkQueue& queue);
    
    void submit (const std::vector<VkCommandBuffer>& commands_buffers) const;

    void submit (
        const std::vector<VkSemaphore>& wait_semaphores,
        const VkPipelineStageFlags& wait_dst_stage_mask,
        const std::vector<VkCommandBuffer>& commands_buffers, 
        const std::vector<VkSemaphore>& signal_semaphores) const;

    VkQueue queue;

private:
    VkDevice device;
};


#endif