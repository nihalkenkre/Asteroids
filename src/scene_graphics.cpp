#include "scene_graphics.hpp"

#include <cstdio>

scene_graphics::scene_graphics (common_graphics* common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    VkDeviceSize data_size = mesh.positions_size + mesh.uvs_size + mesh.indices_size;

    vk_buffer staging_vertex_index_buffer (
        common_graphics_obj->graphics_device.graphics_device,
        data_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        common_graphics_obj->queue_family_indices.graphics_queue_family_index
    );

    vk_device_memory staging_vertex_index_buffer_memory (
        common_graphics_obj->graphics_device.graphics_device, staging_vertex_index_buffer.buffer,
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    staging_vertex_index_buffer.bind_memory (staging_vertex_index_buffer_memory.memory, 0);
    HANDLE data_ptr = staging_vertex_index_buffer_memory.map (0, data_size);

    std::memcpy (data_ptr, mesh.positions.data (), (size_t)mesh.positions_size);
    std::memcpy ((char*)data_ptr + mesh.positions_size, mesh.uvs.data (), (size_t)mesh.uvs_size);
    std::memcpy ((char*)data_ptr + mesh.positions_size + mesh.uvs_size, mesh.indices.data (), (size_t)mesh.indices_size);

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

    vkCmdCopyBuffer (copy_cmd_buffers.command_buffers[0], staging_vertex_index_buffer.buffer, vertex_index_buffer.buffer, 1, &buffer_copy);
    copy_cmd_buffers.end ();

    common_graphics_obj->device_queues.graphics_queue.submit (copy_cmd_buffers.command_buffers);
    vkQueueWaitIdle (common_graphics_obj->device_queues.graphics_queue.queue);


    staging_vertex_index_buffer_memory.unmap ();
}
