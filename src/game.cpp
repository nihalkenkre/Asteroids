#include "game.hpp"
#include "graphics.hpp"
#include "types.hpp"
#include "vulkan_interface.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#include <random>

/*
bool is_w_pressed = false;
bool is_s_pressed = false;
bool is_d_pressed = false;
bool is_a_pressed = false;
bool is_space_bar_pressed = false;
bool is_up_arrow_pressed = false;
bool is_down_arrow_pressed = false;
bool is_right_arrow_pressed = false;
bool is_left_arrow_pressed = false;

float window_aspect_ratio = 1;

player_transform_inputs game_player_transform_inputs;
float3 game_player_output_position;
float game_player_output_rotation;
float2 game_player_output_scale;

uint32_t game_player_shooting_interval_msecs = 100;
uint32_t game_secs_since_last_shot = 0;

bullet_transform_inputs* game_bullets_transform_inputs;

float3* game_bullets_outputs_positions;
float* game_bullets_outputs_rotations;
float2* game_bullets_outputs_scales;

float game_bullets_max_lifetime_msecs = 500.f;

asteroid_transform_inputs* game_large_asteroids_transform_inputs;
float3* game_large_asteroids_outputs_positions;
float* game_large_asteroids_outputs_rotations;
float2* game_large_asteroids_outputs_scales;

asteroid_transform_inputs* game_small_asteroids_transform_inputs;
float3* game_small_asteroids_outputs_positions;
float* game_small_asteroids_outputs_rotations;
float2* game_small_asteroids_outputs_scales;

uint32_t game_large_asteroids_current_max_count = 0;
uint32_t game_large_asteroids_live_count = 0;

uint32_t game_small_asteroids_current_max_count = 0;
uint32_t game_small_asteroids_live_count = 0;

uint32_t game_bullets_current_max_count = 0;
uint32_t game_bullet_live_count = 0;

const uint32_t game_LARGE_ASTEROID_BATCH_SIZE = 50;
const uint32_t game_SMALL_ASTEROID_BATCH_SIZE = 150;
const uint32_t game_BULLET_BATCH_SIZE = 20;

bool should_update_command_buffers = false;

RECT window_rect;

int32_t last_mouse_x;
int32_t last_mouse_y;

uint32_t game_delta_time = 0;

std::mt19937 generator;

std::uniform_real_distribution<> game_large_asteroids_forward_vector_rand;
std::uniform_real_distribution<> game_large_asteroids_forward_speed_rand;
std::uniform_real_distribution<> game_large_asteroids_rotation_speed_rand;

std::uniform_real_distribution<> game_small_asteroids_forward_speed_rand;

std::uniform_real_distribution<> game_large_asteroids_output_position_x_rand;
std::uniform_real_distribution<> game_large_asteroids_output_position_y_rand;
std::uniform_real_distribution<> game_large_asteroids_output_rotation_rand;
*/


game_class::game_class (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    printf ("game_class::game_class\n");

    common_graphics_obj = std::make_unique<common_graphics> (h_instance, h_wnd);
    scene_obj = std::make_unique<scene> (common_graphics_obj.get ());
}

void game_class::process_key_down (const WPARAM w_param)
{
}

void game_class::process_key_up (const WPARAM w_param)
{

}

void game_class::process_left_mouse_click (const int32_t x, const int32_t y)
{

}

void game_class::process_right_mouse_click (const int32_t x, const int32_t y)
{

}

void game_class::update (uint32_t delta_time)
{

}

void game_class::submit_present ()
{

}

struct game_
{
    bool is_w_pressed = false;
    bool is_s_pressed = false;
    bool is_d_pressed = false;
    bool is_a_pressed = false;
    bool is_space_bar_pressed = false;
    bool is_up_arrow_pressed = false;
    bool is_down_arrow_pressed = false;
    bool is_right_arrow_pressed = false;
    bool is_left_arrow_pressed = false;

    float window_aspect_ratio = 1;

    player_transform_inputs player_transform_inputs;
    float3 player_output_position;
    float player_output_rotation;
    float2 player_output_scale;

    uint32_t player_shooting_interval_msecs = 100;
    uint32_t secs_since_last_shot = 0;

    bullet_transform_inputs* bullets_transform_inputs;

    float3* bullets_outputs_positions;
    float* bullets_outputs_rotations;
    float2* bullets_outputs_scales;

    float bullets_max_lifetime_msecs = 500.f;

    asteroid_transform_inputs* large_asteroids_transform_inputs;
    float3* large_asteroids_outputs_positions;
    float* large_asteroids_outputs_rotations;
    float2* large_asteroids_outputs_scales;

    asteroid_transform_inputs* small_asteroids_transform_inputs;
    float3* small_asteroids_outputs_positions;
    float* small_asteroids_outputs_rotations;
    float2* small_asteroids_outputs_scales;

    uint32_t large_asteroids_current_max_count = 0;
    uint32_t large_asteroids_live_count = 0;

    uint32_t small_asteroids_current_max_count = 0;
    uint32_t small_asteroids_live_count = 0;

    uint32_t bullets_current_max_count = 0;
    uint32_t bullet_live_count = 0;

    uint32_t LARGE_ASTEROID_BATCH_SIZE;
    uint32_t SMALL_ASTEROID_BATCH_SIZE;
    uint32_t BULLET_BATCH_SIZE;

