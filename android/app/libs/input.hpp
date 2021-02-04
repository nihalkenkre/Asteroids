//
// Created by nihal on 03-02-2021.
//

#ifndef INPUT_HPP
#define INPUT_HPP

struct joystick_input
{
    float input_move_forward_back; // forward = 1, back = -1
    float input_turn_right_left; // right = -1, left = 1
};

struct trigger_input
{
    bool fire;
};

#endif