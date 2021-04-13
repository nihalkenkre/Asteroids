#include "common_graphics.hpp"

#include <cstdio>


common_graphics::common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    printf ("common_graphics::common_graphics\n");

    instance = vk_instance ();
    physical_device = vk_physical_device (instance.instance);
    queue_family_indices = vk_queue_family_indices (physical_device.physical_device);
    surface = vk_surface (
        instance.instance,
        physical_device.physical_device,
        h_instance,
        h_wnd,
        queue_family_indices.graphics_queue_family_index
    );
    queue_infos = vk_queue_info (queue_family_indices);
    graphics_device = vk_graphics_device (
        physical_device.physical_device,
        queue_infos.queue_create_infos
    );
    device_queues = vk_device_queues (
        physical_device.physical_device,
        graphics_device.graphics_device,
        queue_family_indices,
        queue_infos.queue_indices
    );
    swapchain = vk_swapchain (graphics_device.graphics_device, surface);
    transfer_command_pool = vk_command_pool (
        graphics_device.graphics_device,
        queue_family_indices.transfer_queue_family_index,
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
    );
    common_sampler = vk_sampler (graphics_device.graphics_device);
}