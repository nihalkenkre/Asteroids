#include "common_graphics.hpp"

#include <cstdio>


common_graphics::common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    printf ("common_graphics::common_graphics\n");

    instance = std::make_unique<vk_instance> ();
}