    bool should_update_command_buffers = false;

    RECT window_rect;

    int32_t last_mouse_x;
    int32_t last_mouse_y;

    uint32_t delta_time = 0;

    std::mt19937 generator;

    std::uniform_real_distribution<> large_asteroids_forward_vector_rand;
    std::uniform_real_distribution<> large_asteroids_forward_speed_rand;
    std::uniform_real_distribution<> large_asteroids_rotation_speed_rand;

    std::uniform_real_distribution<> small_asteroids_forward_speed_rand;

    std::uniform_real_distribution<> large_asteroids_output_position_x_rand;
    std::uniform_real_distribution<> large_asteroids_output_position_y_rand;
    std::uniform_real_distribution<> large_asteroids_output_rotation_rand;
};

AGE_RESULT game_reserve_memory_for_asteroids_bullets (game* game_obj);

void* game_create ()
{
    void* alloc = utils_malloc (sizeof (game));
    memset (alloc, 0, sizeof (game));

    return alloc;    
}

AGE_RESULT game_init (game* game_obj, const HINSTANCE h_instance, const HWND h_wnd)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    GetClientRect (h_wnd, &game_obj->window_rect);
    game_obj->window_aspect_ratio = (float)game_obj->window_rect.right / (float)game_obj->window_rect.bottom;

    game_obj->LARGE_ASTEROID_BATCH_SIZE = 50;
    game_obj->SMALL_ASTEROID_BATCH_SIZE = 150;
    game_obj->BULLET_BATCH_SIZE = 20;

    game_obj->player_shooting_interval_msecs = 100;

    game_obj->player_transform_inputs.time_msecs_to_come_to_rest = 500.f;
    game_obj->player_transform_inputs.forward_vector.x = 0;
    game_obj->player_transform_inputs.forward_vector.y = 1;
    game_obj->player_transform_inputs.acceleration = 0.00005f;
    game_obj->player_transform_inputs.deceleration = -0.000025f;
    game_obj->player_transform_inputs.rotation_speed = 0.005f;
    game_obj->player_transform_inputs.max_velocity = 0.05f;

    game_obj->player_output_position.z = 0.5f;
    game_obj->player_output_scale = float2 (0.1f, 0.1f);

    std::random_device rnd_dev;
    game_obj->generator = std::mt19937 (rnd_dev ());

    game_obj->large_asteroids_output_position_x_rand = std::uniform_real_distribution<> (-game_obj->window_aspect_ratio, game_obj->window_aspect_ratio);
    game_obj->large_asteroids_output_position_y_rand = std::uniform_real_distribution<> (-1, 1);
    game_obj->large_asteroids_forward_speed_rand = std::uniform_real_distribution<> (-0.001, 0.001);
    game_obj->large_asteroids_rotation_speed_rand = std::uniform_real_distribution<> (-0.01, 0.01);
    game_obj->large_asteroids_forward_vector_rand = std::uniform_real_distribution<> (-1, 1);
    game_obj->small_asteroids_forward_speed_rand = std::uniform_real_distribution<> (-0.00025, 0.00025);

    game_obj->large_asteroids_output_rotation_rand = std::uniform_real_distribution<> (0, 3.14);

    age_result = game_reserve_memory_for_asteroids_bullets (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = vulkan_interface_init (h_instance, h_wnd);

    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = graphics_init (
        game_obj->large_asteroids_current_max_count, game_obj->large_asteroids_live_count,
        game_obj->small_asteroids_current_max_count, game_obj->small_asteroids_live_count,
        game_obj->bullets_current_max_count, game_obj->bullet_live_count,
        game_obj->window_aspect_ratio
    );

    return age_result;
}

void game_destroy (game* game_obj)
{
    utils_free (game_obj);
}

/*AGE_RESULT game_init (const HINSTANCE h_instance, const HWND h_wnd)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    GetClientRect (h_wnd, &window_rect);
    window_aspect_ratio = (float)window_rect.right / (float)window_rect.bottom;

    game_player_transform_inputs.time_msecs_to_come_to_rest = 500.f;
    game_player_transform_inputs.forward_vector.x = 0;
    game_player_transform_inputs.forward_vector.y = 1;
    game_player_transform_inputs.acceleration = 0.00005f;
    game_player_transform_inputs.deceleration = -0.000025f;
    game_player_transform_inputs.rotation_speed = 0.005f;
    game_player_transform_inputs.max_velocity = 0.05f;

    game_player_output_position.z = 0.5f;
    game_player_output_scale = float2 (0.1f, 0.1f);

    std::random_device rnd_dev;
    generator = std::mt19937 (rnd_dev ());

    game_large_asteroids_output_position_x_rand = std::uniform_real_distribution<> (-window_aspect_ratio, window_aspect_ratio);
    game_large_asteroids_output_position_y_rand = std::uniform_real_distribution<> (-1, 1);
    game_large_asteroids_forward_speed_rand = std::uniform_real_distribution<> (-0.001, 0.001);
    game_large_asteroids_rotation_speed_rand = std::uniform_real_distribution<> (-0.01, 0.01);
    game_large_asteroids_forward_vector_rand = std::uniform_real_distribution<> (-1, 1);
    game_small_asteroids_forward_speed_rand = std::uniform_real_distribution<> (-0.00025, 0.00025);

    game_large_asteroids_output_rotation_rand = std::uniform_real_distribution<> (0, 3.14);

    age_result = game_reserve_memory_for_asteroids_bullets ();
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = vulkan_interface_init (h_instance, h_wnd);

    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = graphics_init (
        game_large_asteroids_current_max_count, game_large_asteroids_live_count, 
        game_small_asteroids_current_max_count, game_small_asteroids_live_count, 
        game_bullets_current_max_count, game_bullet_live_count,
        window_aspect_ratio
    );

    return age_result;
}*/

