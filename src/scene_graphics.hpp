#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"

class scene_graphics
{
public:
    scene_graphics (common_graphics* common_graphics_obj);

private:
    vk_mesh mesh;
    
    vk_buffer vertex_index_buffer;
    vk_device_memory vertex_index_device_memory;
};

#endif