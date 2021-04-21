#include "scene_graphics.hpp"
#include "utils.hpp"
#include "error.hpp"
#include "actor_vert.hpp"
#include "actor_frag.hpp"

#include <cstdio>


scene_graphics::scene_graphics (const common_graphics* common_graphics_obj) : common_graphics_obj (common_graphics_obj)
{
    printf ("scene_graphics::scene_graphics\n");

    create_geometry_buffers ();
    create_image_buffers ();
    create_descriptor_sets ();
    create_graphics_pipeline_set_layout ();
    create_swapchain_render_pass ();
    create_swapchain_framebuffers ();
    create_graphics_pipeline ();
}


void scene_graphics::create_geometry_buffers ()
{
    VkDeviceSize data_size = scene_mesh.positions_size + scene_mesh.uvs_size + scene_mesh.indices_size;

    buffer_data b (data_size, VK_SHARING_MODE_EXCLUSIVE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, common_graphics_obj->graphics_queue_family_index);

    vk_buffer staging_buffer (common_graphics_obj->graphics_device.graphics_device, b);

    vk_device_memory staging_buffer_memory (
        common_graphics_obj->graphics_device.graphics_device, std::vector<VkBuffer> {staging_buffer.buffer},
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    HANDLE mapped_data_ptr = staging_buffer_memory.map (0, data_size);

    std::memcpy (mapped_data_ptr, scene_mesh.positions.data (), (size_t)scene_mesh.positions_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size, scene_mesh.uvs.data (), (size_t)scene_mesh.uvs_size);
    std::memcpy ((char*)mapped_data_ptr + scene_mesh.positions_size + scene_mesh.uvs_size, scene_mesh.indices.data (), (size_t)scene_mesh.indices_size);

    staging_buffer_memory.unmap ();

    vertex_index_buffer = vk_buffer (
        common_graphics_obj->graphics_device.graphics_device, 
        buffer_data (
            data_size, 
            VK_SHARING_MODE_EXCLUSIVE,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
            common_graphics_obj->graphics_queue_family_index));

    vertex_index_device_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkBuffer>{ vertex_index_buffer.buffer },
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    staging_buffer.copy_to_buffer(
        vertex_index_buffer.buffer,
        data_size,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
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
        total_size += static_cast<uint32_t> (i.pixels.size ());
    }

    buffer_data b (
        total_size,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        common_graphics_obj->graphics_queue_family_index
    );

    vk_buffer staging_buffer (common_graphics_obj->graphics_device.graphics_device, b);

    vk_device_memory staging_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device, std::vector<VkBuffer>{ staging_buffer.buffer },
        common_graphics_obj->physical_device.memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
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

        current_offset += static_cast<uint32_t> (i.pixels.size ());
    }

    staging_memory.unmap ();

    background_image = vk_image (common_graphics_obj->graphics_device.graphics_device, background_image_data);
    player_image = vk_image (common_graphics_obj->graphics_device.graphics_device, player_image_data);
    asteroid_image = vk_image (common_graphics_obj->graphics_device.graphics_device, asteroid_image_data);
    bullet_image = vk_image (common_graphics_obj->graphics_device.graphics_device, bullet_image_data);

    std::vector<VkImage> vk_images{ background_image.vk_img, player_image.vk_img, asteroid_image.vk_img, bullet_image.vk_img };

    images_device_memory = vk_device_memory (
        common_graphics_obj->graphics_device.graphics_device,
        vk_images,
        common_graphics_obj->physical_device.memory_properties,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    background_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    player_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    asteroid_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    bullet_image.change_layout (
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    staging_buffer.copy_to_images (
        vk_images,
        image_extents,
        image_offsets,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    background_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    player_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    asteroid_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    bullet_image.change_layout (
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        common_graphics_obj->transfer_queue_family_index,
        common_graphics_obj->transfer_queue_family_index,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        common_graphics_obj->transfer_command_pool.command_pool,
        common_graphics_obj->transfer_queue
    );

    VkImageSubresourceRange subresource_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    VkComponentMapping component_mapping = { 
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, 
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY 
    };

    background_image_view = vk_image_view (
        common_graphics_obj->graphics_device.graphics_device,
        background_image.vk_img,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        component_mapping,
        subresource_range
    );

    player_image_view = vk_image_view (
        common_graphics_obj->graphics_device.graphics_device,
        player_image.vk_img,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        component_mapping,
        subresource_range
    );

    asteroid_image_view = vk_image_view (
        common_graphics_obj->graphics_device.graphics_device,
        asteroid_image.vk_img,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        component_mapping,
        subresource_range
    );

    bullet_image_view = vk_image_view (
        common_graphics_obj->graphics_device.graphics_device,
        bullet_image.vk_img,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        component_mapping,
        subresource_range
    );
}

void scene_graphics::create_descriptor_sets ()
{
    std::vector<VkDescriptorPoolSize> pool_sizes{
        {
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            1
        },
        {
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            4
        }
    };

    descriptor_pool = vk_descriptor_pool (
        common_graphics_obj->graphics_device.graphics_device,
        pool_sizes,
        2
    );

    VkDescriptorSetLayoutBinding transform_layout_binding {
        0,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        1,
        VK_SHADER_STAGE_VERTEX_BIT,
        nullptr
    };

    transform_descriptor_set_layout = vk_descriptor_set_layout (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkDescriptorSetLayoutBinding> {transform_layout_binding}
    );

    VkDescriptorSetLayoutBinding texture_layout_binding {
        0,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        4,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr
    };

    texture_descriptor_set_layout = vk_descriptor_set_layout (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkDescriptorSetLayoutBinding> {texture_layout_binding}
    );

    transform_descriptor_set = vk_descriptor_set (
        common_graphics_obj->graphics_device.graphics_device,
        descriptor_pool.descriptor_pool,
        transform_descriptor_set_layout.descriptor_set_layout
    );

    texture_descriptor_set = vk_descriptor_set (
        common_graphics_obj->graphics_device.graphics_device,
        descriptor_pool.descriptor_pool,
        texture_descriptor_set_layout.descriptor_set_layout
    );

    std::vector<VkDescriptorImageInfo> image_infos{
        {
            common_graphics_obj->common_sampler.sampler,
            background_image_view.image_view,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        },
        {
            common_graphics_obj->common_sampler.sampler,
            player_image_view.image_view,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        },
        {
            common_graphics_obj->common_sampler.sampler,
            asteroid_image_view.image_view,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        },
        {
            common_graphics_obj->common_sampler.sampler,
            bullet_image_view.image_view,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }
    };


    VkWriteDescriptorSet texture_descriptor_set_write{
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        nullptr,
        texture_descriptor_set.descriptor_set,
        0,
        0,
        static_cast<uint32_t> (image_infos.size ()),
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        image_infos.data (),
        nullptr,
        nullptr
    };

    transform_descriptor_set.update (texture_descriptor_set_write);
}

void scene_graphics::create_graphics_pipeline_set_layout ()
{
    std::vector<VkPushConstantRange> push_constant_ranges{
        {
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof (float)
        },
        {
            VK_SHADER_STAGE_FRAGMENT_BIT,
            sizeof (float),
            sizeof (uint32_t)
        }
    };

    graphics_pipeline_layout = vk_graphics_pipeline_layout (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkDescriptorSetLayout>{transform_descriptor_set_layout.descriptor_set_layout, texture_descriptor_set_layout.descriptor_set_layout},
        push_constant_ranges
    );
}

void scene_graphics::create_swapchain_render_pass ()
{
    VkAttachmentDescription color_attachment_description = {
        0,
        common_graphics_obj->surface.format.format,
        VK_SAMPLE_COUNT_1_BIT,
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        VK_ATTACHMENT_STORE_OP_STORE,
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        VK_ATTACHMENT_STORE_OP_DONT_CARE,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference color_attachment_reference = {
        0,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription color_subpass_description = {
        0,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        0,
        nullptr,
        1,
        &color_attachment_reference,
        nullptr,
        nullptr,
        0,
        nullptr
    };

    render_pass = vk_render_pass (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkAttachmentDescription> {color_attachment_description},
        std::vector<VkAttachmentReference> {color_attachment_reference},
        std::vector<VkSubpassDescription> {color_subpass_description}
    );
}


void scene_graphics::create_swapchain_framebuffers ()
{
    framebuffers.reserve (common_graphics_obj->swapchain_image_views.size ());

    for (const auto& iv : common_graphics_obj->swapchain_image_views)
    {
        framebuffers.emplace_back (
            vk_framebuffer (
                common_graphics_obj->graphics_device.graphics_device,
                render_pass.render_pass,
                common_graphics_obj->surface.capabilities.currentExtent,
                iv.image_view
            )
        );
    }
}

void scene_graphics::create_graphics_pipeline ()
{
    vertex_shader_module = vk_shader_module (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<uint32_t> {std::begin (actor_vert), std::end (actor_vert)}
    );

    fragment_shader_module = vk_shader_module (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<uint32_t> {std::begin (actor_frag), std::end (actor_frag)}
    );

    graphics_pipeline = vk_graphics_pipeline (
        common_graphics_obj->graphics_device.graphics_device,
        std::vector<VkShaderModule> {vertex_shader_module.shader_module, fragment_shader_module.shader_module},
        graphics_pipeline_layout.graphics_pipeline_layout,
        render_pass.render_pass,
        common_graphics_obj->surface.capabilities.currentExtent
    );
}