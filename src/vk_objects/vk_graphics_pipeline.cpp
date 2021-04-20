#include "vk_graphics_pipeline.hpp"

#include "error.hpp"
#include <cstdio>
#include <memory>


vk_graphics_pipeline::vk_graphics_pipeline (vk_graphics_pipeline&& other) noexcept
{
    printf ("vk_graphics_pipeline move ctor\n");

    *this = std::move (other);
}

vk_graphics_pipeline& vk_graphics_pipeline::operator=(vk_graphics_pipeline&& other) noexcept
{
    printf ("vk_graphics_pipeline move assignment\n");

    graphics_pipeline = other.graphics_pipeline;
    device = other.device;

    other.graphics_pipeline = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_graphics_pipeline::~vk_graphics_pipeline () noexcept
{
    printf ("vk_graphics_pipeline::vk_graphics_pipeline\n");

    if (graphics_pipeline != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyPipeline (device, graphics_pipeline, nullptr);
    }
}
