#include "scene.hpp"

#include <cstdio>


scene::scene (common_graphics* common_graphics_obj)
{
    printf ("scene::scene\n");

    scene_graphics_obj = std::make_unique <scene_graphics> (common_graphics_obj);
}