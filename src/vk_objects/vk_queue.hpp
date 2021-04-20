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

    VkQueue queue;

private:
    VkDevice device;
};


#endif