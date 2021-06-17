#pragma once

const int NONE = 0x0;
const int PLAYER_CATEGORY_BITMASK = 0x1;
const int WALL_CATEGORY_BITMASK = 0x2;
const int KEY_CATEGORY_BITMASK = 0x4;
const int GOAL_CATEGORY_BITMASK = 0x8;
const int LASER_CATEGORY_BITMASK = 0X16;


const int PLAYER_COLLISION_BITMASK = WALL_CATEGORY_BITMASK;
const int WALL_COLLISION_BITMASK = PLAYER_CATEGORY_BITMASK;
const int KEY_COLLISION_BITMASK = NONE;
const int GOAL_COLLISION_BITMASK = NONE;
const int LASER_COLLISION_BITMASK = NONE;

const int PLAYER_CONTACT_TEST_BITMASK = KEY_CATEGORY_BITMASK | GOAL_CATEGORY_BITMASK;
const int WALL_CONTACT_TEST_BITMASK = NONE;
const int KEY_CONTACT_TEST_BITMASK = PLAYER_CATEGORY_BITMASK;
const int GOAL_CONTACT_TEST_BITMASK = PLAYER_CATEGORY_BITMASK;
const int LASER_CONTACT_TEST_BITMASK = PLAYER_CATEGORY_BITMASK;