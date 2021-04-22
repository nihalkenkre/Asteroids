cmake_minimum_required (VERSION 3.8)

add_compile_options ($<$<CXX_COMPILER_ID:MSVC>:/MP>)
add_compile_options ($<$<CXX_COMPILER_ID:MSVC>:/GR->)


add_definitions (-DUNICODE -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS)

include_directories ($ENV{VULKAN_SDK}/Include)
include_directories (${CMAKE_SOURCE_DIR}/src)
include_directories (${CMAKE_SOURCE_DIR}/src/external/stb)

if (NOT CMAKE_CL_64)
	link_directories ($ENV{VULKAN_SDK}/Lib32)
endif ()

if (CMAKE_CL_64)
	link_directories ($ENV{VULKAN_SDK}/Lib)
endif ()

set (VK_HPP vk_objects/vk_buffer.hpp vk_objects/vk_command_buffers.hpp vk_objects/vk_command_pool.hpp vk_objects/vk_descriptor_pool.hpp vk_objects/vk_descriptor_set_layout.hpp vk_objects/vk_descriptor_sets.hpp vk_objects/vk_device_memory.hpp vk_objects/vk_graphics_device.hpp vk_objects/vk_graphics_pipeline_layout.hpp vk_objects/vk_graphics_pipeline.hpp vk_objects/vk_image_view.hpp vk_objects/vk_image.hpp vk_objects/vk_instance.hpp vk_objects/vk_physical_device.hpp vk_objects/vk_queue.hpp vk_objects/vk_sampler.hpp vk_objects/vk_shader_module.hpp vk_objects/vk_surface.hpp vk_objects/vk_swapchain.hpp vk_objects/vk_render_pass.hpp vk_objects/vk_framebuffer.hpp vk_objects/vk_semaphore.hpp)
set (VK_CPP vk_objects/vk_buffer.cpp vk_objects/vk_command_buffers.cpp vk_objects/vk_command_pool.cpp vk_objects/vk_descriptor_pool.cpp vk_objects/vk_descriptor_set_layout.cpp vk_objects/vk_descriptor_sets.cpp vk_objects/vk_device_memory.cpp vk_objects/vk_graphics_device.cpp vk_objects/vk_graphics_pipeline_layout.cpp vk_objects/vk_graphics_pipeline.cpp vk_objects/vk_image_view.cpp vk_objects/vk_image.cpp vk_objects/vk_instance.cpp vk_objects/vk_physical_device.cpp vk_objects/vk_queue.cpp vk_objects/vk_sampler.cpp vk_objects/vk_shader_module.cpp vk_objects/vk_surface.cpp vk_objects/vk_swapchain.cpp vk_objects/vk_render_pass.cpp vk_objects/vk_framebuffer.cpp vk_objects/vk_semaphore.cpp)

set (HPP error.hpp game.hpp log.hpp common_graphics.hpp scene_graphics.hpp types.hpp vulkan_interface.hpp utils.hpp compute.hpp actor_vert.hpp actor_frag.hpp vk_utils.hpp scene.hpp graphics.hpp ${VK_HPP})
set (CPP main.cpp game.cpp log.cpp common_graphics.cpp scene_graphics.cpp types.cpp vulkan_interface.cpp utils.cpp compute.cpp vk_utils.cpp scene.cpp graphics.cpp ${VK_CPP})

add_executable (Asteroids WIN32 ${HPP} ${CPP})
target_link_libraries (Asteroids vulkan-1)
