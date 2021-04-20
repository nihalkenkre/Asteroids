#include "vk_physical_device.hpp"

#include "error.hpp"


vk_physical_device::vk_physical_device (const VkInstance& instance)
{
    printf ("vk_physical_device::vk_physical_device\n");

    uint32_t device_count;
    VkResult result = vkEnumeratePhysicalDevices (instance, &device_count, nullptr);
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ENUMERATE_PHYSICAL_DEVICES;
    }

    std::vector<VkPhysicalDevice> physical_devices (device_count);
    result = vkEnumeratePhysicalDevices (instance, &device_count, physical_devices.data ());
    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_ENUMERATE_PHYSICAL_DEVICES;
    }

    physical_device = physical_devices[0];

    vkGetPhysicalDeviceMemoryProperties (physical_device, &memory_properties);
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties (physical_device, &device_properties);
    
    limits = device_properties.limits;
}

std::tuple<uint32_t, uint32_t, uint32_t> vk_physical_device::get_queue_family_indices () const
{
    uint32_t property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &property_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_family_properties (property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &property_count, queue_family_properties.data ());

    uint32_t graphics_queue_family_index = 0;
    uint32_t compute_queue_family_index = 0;
    uint32_t transfer_queue_family_index = 0;
    
    graphics_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                std::find_if (queue_family_properties.begin (), 
                                                            queue_family_properties.end (), 
                                                            [&](const VkQueueFamilyProperties& family_property) 
                                                                { 
                                                                    return (family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT);
                                                                }
                                                            )
                                                );

    auto compute_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                   queue_family_properties.end (), 
                                                   [&](const VkQueueFamilyProperties& family_property) 
                                                        { 
                                                            return (family_property.queueFlags & VK_QUEUE_COMPUTE_BIT) && (!(family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT));
                                                        }
                                                   );

    if (compute_family_index_iter != queue_family_properties.end ())
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), compute_family_index_iter);
    }
    else
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                    std::find_if (queue_family_properties.begin (), 
                                                                  queue_family_properties.end (), 
                                                                  [&](const VkQueueFamilyProperties& family_property) 
                                                                  { 
                                                                      return (family_property.queueFlags & VK_QUEUE_COMPUTE_BIT); 
                                                                  }
                                                                  )
                                                    );
    }

    auto transfer_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                    queue_family_properties.end (), 
                                                    [&](const VkQueueFamilyProperties& family_property) 
                                                        { 
                                                            return (family_property.queueFlags & VK_QUEUE_TRANSFER_BIT) && 
                                                                (!(family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT)) && 
                                                                (!(family_property.queueFlags & VK_QUEUE_COMPUTE_BIT)); 
                                                        }
                                                    );
    if (transfer_family_index_iter != queue_family_properties.end ())
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), transfer_family_index_iter);
    }
    else
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                     std::find_if (queue_family_properties.begin (), 
                                                                   queue_family_properties.end (), 
                                                                   [&](const VkQueueFamilyProperties& family_property) 
                                                                        { 
                                                                            return (family_property.queueFlags & VK_QUEUE_TRANSFER_BIT); 
                                                                        }
                                                                   )
                                                     );
    }

    return std::make_tuple (graphics_queue_family_index, compute_queue_family_index, transfer_queue_family_index);
}