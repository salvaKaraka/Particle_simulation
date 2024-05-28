#pragma once

#include <glad/glad.h>
#include <utils/SKMath.h>


#include "ErrorChecker.h"
#include "ShaderProgram.h"
#include "VAO.h"
#include "EBO.h"

void enableBlending(bool enable);
void clear(float r, float g, float b, float a);

void drawParticle(VAO& vao, EBO& ebo, ShaderProgram& program, Vec2& position, float& velocity, float& radius, int& screen_width, int& screen_height);
void drawCursor(VAO& vao, EBO& ebo, ShaderProgram& program, Vec2& position, float radius, bool& left_clicked, bool& right_clicked, float& time, int& screen_width, int& screen_height);
void drawContainer(VAO& vao, EBO& ebo, ShaderProgram& program, Vec2& position, Vec2& velocity, float& radius, int& screen_width, int& screen_height);