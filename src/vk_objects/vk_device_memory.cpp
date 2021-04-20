#include "vk_device_memory.hpp"

#include "error.hpp"
#include <cstdio>

vk_device_memory::vk_device_memory (
    const VkDevice& device,
    const std::vector<VkBuffer>& vk_type_objects,
    const VkPhysicalDeviceMemoryProperties& memory_properties,
    const VkMemoryPropertyFlags required_types) : device (device)
{
    printf ("vk_device_memory::vk_device_memory vk_types\n");

    allocate_bind_buffers (
        device,
        vk_type_objects,
        memory_properties,
        required_types
    );
}


vk_device_memory::vk_device_memory (
    const VkDevice& device,
    const std::vector<VkImage>& vk_type_objects,
    const VkPhysicalDeviceMemoryProperties& memory_properties,
    const VkMemoryPropertyFlags required_types) : device (device)
{
    printf ("vk_device_memory::vk_device_memory vk_types\n");

    allocate_bind_images (
        device,
        vk_type_objects,
        memory_properties,
        required_types
    );
}

vk_device_memory::vk_device_memory (vk_device_memory&& other) noexcept
{
    printf ("vk_device_memory move ctor\n");

    *this = std::move (other);
}

vk_device_memory& vk_device_memory::operator= (vk_device_memory&& other) noexcept
{
    printf ("vk_device_memory move assignment\n");

    memory = other.memory;
    device = other.device;

    other.memory = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_device_memory::~vk_device_memory () noexcept
{
    printf ("vk_device_memory::~vk_device_memory\n");

    if (memory != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkFreeMemory (device, memory, nullptr);
    }
}

HANDLE vk_device_memory::map (const VkDeviceSize& offset, const VkDeviceSize& size) const
{
    HANDLE data = nullptr;
    
    VkResult result = vkMapMemory (device, memory, offset, size, 0, &data);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_MAP_MEMORY;
    }

    return data;
}

void vk_device_memory::unmap () const
{
    vkUnmapMemory (device, memory);
}

void vk_device_memory::allocate_bind_buffers (
    const VkDevice& device, 
    const std::vector<VkBuffer>& buffers, 
    const VkPhysicalDeviceMemoryProperties& memory_properties, 
    const VkMemoryPropertyFlags required_types)
{
    VkDeviceSize total_data_size = 0;
    VkMemoryRequirements memory_requirements;

    std::vector<VkDeviceSize> buffer_data_offsets;
    buffer_data_offsets.reserve (buffers.size ());

    for (const auto& b : buffers)
    {
        buffer_data_offsets.emplace_back (total_data_size);
        vkGetBufferMemoryRequirements (device, b, &memory_requirements);
        total_data_size += memory_requirements.size;
    }

    memory_requirements.size = total_data_size;
    uint32_t required_memory_type_index = 0;

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
        {
            required_memory_type_index = i;
            break;
        }
    }

    VkMemoryAllocateInfo allocate_info = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memory_requirements.size,
        required_memory_type_index
    };

    VkResult result = vkAllocateMemory (device, &allocate_info, nullptr, &memory);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_SYSTEM_ALLOCATE_MEMORY;
    }

    uint32_t index_counter = 0;

    for (const auto& b : buffers)
    {
        result = vkBindBufferMemory (device, b, memory, buffer_data_offsets[index_counter]);
        if (result != VK_SUCCESS)
        {
            throw AGE_RESULT::ERROR_GRAPHICS_BIND_BUFFER_MEMORY;
        }

        ++index_counter;
    }
}

void vk_device_memory::allocate_bind_images (
    const VkDevice& device, 
    const std::vector<VkImage>& images, 
    const VkPhysicalDeviceMemoryProperties& memory_properties, 
    const VkMemoryPropertyFlags required_types)
{
    VkDeviceSize total_data_size = 0;
    VkMemoryRequirements memory_requirements;

    std::vector<VkDeviceSize> image_data_offsets;
    image_data_offsets.reserve (images.size ());

    for (const auto& i : images)
    {
        image_data_offsets.emplace_back (total_data_size);
        vkGetImageMemoryRequirements (device, i, &memory_requirements);
        total_data_size += memory_requirements.size;
    }

    memory_requirements.size = total_data_size;
    uint32_t required_memory_type_index = 0;

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
        {
            required_memory_type_index = i;
            break;
        }
    }

    VkMemoryAllocateInfo memory_allocate_info = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memory_requirements.size,
        required_memory_type_index
    };

    VkResult result = vkAllocateMemory (device, &memory_allocate_info, nullptr, &memory);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_MEMORY;
    }

    uint32_t index_counter = 0;

    for (const auto& i : images)
    {
        result = vkBindImageMemory (device, i, memory, image_data_offsets[index_counter]);
        if (result != VK_SUCCESS)
        {
            throw AGE_RESULT::ERROR_GRAPHICS_BIND_IMAGE_MEMORY;
        }

        ++index_counter;
    }
}