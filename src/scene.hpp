#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>

#include "common_graphics.hpp"
#include "scene_graphics.hpp"

class scene
{
public:
    scene () {}
    scene (const common_graphics* common_graphics_obj);

    void process_left_mouse_click (const int32_t x, const int32_t y);
    void process_right_mouse_click (const int32_t x, const int32_t y);
    void process_key_down (const WPARAM w_param);
    void process_key_up (const WPARAM l_param);

    void update (uint32_t delta_msecs);
    void submit_present ();

private:
    scene_graphics scene_graphics_obj;
};

#endif
