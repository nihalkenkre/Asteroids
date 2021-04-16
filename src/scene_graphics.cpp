#include "scene_graphics.hpp"
#include "utils.hpp"

#include "error.hpp"
#include <cstdio>


scene_graphics::scene_graphics (const common_graphics* common_graphics_obj) : common_graphics_obj (common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    create_geometry_buffers ();
}

scene_graphics::~scene_graphics ()
{
    
}

void scene_graphics::create_geometry_buffers ()
{
    VkDeviceSize data_size = scene_mesh.positions_size + scene_mesh.uvs_size + scene_mesh.indices_size;

    buffer_data b (data_size, VK_SHARING_MODE_EXCLUSIVE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, common_graphics_obj->queue_family_indices.graphics_queue_family_index);

    vk_buffer staging_buffer (common_graphics_obj->graphics_device.graphics_device, b);

    vk_device_memory staging_buffer_memory (
        common_graphics_obj->graphics_device.graphics_device, std::vector<VkBuffer> {staging_buffer.buffer},
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        vk_type::buffer
    );

    HANDLE mapped_data_ptr = staging_buffer_memory.map (0, data_size);

    std::memcpy (mapped_data_ptr, scene_mesh.positions.data (), (size_t)scene_mesh.positions_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size, scene_mesh.uvs.data (), (size_t)scene_mesh.uvs_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size + scene_mesh.uvs_size, scene_mesh.indices.data (), (size_t)scene_mesh.indices_size);

    staging_buffer_memory.unmap ();

    vk_buffer vertex_index_buffer (common_graphics_obj->graphics_device.graphics_device, buffer_data( data_size, VK_SHARING_MODE_EXCLUSIVE,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT , common_graphics_obj->queue_family_indices.graphics_queue_family_index ));

    vk_device_memory vertex_index_device_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkBuffer>{ vertex_index_buffer.buffer },
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vk_type::buffer
    );

    staging_buffer.copy_to_buffer(
        vertex_index_buffer.buffer,
        data_size,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );
}

void scene_graphics::create_image_buffers ()
{
    std::vector<image_data> images;
    images.reserve (4);
 
    images.emplace_back (image_data ("background.png"));
    images.emplace_back (image_data ("player.png"));
    images.emplace_back (image_data ("asteroid.png"));
    images.emplace_back (image_data ("bullet.png"));

    uint32_t total_size = 0;
    for (const auto& i : images)
    {
        total_size += i.pixels.size ();
    }

    buffer_data b (
        total_size, 
        VK_SHARING_MODE_EXCLUSIVE, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        common_graphics_obj->queue_family_indices.graphics_queue_family_index
    );

    vk_buffer staging_buffer (common_graphics_obj->graphics_device.graphics_device, b);

    vk_device_memory staging_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device, std::vector<VkBuffer>{ staging_buffer.buffer },
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        vk_type::buffer
    );

    HANDLE mapped_data_ptr = staging_memory.map (0, total_size);
    staging_memory.unmap ();
}
