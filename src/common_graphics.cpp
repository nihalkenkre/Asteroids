#include "common_graphics.hpp"

#include <cstdio>
#include <tuple>
#include <map>


common_graphics::common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    printf ("common_graphics::common_graphics\n");

    instance = vk_instance ();
    physical_device = vk_physical_device (instance.instance);
    std::tie (
        graphics_queue_family_index, 
        compute_queue_family_index, 
        transfer_queue_family_index) = physical_device.get_queue_family_indices ();
    surface = vk_surface (
        instance.instance,
        physical_device.physical_device,
        h_instance,
        h_wnd,
        graphics_queue_family_index
    );
/*    queue_infos = vk_queue_info (queue_family_indices);

    device_queues = vk_device_queues (
        physical_device.physical_device,
        graphics_device.graphics_device,
        queue_family_indices,
        queue_infos.queue_indices
    );*/
    get_queue_create_infos_indices ();
    graphics_device = vk_graphics_device (
        physical_device.physical_device,
        queue_create_infos
    );
    swapchain = vk_swapchain (graphics_device.graphics_device, surface);
    transfer_command_pool = vk_command_pool (
        graphics_device.graphics_device,
        transfer_queue_family_index,
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
    );
    get_device_queues ();
    common_sampler = vk_sampler (graphics_device.graphics_device);
}

void common_graphics::get_queue_create_infos_indices ()
{
    std::vector<uint32_t> family_indices = { 
                                            graphics_queue_family_index, 
                                            compute_queue_family_index, 
                                            transfer_queue_family_index
                                        };

    std::map<uint32_t, uint32_t> family_indices_queue_count;

    for (const auto& family_index : family_indices)
    {
        family_indices_queue_count[family_index] = static_cast<uint32_t> (std::count (family_indices.begin (), family_indices.end (), family_index));
    }

    for (const auto& family_index_family_count : family_indices_queue_count)
    {
        for (uint32_t i = 0; i < family_index_family_count.second; ++i)
        {
            queue_indices.push_back (i);
        }
    }

    for (const auto& family_index_queue_count : family_indices_queue_count)
    {
        std::vector<float> queue_priorities (family_index_queue_count.second, 1);
        VkDeviceQueueCreateInfo queue_create_info = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            family_index_queue_count.first,
            family_index_queue_count.second,
            queue_priorities.data ()
        };

        queue_create_infos.emplace_back (queue_create_info);
    }
}

void common_graphics::get_device_queues ()
{
    uint32_t property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties (physical_device.physical_device, &property_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_family_properties (property_count);
    vkGetPhysicalDeviceQueueFamilyProperties (physical_device.physical_device, &property_count, queue_family_properties.data ());

    uint32_t graphics_queue_index = 0;
	uint32_t compute_queue_index = graphics_queue_family_index == compute_queue_family_index ? 1 : 0;
	uint32_t transfer_queue_index = transfer_queue_family_index == compute_queue_family_index ? compute_queue_index + 1 : 0;

	vkGetDeviceQueue (graphics_device.graphics_device, graphics_queue_family_index, graphics_queue_index, &graphics_queue);
	vkGetDeviceQueue (graphics_device.graphics_device, compute_queue_family_index, compute_queue_index, &compute_queue);
	vkGetDeviceQueue (graphics_device.graphics_device, transfer_queue_family_index, transfer_queue_index, &transfer_queue);
}