AGE_RESULT game_reserve_memory_for_asteroids_bullets (game* game_obj)
{
    game_obj->large_asteroids_current_max_count += game_obj->LARGE_ASTEROID_BATCH_SIZE;
    game_obj->large_asteroids_transform_inputs = (asteroid_transform_inputs*)utils_malloc (sizeof (asteroid_transform_inputs) * game_obj->large_asteroids_current_max_count);
    
    game_obj->large_asteroids_outputs_positions = (float3*)utils_malloc (sizeof (float3) * game_obj->large_asteroids_current_max_count);
    game_obj->large_asteroids_outputs_rotations = (float*)utils_malloc (sizeof (float) * game_obj->large_asteroids_current_max_count);
    game_obj->large_asteroids_outputs_scales = (float2*)utils_malloc (sizeof (float2) * game_obj->large_asteroids_current_max_count);
    
    game_obj->small_asteroids_current_max_count += game_obj->SMALL_ASTEROID_BATCH_SIZE;
    game_obj->small_asteroids_transform_inputs = (asteroid_transform_inputs*)utils_malloc (sizeof (asteroid_transform_inputs) * game_obj->small_asteroids_current_max_count);
    
    game_obj->small_asteroids_outputs_positions = (float3*)utils_malloc (sizeof (float3) * game_obj->small_asteroids_current_max_count);
    game_obj->small_asteroids_outputs_rotations = (float*)utils_malloc (sizeof (float) * game_obj->small_asteroids_current_max_count);
    game_obj->small_asteroids_outputs_scales = (float2*)utils_malloc (sizeof (float2) * game_obj->small_asteroids_current_max_count);
    
    game_obj->bullets_current_max_count += game_obj->BULLET_BATCH_SIZE;
    game_obj->bullets_transform_inputs = (bullet_transform_inputs*)utils_malloc (sizeof (bullet_transform_inputs) * game_obj->bullets_current_max_count);
    
    game_obj->bullets_outputs_positions = (float3*)utils_malloc (sizeof (float3) * game_obj->bullets_current_max_count);
    game_obj->bullets_outputs_rotations = (float*)utils_malloc (sizeof (float) * game_obj->bullets_current_max_count);
    game_obj->bullets_outputs_scales = (float2*)utils_malloc (sizeof (float2) * game_obj->bullets_current_max_count);

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT game_large_asteroid_add (game* game_obj, float3 position)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->large_asteroids_live_count == game_obj->large_asteroids_current_max_count)
    {
        game_obj->large_asteroids_current_max_count += game_obj->LARGE_ASTEROID_BATCH_SIZE;

        game_obj->large_asteroids_transform_inputs = (asteroid_transform_inputs*)utils_realloc (game_obj->large_asteroids_transform_inputs, sizeof (asteroid_transform_inputs) * game_obj->large_asteroids_current_max_count);

        game_obj->large_asteroids_outputs_positions = (float3*)utils_realloc (game_obj->large_asteroids_outputs_positions, sizeof (float3) * game_obj->large_asteroids_current_max_count);
        game_obj->large_asteroids_outputs_rotations = (float*)utils_realloc (game_obj->large_asteroids_outputs_rotations, sizeof (float) * game_obj->large_asteroids_current_max_count);
        game_obj->large_asteroids_outputs_scales = (float2*)utils_realloc (game_obj->large_asteroids_outputs_scales, sizeof (float2) * game_obj->large_asteroids_current_max_count);
        
        age_result = graphics_create_transforms_buffer (
            game_obj->large_asteroids_current_max_count,
            game_obj->small_asteroids_current_max_count,
            game_obj->bullets_current_max_count
        );
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    game_obj->large_asteroids_outputs_positions[game_obj->large_asteroids_live_count] = position;
    game_obj->large_asteroids_outputs_rotations[game_obj->large_asteroids_live_count] = (float)game_obj->large_asteroids_output_rotation_rand (game_obj->generator);
    game_obj->large_asteroids_outputs_scales[game_obj->large_asteroids_live_count] = float2 (0.1f, 0.1f);

    game_obj->large_asteroids_transform_inputs[game_obj->large_asteroids_live_count].forward_vector = float2 ((float)game_obj->large_asteroids_forward_vector_rand (game_obj->generator), (float)game_obj->large_asteroids_forward_vector_rand (game_obj->generator));
    game_obj->large_asteroids_transform_inputs[game_obj->large_asteroids_live_count].forward_speed = (float)game_obj->large_asteroids_forward_speed_rand (game_obj->generator);
    game_obj->large_asteroids_transform_inputs[game_obj->large_asteroids_live_count].rotation_speed = (float)game_obj->large_asteroids_rotation_speed_rand (game_obj->generator);

    ++game_obj->large_asteroids_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_small_asteroid_add (game* game_obj, float3 position)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->small_asteroids_live_count == game_obj->small_asteroids_current_max_count)
    {
        game_obj->small_asteroids_current_max_count += game_obj->SMALL_ASTEROID_BATCH_SIZE;

        game_obj->small_asteroids_transform_inputs = (asteroid_transform_inputs*)utils_realloc (game_obj->small_asteroids_transform_inputs, sizeof (asteroid_transform_inputs) * game_obj->small_asteroids_current_max_count);

        game_obj->small_asteroids_outputs_positions = (float3*)utils_realloc (game_obj->small_asteroids_outputs_positions, sizeof (float3) * game_obj->small_asteroids_current_max_count);
        game_obj->small_asteroids_outputs_rotations = (float*)utils_realloc (game_obj->small_asteroids_outputs_rotations, sizeof (float) * game_obj->small_asteroids_current_max_count);
        game_obj->small_asteroids_outputs_scales = (float2*)utils_realloc (game_obj->small_asteroids_outputs_scales, sizeof (float2) * game_obj->small_asteroids_current_max_count);

        age_result = graphics_create_transforms_buffer (
            game_obj->large_asteroids_current_max_count,
            game_obj->small_asteroids_current_max_count,
            game_obj->bullets_current_max_count
        );
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    game_obj->small_asteroids_outputs_positions[game_obj->small_asteroids_live_count] = position;
    game_obj->small_asteroids_outputs_rotations[game_obj->small_asteroids_live_count] = (float)game_obj->large_asteroids_output_rotation_rand (game_obj->generator);
    game_obj->small_asteroids_outputs_scales[game_obj->small_asteroids_live_count] = float2 (0.05f, 0.05f);

    game_obj->small_asteroids_transform_inputs[game_obj->small_asteroids_live_count].forward_vector = float2 ((float)game_obj->large_asteroids_forward_vector_rand (game_obj->generator), (float)game_obj->large_asteroids_forward_vector_rand (game_obj->generator));
    game_obj->small_asteroids_transform_inputs[game_obj->small_asteroids_live_count].forward_speed = (float)game_obj->small_asteroids_forward_speed_rand (game_obj->generator);
    game_obj->small_asteroids_transform_inputs[game_obj->small_asteroids_live_count].rotation_speed = (float)game_obj->large_asteroids_rotation_speed_rand (game_obj->generator);

    ++game_obj->small_asteroids_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_process_left_mouse_click (game* game_obj, const int32_t x, const int32_t y)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;
    
    age_result = game_large_asteroid_add (game_obj, float3 ((float)game_obj->large_asteroids_output_position_x_rand (game_obj->generator), (float)game_obj->large_asteroids_output_position_y_rand (game_obj->generator), 0.5));
    
    return age_result;
}

