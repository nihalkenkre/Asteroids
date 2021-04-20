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
        commands_buffers.size (),
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
