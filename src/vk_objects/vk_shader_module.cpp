#include "vk_shader_module.hpp"

#include "error.hpp"


vk_shader_module::vk_shader_module (
    const VkDevice& device, 
    const std::vector<uint32_t> shader_code) : device (device)
{
    printf ("vk_shader_module::vk_shader_module\n");

    VkShaderModuleCreateInfo create_info = {
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		nullptr,
		0,
		shader_code.size (),
		shader_code.data ()
    };

    VkResult result = vkCreateShaderModule (device, &create_info, nullptr, &shader_module);
    if (result != VK_SUCCESS) 
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_SHADER_MODULE;
    }
}

vk_shader_module::vk_shader_module (vk_shader_module&& other) noexcept
{
    printf ("vk_shader_module move ctor\n");

    *this = std::move (other);
}

vk_shader_module& vk_shader_module::operator= (vk_shader_module&& other) noexcept
{
    printf ("vk_shader_module move assignment\n");

    shader_module = other.shader_module;
    device = other.device;

    other.shader_module = VK_NULL_HANDLE;
    other.device = VK_NULL_HANDLE;

    return *this;
}

vk_shader_module::~vk_shader_module () noexcept
{
    printf ("vk_shader_module::~vk_shader_module\n");

    if (shader_module != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule (device, shader_module, nullptr);
    }
}