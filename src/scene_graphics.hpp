#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"
#include "utils.hpp"

class scene_graphics
{
public:
    scene_graphics (common_graphics* common_graphics_obj);

private:

    void create_geometry_buffers (common_graphics* common_graphics_obj);
    void create_image_buffers (common_graphics* common_graphics_obj);

    mesh scene_mesh;
    
    vk_buffer vertex_index_buffer;
    vk_device_memory vertex_index_device_memory;
};

#endif