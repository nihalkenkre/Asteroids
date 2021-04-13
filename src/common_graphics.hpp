#ifndef COMMON_GRAPHICS_HPP
#define COMMON_GRAPHICS_HPP

#include <Windows.h>
#include <memory>

#include "vk_objects.hpp"

class common_graphics
{
public:
	common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd);

	vk_instance instance;
	vk_surface surface; 
	vk_graphics_device graphics_device;
	vk_physical_device physical_device;
	vk_swapchain swapchain;
	vk_sampler common_sampler;
	vk_command_pool transfer_command_pool;
	vk_queue_family_indices queue_family_indices;
	vk_queue_info queue_infos;
	vk_device_queues device_queues;

	std::vector<VkImage> swapchain_images;
	std::vector<vk_image_view> swapchain_image_views;
};

#endif