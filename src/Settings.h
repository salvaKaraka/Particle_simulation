#pragma once

// Settings
constexpr unsigned int SCR_WIDTH = 1000;
constexpr unsigned int SCR_HEIGHT = 1000;

constexpr float BG_R = 0.11f;
constexpr float BG_G = 0.11f;
constexpr float BG_B = 0.11f;


constexpr float RADIUS = 0.03f;
constexpr float PI = 3.14159265359f;
constexpr unsigned int STEPS = 10;
constexpr float ANGLE = PI * 2.f / STEPS;

constexpr int NUM_PARTICLES = 100;

constexpr unsigned int SUB_STEPS = 8;

constexpr float GRAVITY = -1.9f;
constexpr float BORDER_COLLISION_DAMPING = .9f;
constexpr float PARTICLE_COLLISION_DAMPING = .98f;

constexpr float CURSOR_RADIUS = 0.5f;

constexpr float ATTRACTION_STRENGTH = 100.0f;
constexpr float REPULSION_STRENGTH = -200.0f;

//TODOs: 
// - Add new objects to the simulation
// - Fix the resizing of the window

