#include "scene_graphics.hpp"
#include "utils.hpp"

#include <cstdio>


scene_graphics::scene_graphics (common_graphics* common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    create_geometry_buffers (common_graphics_obj);
    create_image_buffers (common_graphics_obj);
}

void scene_graphics::create_geometry_buffers (common_graphics* common_graphics_obj)
{
    VkDeviceSize data_size = scene_mesh.positions_size + scene_mesh.uvs_size + scene_mesh.indices_size;

    vk_buffer staging_buffer (
        common_graphics_obj->graphics_device.graphics_device,
        data_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        common_graphics_obj->queue_family_indices.graphics_queue_family_index
    );

    vk_device_memory staging_buffer_memory (
        common_graphics_obj->graphics_device.graphics_device, staging_buffer.buffer,
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    staging_buffer.bind_memory (staging_buffer_memory.memory, 0);
    HANDLE mapped_data_ptr = staging_buffer_memory.map (0, data_size);

    std::memcpy (mapped_data_ptr, scene_mesh.positions.data (), (size_t)scene_mesh.positions_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size, scene_mesh.uvs.data (), (size_t)scene_mesh.uvs_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size + scene_mesh.uvs_size, scene_mesh.indices.data (), (size_t)scene_mesh.indices_size);

    vertex_index_buffer = vk_buffer (common_graphics_obj->graphics_device.graphics_device,
        data_size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        common_graphics_obj->queue_family_indices.graphics_queue_family_index
    );

    vertex_index_device_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        vertex_index_buffer.buffer,
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vertex_index_device_memory.bind_buffer (vertex_index_buffer.buffer, 0);

    vk_command_buffers copy_cmd_buffers = vk_command_buffers (
        common_graphics_obj->graphics_device.graphics_device,
        common_graphics_obj->transfer_command_pool.command_pool,
        1
    );

    copy_cmd_buffers.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VkBufferCopy buffer_copy = {
        0, 0, data_size
    };

    vkCmdCopyBuffer (copy_cmd_buffers.command_buffers[0], staging_buffer.buffer, vertex_index_buffer.buffer, 1, &buffer_copy);
    copy_cmd_buffers.end ();

    common_graphics_obj->device_queues.graphics_queue.submit (copy_cmd_buffers.command_buffers);
    vkQueueWaitIdle (common_graphics_obj->device_queues.graphics_queue.queue);

    staging_buffer_memory.unmap ();
}

void scene_graphics::create_image_buffers (common_graphics* common_graphics_obj)
{
    std::vector<image> images;
    images.reserve (4);
 
    images.emplace_back (image ("background.png"));
    images.emplace_back (image ("player.png"));
    images.emplace_back (image ("asteroid.png"));
    images.emplace_back (image ("bullet.png"));

    uint32_t total_size = 0;
    for (const auto& i : images)
    {
        total_size += i.pixels.size ();
    }

    vk_buffer staging_buffer = vk_buffer (
        common_graphics_obj->graphics_device.graphics_device,
        total_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        common_graphics_obj->queue_family_indices.graphics_queue_family_index
    );

    vk_device_memory staging_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        staging_buffer.buffer,
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    staging_memory.bind_buffer (staging_buffer.buffer, 0);
    HANDLE mapped_data_ptr = staging_memory.map (0, total_size);

    uint32_t current_offset = 0;

    for (const auto& i : images)
    {
        std::memcpy ((char*)mapped_data_ptr + current_offset, i.pixels.data (), i.pixels.size ());
        current_offset += i.pixels.size ();
    }

    std::vector<vk_image> vk_images;
    vk_images.reserve (images.size ());

    for (const auto& i : images)
    {
        vk_images.emplace_back (vk_image (common_graphics_obj->graphics_device.graphics_device, i));
    }

    staging_memory.unmap ();
}
