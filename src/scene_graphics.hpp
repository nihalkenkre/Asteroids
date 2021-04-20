#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"
#include "utils.hpp"

#include "vk_objects/vk_buffer.hpp"
#include "vk_objects/vk_device_memory.hpp"
#include "vk_objects/vk_image.hpp"
#include "vk_objects/vk_image_view.hpp"
#include "vk_objects/vk_graphics_pipeline_layout.hpp"
#include "vk_objects/vk_shader_module.hpp"

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

    vk_graphics_pipeline_layout graphics_pipeline_layout;

    vk_shader_module vertex_shader_module;
    vk_shader_module fragment_shader_module;
};

#endif