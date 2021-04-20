#ifndef VK_SHADER_MODULE_HPP
#define VK_SHADER_MODULE_HPP

#include <vulkan/vulkan.h>
#include <vector>

class vk_shader_module
{
public:
    vk_shader_module () : shader_module (VK_NULL_HANDLE), device (VK_NULL_HANDLE) {}
    vk_shader_module (
        const VkDevice& device,
        const std::vector<uint32_t> shader_code
    );

    vk_shader_module (const vk_shader_module& other) = delete;
    vk_shader_module& operator= (const vk_shader_module& other) = delete;

    vk_shader_module (vk_shader_module&& other) noexcept;
    vk_shader_module& operator= (vk_shader_module&& other) noexcept;

    ~vk_shader_module () noexcept;

    VkShaderModule shader_module;

private:
    VkDevice device;
};


#endif