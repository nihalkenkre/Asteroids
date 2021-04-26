#include "common_graphics.hpp"

#include "error.hpp"

#include <cstdio>
#include <tuple>
#include <map>


common_graphics::common_graphics (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    printf ("common_graphics::common_graphics\n");

    instance = vk_instance ();
    physical_device = vk_physical_device (instance.instance);
    std::tie (graphics_queue_family_index, compute_queue_family_index, transfer_queue_family_index) = physical_device.get_queue_family_indices ();
   
    surface = vk_surface (instance.instance, physical_device.physical_device,
                            h_instance, h_wnd, graphics_queue_family_index
    );

    get_queue_create_infos_indices ();

    graphics_device = vk_graphics_device (
        physical_device.physical_device,
        queue_create_infos
    );
    swapchain = vk_swapchain (graphics_device.graphics_device, surface);

    create_swapchain_image_views ();

    transfer_command_pool = vk_command_pool (graphics_device.graphics_device, transfer_queue_family_index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    graphics_command_pool = vk_command_pool (graphics_device.graphics_device, graphics_queue_family_index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

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
        std::vector<float> priorities (family_index_queue_count.second, 1);
        queue_priorities.push_back (priorities);

        VkDeviceQueueCreateInfo queue_create_info = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            family_index_queue_count.first,
            family_index_queue_count.second,
            nullptr
        };
        queue_create_infos.push_back (queue_create_info);
    }

    uint32_t index_counter = 0;

    for (auto &q_c_i : queue_create_infos)
    {
        q_c_i.pQueuePriorities = queue_priorities[index_counter].data ();
        ++index_counter;
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

void common_graphics::create_swapchain_image_views ()
{
    uint32_t image_count = 0;
    VkResult result = vkGetSwapchainImagesKHR (
        graphics_device.graphics_device, swapchain.swapchain, 
        &image_count, nullptr);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_GET_SWAPCHAIN_IMAGES;
    }

    swapchain_images.resize (image_count);

    result = vkGetSwapchainImagesKHR (
        graphics_device.graphics_device, swapchain.swapchain,
        &image_count, swapchain_images.data ());

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_GET_SWAPCHAIN_IMAGES;
    }

    swapchain_image_views.reserve (image_count);

    VkImageSubresourceRange subresource_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    VkComponentMapping component_mapping = { 
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, 
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY 
    };

    for (const auto& i : swapchain_images)
    {
        swapchain_image_views.emplace_back (
            vk_image_view (
                graphics_device.graphics_device, i,
                VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_B8G8R8A8_UNORM,
                component_mapping, subresource_range
            )
        );
    }
}
