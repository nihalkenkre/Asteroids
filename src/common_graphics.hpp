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
	std::unique_ptr<vk_graphics_device> graphics_device;
	std::unique_ptr<vk_surface> surface;
	std::unique_ptr<vk_physical_device> physical_device;
	std::unique_ptr<vk_swapchain> swapchain;
	std::unique_ptr<vk_command_pool> transfer_command_pool;
	std::unique_ptr<vk_queue_family_indices> queue_family_indices;
	std::unique_ptr<vk_queue_info> queue_infos;
	std::unique_ptr<vk_device_queues> device_queues;

	std::vector<VkImage> swapchain_images;
	std::vector<vk_image_view> swapchain_image_views;
};

#endif