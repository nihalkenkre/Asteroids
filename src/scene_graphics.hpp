#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"
#include "utils.hpp"

class scene_graphics
{
public:
    scene_graphics (const common_graphics* common_graphics_obj);
    
    scene_graphics (const scene_graphics& other) = delete;
    scene_graphics& operator= (const scene_graphics& other) = delete;

    scene_graphics (scene_graphics&& other) = delete;
    scene_graphics& operator= (scene_graphics&& other) = delete;

    ~scene_graphics ();

private:

    void create_geometry_buffers ();
    void create_image_buffers ();

    const common_graphics* common_graphics_obj;

    mesh scene_mesh;
    
    vk_buffer vertex_index_buffer;
    vk_device_memory vertex_index_device_memory;
};

#endif