#include "scene_graphics.hpp"
#include "utils.hpp"

#include "error.hpp"
#include <cstdio>


scene_graphics::scene_graphics (const common_graphics* common_graphics_obj) : common_graphics_obj (common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    create_geometry_buffers ();
    //create_image_buffers ();
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

    vertex_index_buffer = vk_buffer (common_graphics_obj->graphics_device.graphics_device, buffer_data( data_size, VK_SHARING_MODE_EXCLUSIVE,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT , common_graphics_obj->queue_family_indices.graphics_queue_family_index ));

    vertex_index_device_memory = vk_device_memory (
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
    image_data background_image_data ("background.png");
    image_data player_image_data ("player.png");
    image_data asteroid_image_data ("asteroid.png");
    image_data bullet_image_data ("bullet.png");

    std::vector<image_data> images{ background_image_data, player_image_data, asteroid_image_data, bullet_image_data };

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

    std::vector<VkDeviceSize> image_offsets;
    image_offsets.reserve (images.size ());

    std::vector<VkExtent3D> image_extents;
    image_extents.reserve (images.size ());

    uint32_t current_offset = 0;

    for (const auto& i : images)
    {
        std::memcpy ((char*)mapped_data_ptr + current_offset, i.pixels.data (), i.pixels.size ());
        image_offsets.emplace_back (current_offset);

        image_extents.emplace_back (VkExtent3D {i.width, i.height, 1});

        current_offset += i.pixels.size ();
    }

    staging_memory.unmap ();

    vk_image background_image (common_graphics_obj->graphics_device.graphics_device, background_image_data);
    vk_image player_image (common_graphics_obj->graphics_device.graphics_device, player_image_data);
    vk_image asteroid_image (common_graphics_obj->graphics_device.graphics_device, asteroid_image_data);
    vk_image bullet_image (common_graphics_obj->graphics_device.graphics_device, bullet_image_data);

    std::vector<VkImage> vk_images{ background_image.vk_img, player_image.vk_img, asteroid_image.vk_img, bullet_image.vk_img };

    images_device_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        vk_images,
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vk_type::image
    );

    background_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    player_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    asteroid_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    bullet_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    staging_buffer.copy_to_images (
        vk_images,
        image_extents,
        image_offsets,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

        background_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    player_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    asteroid_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    bullet_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        common_graphics_obj->queue_family_indices.transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->device_queues.transfer_queue.queue
    );

    
}