AGE_RESULT game_large_asteroid_remove (game* game_obj, const uint32_t& index_to_remove)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->large_asteroids_live_count == 0)
    {
        return age_result;
    }

    for (uint32_t a = index_to_remove; a < game_obj->large_asteroids_live_count; ++a)
    {
        game_obj->large_asteroids_transform_inputs[a] = game_obj->large_asteroids_transform_inputs[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->large_asteroids_live_count; ++a)
    {
        game_obj->large_asteroids_outputs_positions[a] = game_obj->large_asteroids_outputs_positions[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->large_asteroids_live_count; ++a)
    {
        game_obj->large_asteroids_outputs_rotations[a] = game_obj->large_asteroids_outputs_rotations[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->large_asteroids_live_count; ++a)
    {
        game_obj->large_asteroids_outputs_scales[a] = game_obj->large_asteroids_outputs_scales[a + 1];
    }

    --game_obj->large_asteroids_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_small_asteroid_remove (game* game_obj, const uint32_t& index_to_remove)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->small_asteroids_live_count == 0)
    {
        return age_result;
    }

    for (uint32_t a = index_to_remove; a < game_obj->small_asteroids_live_count; ++a)
    {
        game_obj->small_asteroids_transform_inputs[a] = game_obj->small_asteroids_transform_inputs[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->small_asteroids_live_count; ++a)
    {
        game_obj->small_asteroids_outputs_positions[a] = game_obj->small_asteroids_outputs_positions[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->small_asteroids_live_count; ++a)
    {
        game_obj->small_asteroids_outputs_rotations[a] = game_obj->small_asteroids_outputs_rotations[a + 1];
    }

    for (uint32_t a = index_to_remove; a < game_obj->small_asteroids_live_count; ++a)
    {
        game_obj->small_asteroids_outputs_scales[a] = game_obj->small_asteroids_outputs_scales[a + 1];
    }

    --game_obj->small_asteroids_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_process_right_mouse_click (game* game_obj, const int32_t x, const int32_t y)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    age_result = game_large_asteroid_remove (game_obj, (uint32_t)(((float)std::rand () / (float)RAND_MAX) * game_obj->large_asteroids_live_count));

    return age_result;
}

AGE_RESULT game_update_player_vectors (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    game_obj->player_transform_inputs.forward_vector.x = -sinf (game_obj->player_transform_inputs.rotation);
    game_obj->player_transform_inputs.forward_vector.y = cosf (game_obj->player_transform_inputs.rotation);

    return age_result;
}

AGE_RESULT game_process_mouse_move (game* game_obj, const int32_t x, const int32_t y)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (x < 0 || y < 0)
    {
        return age_result;
    }

    game_obj->last_mouse_x = x;
    game_obj->last_mouse_y = y;

    return age_result;
}

AGE_RESULT game_player_increase_speed (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (std::hypotf (game_obj->player_transform_inputs.v.x, game_obj->player_transform_inputs.v.y) > game_obj->player_transform_inputs.max_velocity)
    {
        return age_result;
    }

    float2 acceleration = { 
        game_obj->player_transform_inputs.acceleration * game_obj->player_transform_inputs.forward_vector.x,
        game_obj->player_transform_inputs.acceleration * game_obj->player_transform_inputs.forward_vector.y
    };

    game_obj->player_transform_inputs.v.x = game_obj->player_transform_inputs.u.x + (acceleration.x * game_obj->delta_time);
    game_obj->player_transform_inputs.v.y = game_obj->player_transform_inputs.u.y + (acceleration.y * game_obj->delta_time);

    game_obj->player_transform_inputs.u = game_obj->player_transform_inputs.v;

    return age_result;
}

AGE_RESULT game_player_decrease_speed (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (std::hypotf (game_obj->player_transform_inputs.v.x, game_obj->player_transform_inputs.v.y) > game_obj->player_transform_inputs.max_velocity)
    {
        return age_result;
    }

    float2 deceleration = { 
        game_obj->player_transform_inputs.deceleration * game_obj->player_transform_inputs.forward_vector.x,
        game_obj->player_transform_inputs.deceleration * game_obj->player_transform_inputs.forward_vector.y
    };

    game_obj->player_transform_inputs.v.x = game_obj->player_transform_inputs.u.x + (deceleration.x * game_obj->delta_time);
    game_obj->player_transform_inputs.v.y = game_obj->player_transform_inputs.u.y + (deceleration.y * game_obj->delta_time);

    game_obj->player_transform_inputs.u = game_obj->player_transform_inputs.v;

    return age_result;
}

AGE_RESULT game_player_turn_right (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    game_obj->player_transform_inputs.rotation -= (game_obj->player_transform_inputs.rotation_speed * game_obj->delta_time);
    game_obj->player_output_rotation = game_obj->player_transform_inputs.rotation;

    age_result = game_update_player_vectors (game_obj);

    return age_result;
}

AGE_RESULT game_player_turn_left (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;
    
    game_obj->player_transform_inputs.rotation += (game_obj->player_transform_inputs.rotation_speed * game_obj->delta_time);
    game_obj->player_output_rotation = game_obj->player_transform_inputs.rotation;

    age_result = game_update_player_vectors (game_obj);
    
    return age_result;
}

AGE_RESULT game_player_look_at_mouse (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    float2 transformed_mouse_pos = float2 (
        (float)game_obj->last_mouse_x / (float)game_obj->window_rect.right * 2 - 1,
        (float)game_obj->last_mouse_y / (float)game_obj->window_rect.bottom * 2 - 1
    );

    float2 look_at_mouse = float2 (transformed_mouse_pos.x - game_obj->player_output_position.x, transformed_mouse_pos.y - game_obj->player_output_position.y);
    float2_unit_vector (&look_at_mouse);
    game_obj->player_transform_inputs.forward_vector = look_at_mouse;

 // game_player_output_rotation.x = -asinf (game_player_transform_inputs.forward_vector.x);
//  game_player_output_rotation.y = acosf (game_player_transform_inputs.forward_vector.y);

    return age_result;
}

AGE_RESULT game_bullet_add (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->bullet_live_count == game_obj->bullets_current_max_count)
    {
        game_obj->bullets_current_max_count += game_obj->BULLET_BATCH_SIZE;
        game_obj->bullets_transform_inputs = (bullet_transform_inputs*)utils_realloc (game_obj->bullets_transform_inputs, sizeof (bullet_transform_inputs) * game_obj->bullets_current_max_count);

        game_obj->bullets_outputs_positions = (float3*)utils_realloc (game_obj->bullets_outputs_positions, sizeof (float3) * game_obj->bullets_current_max_count);
        game_obj->bullets_outputs_rotations = (float*)utils_realloc (game_obj->bullets_outputs_rotations, sizeof (float) * game_obj->bullets_current_max_count);
        game_obj->bullets_outputs_scales = (float2*)utils_realloc (game_obj->bullets_outputs_scales, sizeof (float2) * game_obj->bullets_current_max_count);

        age_result = graphics_create_transforms_buffer (
            game_obj->large_asteroids_current_max_count, 
            game_obj->small_asteroids_current_max_count, 
            game_obj->bullets_current_max_count
        );
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    game_obj->bullets_transform_inputs[game_obj->bullet_live_count].forward_vector = game_obj->player_transform_inputs.forward_vector;
    game_obj->bullets_transform_inputs[game_obj->bullet_live_count].speed = (float2_length (&game_obj->player_transform_inputs.v) / (float)game_obj->delta_time) + 0.005f;
    
    game_obj->bullets_outputs_positions[game_obj->bullet_live_count] = game_obj->player_output_position;
    game_obj->bullets_outputs_rotations[game_obj->bullet_live_count] = game_obj->player_output_rotation;
    game_obj->bullets_outputs_scales[game_obj->bullet_live_count] = float2 (0.05f, 0.05f);

    ++game_obj->bullet_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_bullet_remove (game* game_obj, const uint32_t& index_to_remove)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->bullet_live_count == 0)
    {
        return age_result;
    }

    for (uint32_t b = index_to_remove; b < game_obj->bullet_live_count; ++b)
    {
        game_obj->bullets_transform_inputs[b] = game_obj->bullets_transform_inputs[b + 1];
    }
    
    for (uint32_t b = index_to_remove; b < game_obj->bullet_live_count; ++b)
    {
        game_obj->bullets_outputs_positions[b] = game_obj->bullets_outputs_positions[b + 1];
    }

    for (uint32_t b = index_to_remove; b < game_obj->bullet_live_count; ++b)
    {
        game_obj->bullets_outputs_rotations[b] = game_obj->bullets_outputs_rotations[b + 1];
    }

    for (uint32_t b = index_to_remove; b < game_obj->bullet_live_count; ++b)
    {
        game_obj->bullets_outputs_scales[b] = game_obj->bullets_outputs_scales[b + 1];
    }

    --game_obj->bullet_live_count;

    game_obj->should_update_command_buffers = true;

    return age_result;
}

AGE_RESULT game_player_attempt_to_shoot (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->secs_since_last_shot > game_obj->player_shooting_interval_msecs)
    {
        age_result = game_bullet_add (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }

        game_obj->secs_since_last_shot = 0;
    }

    return age_result;
}

