#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"
#include "utils.hpp"

#include "vk_objects/vk_buffer.hpp"
#include "vk_objects/vk_device_memory.hpp"
#include "vk_objects/vk_image.hpp"
#include "vk_objects/vk_image_view.hpp"
#include "vk_objects/vk_descriptor_pool.hpp"
#include "vk_objects/vk_descriptor_set_layout.hpp"
#include "vk_objects/vk_descriptor_sets.hpp"
#include "vk_objects/vk_graphics_pipeline_layout.hpp"
#include "vk_objects/vk_render_pass.hpp"
#include "vk_objects/vk_framebuffer.hpp"
#include "vk_objects/vk_shader_module.hpp"
#include "vk_objects/vk_graphics_pipeline.hpp"
#include "vk_objects/vk_command_buffers.hpp"
#include "vk_objects/vk_semaphore.hpp"
#include "vk_objects/vk_fence.hpp"

class scene_graphics
{
public:
    scene_graphics (const common_graphics* common_graphics_obj);
    
    scene_graphics (const scene_graphics& other) = delete;
    scene_graphics& operator= (const scene_graphics& other) = delete;

    scene_graphics (scene_graphics&& other) = delete;
    scene_graphics& operator= (scene_graphics&& other) = delete;

private:

    void create_geometry_buffers ();
    void create_image_buffers ();
    void create_graphics_pipeline ();
    void create_descriptor_sets ();
    void create_graphics_pipeline_set_layout ();
    void create_swapchain_render_pass ();
    void create_swapchain_framebuffers ();
    void create_swapchain_command_buffers ();
    void create_swapchain_semaphores_fences ();

    const common_graphics* common_graphics_obj;

    mesh_data scene_mesh;
    
    vk_buffer vertex_index_buffer;
    vk_device_memory vertex_index_device_memory;

    vk_device_memory images_device_memory;

    vk_image background_image;
    vk_image player_image;
    vk_image asteroid_image;
    vk_image bullet_image;

    vk_image_view background_image_view;
    vk_image_view player_image_view;
    vk_image_view asteroid_image_view;
    vk_image_view bullet_image_view;

    vk_descriptor_pool descriptor_pool;

    vk_graphics_pipeline_layout graphics_pipeline_layout;
    vk_descriptor_set_layout transform_descriptor_set_layout;
    vk_descriptor_set_layout texture_descriptor_set_layout;

    vk_descriptor_set transform_descriptor_set;
    vk_descriptor_set texture_descriptor_set;

    vk_render_pass render_pass;
    std::vector<vk_framebuffer> framebuffers;

    vk_shader_module vertex_shader_module;
    vk_shader_module fragment_shader_module;

    vk_graphics_pipeline graphics_pipeline;

    vk_command_buffers swapchain_command_buffers;

    vk_semaphore swapchain_wait_semaphore;
    std::vector<vk_semaphore> swapchain_signal_semaphores;
    std::vector<vk_fence> swapchain_fences;
};

#endif