#include "vk_descriptor_pool.hpp"

#include "error.hpp"


vk_descriptor_pool::vk_descriptor_pool (
    const VkDevice& device,
    const std::vector<VkDescriptorPoolSize>& pool_sizes,
    const uint32_t& max_sets) : device (device)
{
    printf ("vk_descriptor_pool::vk_descriptor_pool\n");

    VkDescriptorPoolCreateInfo create_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        nullptr,
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        max_sets,
        static_cast<uint32_t> (pool_sizes.size ()),
        pool_sizes.data ()
    };

    VkResult result = vkCreateDescriptorPool (device, &create_info, nullptr, &descriptor_pool);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_DESCRIPTOR_POOL;
    }
}

vk_descriptor_pool::vk_descriptor_pool (vk_descriptor_pool&& other) noexcept
{
    printf ("vk_descriptor_pool move ctor\n");

    *this = std::move (other);
}

vk_descriptor_pool& vk_descriptor_pool::operator=(vk_descriptor_pool&& other) noexcept
{
    printf ("vk_descriptor_pool move assignment\n");

    descriptor_pool = other.descriptor_pool;
    device = other.device;

    other.descriptor_pool = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_descriptor_pool::~vk_descriptor_pool () noexcept
{
    printf ("vk_descriptor_pool::~vk_descriptor_pool\n");

    if (descriptor_pool != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool (device, descriptor_pool, nullptr);
    }
}