/*AGE_RESULT game_bullets_check_life ()
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    for (uint32_t b = 0; b < game_bullet_live_count; ++b)
    {
        game_bullets_current_lifetimes_msecs[b] += game_delta_time;
    }

    for (uint32_t b = 0; b < game_bullet_live_count; ++b)
    {
        if (game_bullets_current_lifetimes_msecs[b] > game_bullets_max_lifetime_msecs)
        {
            age_result = game_bullet_remove (b);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                goto exit;
            }
        }
    }

exit:
    return age_result;
}*/

AGE_RESULT game_process_key_down (game* game_obj, const WPARAM w_param)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;
    
    switch (w_param) 
    {
        case 0x57: // w
        game_obj->is_w_pressed = true;
        break;

        case 0x53: // s
            game_obj->is_s_pressed = true;
        break;

        case 0x44: // d
            game_obj->is_d_pressed = true;
        break;
        
        case 0x41: // a
            game_obj->is_a_pressed = true;
        break;

        case 0x26: // up arrow
            game_obj->is_up_arrow_pressed = true;
        break;

        case 0x28: // down arrow
            game_obj->is_down_arrow_pressed = true;
        break;

        case 0x27: // right arrow
            game_obj->is_right_arrow_pressed = true;
        break;

        case 0x25: // left arrow
            game_obj->is_left_arrow_pressed = true;
        break;

        case 0x20: // space
            game_obj->is_space_bar_pressed = true;
        break;

        default:
        break;
    }

    return age_result;
}

