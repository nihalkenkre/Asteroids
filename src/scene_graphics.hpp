#ifndef SCENE_GRAPHICS_HPP
#define SCENE_GRAPHICS_HPP

#include "common_graphics.hpp"

class scene_graphics
{
public:
    scene_graphics (const common_graphics* common_graphics_obj);

private:
    vk_mesh mesh;
};

#endif