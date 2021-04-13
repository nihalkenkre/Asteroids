#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>

#include "common_graphics.hpp"
#include "scene_graphics.hpp"

class scene
{
public:
    scene (common_graphics* common_graphics_obj);

private:
    std::unique_ptr<scene_graphics> scene_graphics_obj;
};

#endif
