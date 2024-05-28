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

constexpr float GRAVITY = -0.98f;

constexpr float CURSOR_RADIUS = 0.5f;

constexpr float ATTRACTION_STRENGTH = 1000.0f;
constexpr float REPULSION_STRENGTH = -2000.0f;

//TODOs: 
// - Add new objects to the simulation
// - Fix the resizing of the window
// - Fix coordenate system
// - Add a position variable to the container
// - Add Links between particles
// - Batch rendering