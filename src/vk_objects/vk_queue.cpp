#include "vk_queue.hpp"

#include "error.hpp"

vk_queue::vk_queue (const VkDevice& device, const VkQueue& queue) : device (device), queue (queue)
{
    printf ("vk_queue::vk_queue\n");
}

void vk_queue::submit (const std::vector<VkCommandBuffer>& commands_buffers) const
{
    VkSubmitInfo submit_info = {
    VK_STRUCTURE_TYPE_SUBMIT_INFO,
    nullptr,
    0,
    nullptr,
    0,
    static_cast<uint32_t> (commands_buffers.size ()),
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

void vk_queue::submit (
    const std::vector<VkSemaphore>& wait_semaphores, 
    const VkPipelineStageFlags& wait_dst_stage_mask, 
    const std::vector<VkCommandBuffer>& commands_buffers, 
    const std::vector<VkSemaphore>& signal_semaphores,
    const VkFence& fence) const
{
    VkSubmitInfo submit_info = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr,
        static_cast<uint32_t> (wait_semaphores.size ()),
        wait_semaphores.data (),
        &wait_dst_stage_mask,
        static_cast<uint32_t> (commands_buffers.size ()),
        commands_buffers.data (),
        static_cast<uint32_t> (signal_semaphores.size ()),
        signal_semaphores.data ()
    };
    
    VkResult vk_result = vkQueueSubmit (queue, 1, &submit_info, fence);
    if (vk_result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_QUEUE_SUBMIT;
    }
}

void vk_queue::present (
    const std::vector<VkSemaphore>& signal_semaphores, 
    const std::vector<VkSwapchainKHR>& swapchains, 
    const std::vector<uint32_t>& image_indices,
    std::vector<VkResult>& results) const
{
    VkPresentInfoKHR present_info{
        VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        nullptr,
        static_cast<uint32_t> (signal_semaphores.size ()),
        signal_semaphores.data (),
        static_cast<uint32_t> (swapchains.size ()),
        swapchains.data (),
        image_indices.data (),
        results.data ()
    };

    VkResult result = vkQueuePresentKHR (queue, &present_info);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_QUEUE_PRESENT;
    }
}
