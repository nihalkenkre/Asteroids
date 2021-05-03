#include "vk_graphics_pipeline.hpp"

#include "error.hpp"
#include <cstdio>
#include <memory>

#include <array>

vk_graphics_pipeline::vk_graphics_pipeline (
    const VkDevice& device, 
    const std::vector<VkShaderModule>& shader_modules,
    const std::vector<std::string>& main_functions,
    const std::vector<VkShaderStageFlagBits>& stage_flags,
    const VkPipelineLayout& graphics_pipeline_layout,
    const VkRenderPass& render_pass,
    const VkExtent2D& extent) : device (device)
{
    printf ("vk_graphics_pipeline::vk_graphics_pipeline\n");

    std::vector <VkPipelineShaderStageCreateInfo> stage_infos;
    stage_infos.reserve (shader_modules.size ());

    uint32_t index_counter = 0;
    
    for (const auto& sm : shader_modules)
    {
        VkPipelineShaderStageCreateInfo stage_info {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            stage_flags[index_counter],
            sm,
            main_functions[index_counter].c_str (),
            nullptr
        };
        
        stage_infos.emplace_back (stage_info);
        ++index_counter;
    }

    std::array<VkVertexInputBindingDescription, 2> vertex_input_binding_descriptions = {
        VkVertexInputBindingDescription {
			0,
			sizeof (float) * 2,
			VK_VERTEX_INPUT_RATE_VERTEX
		},
        VkVertexInputBindingDescription {
			1,
			sizeof (float) * 2,
			VK_VERTEX_INPUT_RATE_VERTEX
		}
	};

    std::array<VkVertexInputAttributeDescription, 2> vertex_input_attribute_descriptions = {
        VkVertexInputAttributeDescription {
            0,
            0,
            VK_FORMAT_R32G32_SFLOAT,
            0
        },
        VkVertexInputAttributeDescription {
            1,
            1,
            VK_FORMAT_R32G32_SFLOAT,
            0
        }
    };

	VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		nullptr,
		0,
        static_cast<uint32_t> (vertex_input_binding_descriptions.size ()),
		vertex_input_binding_descriptions.data (),
        static_cast<uint32_t> (vertex_input_attribute_descriptions.size ()),
		vertex_input_attribute_descriptions.data ()
	};

    VkPipelineInputAssemblyStateCreateInfo vertex_input_assembly_state_create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        VK_FALSE
    };

    VkViewport viewport = {
        0,
        (float)extent.height,
        (float)extent.width,
        -(float)extent.height,
        0,
        1
    };

    VkRect2D scissor = {
        {0,0},
        extent
    };

    VkPipelineViewportStateCreateInfo viewport_state_create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        nullptr,
        0,
        1,
        &viewport,
        1,
        &scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_FALSE,
        VK_FALSE,
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_BACK_BIT,
        VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_FALSE,
        0,
        0,
        0,
        1
    };

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_SAMPLE_COUNT_1_BIT
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
        VK_TRUE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_FALSE,
        VK_LOGIC_OP_NO_OP,
        1,
        &color_blend_attachment_state,
        {1,1,1,1}
    };

    VkGraphicsPipelineCreateInfo create_info {
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        nullptr,
        0,
        static_cast<uint32_t> (stage_infos.size ()),
        stage_infos.data (),
        &vertex_input_state_create_info,
        &vertex_input_assembly_state_create_info,
        nullptr,
        &viewport_state_create_info,
        &rasterization_state_create_info,
        &multisample_state_create_info,
        nullptr,
        &color_blend_state_create_info,
        nullptr,
        graphics_pipeline_layout,
        render_pass,
        0,
        VK_NULL_HANDLE,
        0
    };

    VkResult result = vkCreateGraphicsPipelines (
        device, VK_NULL_HANDLE, 1, &create_info, nullptr, &graphics_pipeline);

    if (result != VK_SUCCESS)
    {
        throw AGE_RESULT::ERROR_GRAPHICS_CREATE_GRAPHICS_PIPELINE;
    }
}

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
    printf ("vk_graphics_pipeline::~vk_graphics_pipeline\n");

    if (graphics_pipeline != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
    {
        vkDestroyPipeline (device, graphics_pipeline, nullptr);
    }
}
