#ifndef COMMON_GRAPHICS_HPP
#define COMMON_GRAPHICS_HPP

#include <Windows.h>
#include <memory>

#include "vk_objects/vk_instance.hpp"
#include "vk_objects/vk_surface.hpp"
#include "vk_objects/vk_graphics_device.hpp"
#include "vk_objects/vk_physical_device.hpp"
#include "vk_objects/vk_swapchain.hpp"
#include "vk_objects/vk_sampler.hpp"
#include "vk_objects/vk_command_pool.hpp"
#include "vk_objects/vk_image_view.hpp"

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
	vk_command_pool graphics_command_pool;

	std::vector<VkImage> swapchain_images;
	std::vector<vk_image_view> swapchain_image_views;

	VkQueue graphics_queue;
	VkQueue compute_queue;
	VkQueue transfer_queue;

	uint32_t graphics_queue_family_index;
	uint32_t compute_queue_family_index;
	uint32_t transfer_queue_family_index;

	std::vector<uint32_t> queue_indices;

private:
	void get_queue_create_infos_indices ();
	void get_device_queues ();
	void create_swapchain_image_views ();

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
};

#endif