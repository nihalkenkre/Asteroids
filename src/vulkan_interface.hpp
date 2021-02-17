#ifndef VULKAN_INTERFACE_HPP
#define VULKAN_INTERFACE_HPP

#include "error.hpp"

#ifdef WIN32
#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#elif __ANDROID__
#include "vulkan_wrapper.hpp"
#endif

#include <vector>

extern VkInstance instance;
extern VkPhysicalDevice physical_device;
extern VkDevice device;
extern uint32_t graphics_queue_family_index;
extern uint32_t compute_queue_family_index;
extern uint32_t transfer_queue_family_index;
extern VkQueue graphics_queue;
extern VkQueue compute_queue;
extern VkQueue transfer_queue;
extern VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
extern VkPhysicalDeviceLimits physical_device_limits;
extern VkExtent2D surface_extent;
extern VkSwapchainKHR swapchain;
extern VkImage* swapchain_images;
extern VkImageView* swapchain_image_views;
extern uint32_t swapchain_image_count;
extern VkExtent2D current_extent;
extern VkSurfaceFormatKHR chosen_surface_format;
extern VkCommandPool graphics_command_pool;
extern VkSampler common_sampler;

#ifdef WIN32
AGE_RESULT vulkan_interface_init (HINSTANCE h_instance, HWND h_wnd);
#elif __ANDROID__
AGE_RESULT vulkan_interface_init_from_app (struct android_app* p_app);
AGE_RESULT vulkan_interface_init_from_window (ANativeWindow* window);
#endif
void vulkan_interface_shutdown ();

#endif