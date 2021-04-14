#include "scene_graphics.hpp"
#include "utils.hpp"

#include <cstdio>


scene_graphics::scene_graphics (const common_graphics* common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    create_geometry_buffers (common_graphics_obj);
    create_image_buffers (common_graphics_obj);
}

void scene_graphics::create_geometry_buffers (const common_graphics* common_graphics_obj)
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
        common_graphics_obj->graphics_device.graphics_device, std::vector<vk_buffer>{ staging_buffer },
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

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
        std::vector<vk_buffer>{ vertex_index_buffer },
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vertex_index_buffer.copy_from_buffer (
        staging_buffer.buffer,
        data_size,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    staging_buffer_memory.unmap ();
}

void scene_graphics::create_image_buffers (const common_graphics* common_graphics_obj)
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
        std::vector<vk_buffer>{ staging_buffer },
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    HANDLE mapped_data_ptr = staging_memory.map (0, total_size);

    uint32_t current_offset = 0;

    for (const auto& i : images)
    {
        std::memcpy ((char*)mapped_data_ptr + current_offset, i.pixels.data (), i.pixels.size ());
        current_offset += i.pixels.size ();
    }

    std::vector<vk_image> vk_imgs;
    vk_imgs.reserve (images.size ());

    for (const auto& i : images)
    {
        vk_imgs.emplace_back (vk_image (common_graphics_obj->graphics_device.graphics_device, i));
    }

    vk_device_memory images_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        vk_imgs,
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    staging_memory.unmap ();
}