AGE_RESULT game_process_key_up (game* game_obj, const WPARAM w_param)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    switch (w_param) 
    {
        case 0x57: // w
            game_obj->is_w_pressed = false;
        break;

        case 0x53: // s
            game_obj->is_s_pressed = false;
        break;

        case 0x44: // d
            game_obj->is_d_pressed = false;
        break;
        
        case 0x41: // a
            game_obj->is_a_pressed = false;
        break;

        case 0x26: // up arrow
            game_obj->is_up_arrow_pressed = false;
        break;

        case 0x28: // down arrow
            game_obj->is_down_arrow_pressed = false;
        break;

        case 0x27: // right arrow
            game_obj->is_right_arrow_pressed = false;
        break;

        case 0x25: // left arrow
            game_obj->is_left_arrow_pressed = false;
        break;

        case 0x20: // space
            game_obj->is_space_bar_pressed = false;
        break;

        default:
        break;
    }

    return age_result;
}

AGE_RESULT game_update_player_asteroids_bullets_output_positions (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    game_obj->player_output_position.x += game_obj->player_transform_inputs.v.x;
    game_obj->player_output_position.y += game_obj->player_transform_inputs.v.y;

    if (game_obj->player_output_position.x > game_obj->window_aspect_ratio)
    {
        game_obj->player_output_position.x = -game_obj->window_aspect_ratio;
    }
    
    if (game_obj->player_output_position.x < -game_obj->window_aspect_ratio)
    {
        game_obj->player_output_position.x = game_obj->window_aspect_ratio;
    }

    if (game_obj->player_output_position.y > 1.f)
    {
        game_obj->player_output_position.y = -1.f;
    }
    
    if (game_obj->player_output_position.y < -1.f)
    {
        game_obj->player_output_position.y = 1.f;
    }

    for (uint32_t a = 0; a < game_obj->large_asteroids_live_count; ++a)
    {
        game_obj->large_asteroids_outputs_positions[a].x += (game_obj->large_asteroids_transform_inputs[a].forward_vector.x * game_obj->large_asteroids_transform_inputs[a].forward_speed * game_obj->delta_time);
        game_obj->large_asteroids_outputs_positions[a].y += (game_obj->large_asteroids_transform_inputs[a].forward_vector.y * game_obj->large_asteroids_transform_inputs[a].forward_speed * game_obj->delta_time);

        if (game_obj->large_asteroids_outputs_positions[a].x > game_obj->window_aspect_ratio)
        {
            game_obj->large_asteroids_outputs_positions[a].x = -game_obj->window_aspect_ratio;
        }
        else if (game_obj->large_asteroids_outputs_positions[a].x < -game_obj->window_aspect_ratio)
        {
            game_obj->large_asteroids_outputs_positions[a].x = game_obj->window_aspect_ratio;
        }
        else if (game_obj->large_asteroids_outputs_positions[a].y > 1.f)
        {
            game_obj->large_asteroids_outputs_positions[a].y = -1.f;
        }
        else if (game_obj->large_asteroids_outputs_positions[a].y < -1.f)
        {
            game_obj->large_asteroids_outputs_positions[a].y = 1.f;
        }

        game_obj->large_asteroids_outputs_rotations[a] += (game_obj->large_asteroids_transform_inputs[a].rotation_speed) * game_obj->delta_time;
    }

    for (uint32_t a = 0; a < game_obj->small_asteroids_live_count; ++a)
    {
        game_obj->small_asteroids_outputs_positions[a].x += (game_obj->small_asteroids_transform_inputs[a].forward_vector.x * game_obj->small_asteroids_transform_inputs[a].forward_speed * game_obj->delta_time);
        game_obj->small_asteroids_outputs_positions[a].y += (game_obj->small_asteroids_transform_inputs[a].forward_vector.y * game_obj->small_asteroids_transform_inputs[a].forward_speed * game_obj->delta_time);

        if (game_obj->small_asteroids_outputs_positions[a].x > game_obj->window_aspect_ratio)
        {
            game_obj->small_asteroids_outputs_positions[a].x = -game_obj->window_aspect_ratio;
        }
        else if (game_obj->small_asteroids_outputs_positions[a].x < -game_obj->window_aspect_ratio)
        {
            game_obj->small_asteroids_outputs_positions[a].x = game_obj->window_aspect_ratio;
        }
        else if (game_obj->small_asteroids_outputs_positions[a].y > 1.f)
        {
            game_obj->small_asteroids_outputs_positions[a].y = -1.f;
        }
        else if (game_obj->small_asteroids_outputs_positions[a].y < -1.f)
        {
            game_obj->small_asteroids_outputs_positions[a].y = 1.f;
        }

        game_obj->small_asteroids_outputs_rotations[a] += (game_obj->small_asteroids_transform_inputs[a].rotation_speed) * game_obj->delta_time;
    }

    for (uint32_t b = 0; b < game_obj->bullet_live_count; ++b)
    {
        game_obj->bullets_outputs_positions[b].x += (game_obj->bullets_transform_inputs[b].forward_vector.x * (game_obj->bullets_transform_inputs[b].speed * game_obj->delta_time));
        game_obj->bullets_outputs_positions[b].y += (game_obj->bullets_transform_inputs[b].forward_vector.y * (game_obj->bullets_transform_inputs[b].speed * game_obj->delta_time));

        if (game_obj->bullets_outputs_positions[b].x < -game_obj->window_aspect_ratio)
        {
            age_result = game_bullet_remove (game_obj, b);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                return age_result;
            }

            break;
        }
        else if (game_obj->bullets_outputs_positions[b].x > game_obj->window_aspect_ratio)
        {
            age_result = game_bullet_remove (game_obj, b);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                return age_result;
            }

            break;
        }
        else if (game_obj->bullets_outputs_positions[b].y < -1)
        {
            age_result = game_bullet_remove (game_obj, b);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                return age_result;
            }

            break;
        }
        else if (game_obj->bullets_outputs_positions[b].y > 1)
        {
            age_result = game_bullet_remove (game_obj, b);
            if (age_result != AGE_RESULT::SUCCESS)
            {
                return age_result;
            }

            break;
        }
    }

    return age_result;
}

