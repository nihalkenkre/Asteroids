#include "scene.hpp"

#include <cstdio>


scene::scene (const common_graphics* common_graphics_obj)
{
    printf ("scene::scene\n");

    scene_graphics_obj = scene_graphics (common_graphics_obj);
}

void scene::process_left_mouse_click (const int32_t x, const int32_t y)
{
}

void scene::process_right_mouse_click (const int32_t x, const int32_t y)
{
}

void scene::process_key_down (const WPARAM w_param)
{
}

void scene::process_key_up (const WPARAM l_param)
{
}

void scene::update (uint32_t delta_msecs)
{
}

void scene::submit_present ()
{
}
