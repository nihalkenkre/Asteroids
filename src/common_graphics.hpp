#ifndef COMMON_GRAPHICS_HPP
#define COMMON_GRAPHICS_HPP

#include <Windows.h>
#include <memory>

#include "vk_objects.hpp"

class common_graphics
{
public:
	common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd);

	std::unique_ptr<vk_instance> instance;
};

#endif