AGE_RESULT game_player_apply_damping (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    float damping_value = 1;

    if (game_obj->delta_time < game_obj->player_transform_inputs.time_msecs_to_come_to_rest)
    {
        damping_value = (1 - (game_obj->delta_time / game_obj->player_transform_inputs.time_msecs_to_come_to_rest));
    }

    game_obj->player_transform_inputs.v.x *= damping_value;
    game_obj->player_transform_inputs.v.y *= damping_value;

    game_obj->player_transform_inputs.u.x *= damping_value;
    game_obj->player_transform_inputs.u.y *= damping_value;

    return age_result;
}

AGE_RESULT game_process_player_input (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    if (game_obj->is_w_pressed)
    {
        age_result = game_player_increase_speed (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_up_arrow_pressed)
    {
        age_result = game_player_increase_speed (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_s_pressed)
    {
        age_result = game_player_decrease_speed (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_down_arrow_pressed)
    {
        age_result = game_player_decrease_speed (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_d_pressed)
    {
        age_result = game_player_turn_right (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_right_arrow_pressed)
    {
        age_result = game_player_turn_right (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_a_pressed)
    {
        age_result = game_player_turn_left (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_left_arrow_pressed)
    {
        age_result = game_player_turn_left (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    if (game_obj->is_space_bar_pressed)
    {
        age_result = game_player_attempt_to_shoot (game_obj);
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }
    }

    return age_result;
}

AGE_RESULT game_bullets_large_asteroids_collision_checks (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    for (uint32_t b = 0; b < game_obj->bullet_live_count; ++b)
    {
        for (uint32_t a = 0; a < game_obj->large_asteroids_live_count; ++a)
        {
            float2 diff = {
                game_obj->large_asteroids_outputs_positions[a].x - game_obj->bullets_outputs_positions[b].x,
                game_obj->large_asteroids_outputs_positions[a].y - game_obj->bullets_outputs_positions[b].y,
            };

            if (hypotf (diff.x, diff.y) < 0.1f)
            {
                age_result = game_bullet_remove (game_obj, b);
                if (age_result != AGE_RESULT::SUCCESS)
                {
                    return age_result;
                }

                float3 position = game_obj->large_asteroids_outputs_positions[a];

                age_result = game_large_asteroid_remove (game_obj, a);
                if (age_result != AGE_RESULT::SUCCESS)
                {
                    return age_result;
                }

                for (uint32_t i = 0; i < 3; ++i)
                {
                    age_result = game_small_asteroid_add (game_obj, position);
                    if (age_result != AGE_RESULT::SUCCESS)
                    {
                        return age_result;
                    }
                }

                break;
            }
        }
    }

    return age_result;
}

AGE_RESULT game_bullets_small_asteroids_collision_checks (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    for (uint32_t b = 0; b < game_obj->bullet_live_count; ++b)
    {
        for (uint32_t a = 0; a < game_obj->small_asteroids_live_count; ++a)
        {
            float2 diff = {
                game_obj->small_asteroids_outputs_positions[a].x - game_obj->bullets_outputs_positions[b].x,
                game_obj->small_asteroids_outputs_positions[a].y - game_obj->bullets_outputs_positions[b].y,
            };

            if (hypotf (diff.x, diff.y) < 0.05f)
            {
                age_result = game_bullet_remove (game_obj, b);
                if (age_result != AGE_RESULT::SUCCESS)
                {
                    return age_result;
                }

                age_result = game_small_asteroid_remove (game_obj, a);
                if (age_result != AGE_RESULT::SUCCESS)
                {
                    return age_result;
                }

                break;
            }
        }
    }

    return age_result;
}

AGE_RESULT game_update (game* game_obj, uint32_t delta_msecs)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    game_obj->delta_time = delta_msecs;

    game_obj->secs_since_last_shot += game_obj->delta_time;;

    age_result = game_process_player_input (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = game_update_player_asteroids_bullets_output_positions (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = game_player_apply_damping (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = game_bullets_large_asteroids_collision_checks (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = game_bullets_small_asteroids_collision_checks (game_obj);
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    age_result = graphics_update_transforms_buffer_data (
        &game_obj->player_output_position,
        &game_obj->player_output_rotation,
        &game_obj->player_output_scale,
        game_obj->large_asteroids_outputs_positions,
        game_obj->large_asteroids_outputs_rotations,
        game_obj->large_asteroids_outputs_scales,
        game_obj->large_asteroids_live_count,
        game_obj->large_asteroids_current_max_count,
        game_obj->small_asteroids_outputs_positions,
        game_obj->small_asteroids_outputs_rotations,
        game_obj->small_asteroids_outputs_scales,
        game_obj->small_asteroids_live_count,
        game_obj->small_asteroids_current_max_count,
        game_obj->bullets_outputs_positions,
        game_obj->bullets_outputs_rotations,
        game_obj->bullets_outputs_scales,
        game_obj->bullet_live_count,
        game_obj->bullets_current_max_count,
        game_obj->window_aspect_ratio
    );

    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    if (game_obj->should_update_command_buffers)
    {
        age_result = graphics_update_command_buffers (
            game_obj->large_asteroids_live_count,
            game_obj->small_asteroids_live_count,
            game_obj->bullet_live_count,
            game_obj->window_aspect_ratio
        );
        if (age_result != AGE_RESULT::SUCCESS)
        {
            return age_result;
        }

        game_obj->should_update_command_buffers = false;
    }

    return age_result;
}

AGE_RESULT game_submit_present (game* game_obj)
{
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    age_result = graphics_submit_present ();

    return age_result;
}

void game_shutdown (game* game_obj)
{
    graphics_shutdown ();
    vulkan_interface_shutdown ();
    
    utils_free (game_obj->large_asteroids_transform_inputs);

    utils_free (game_obj->large_asteroids_outputs_positions);
    utils_free (game_obj->large_asteroids_outputs_rotations);
    utils_free (game_obj->large_asteroids_outputs_scales);

    utils_free (game_obj->small_asteroids_transform_inputs);

    utils_free (game_obj->small_asteroids_outputs_positions);
    utils_free (game_obj->small_asteroids_outputs_rotations);
    utils_free (game_obj->small_asteroids_outputs_scales);

    utils_free (game_obj->bullets_transform_inputs);

    utils_free (game_obj->bullets_outputs_positions);
    utils_free (game_obj->bullets_outputs_rotations);
    utils_free (game_obj->bullets_outputs_scales);
}