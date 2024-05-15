#include "Particle.h"
#include <math.h>
#include <glad/glad.h>
#include <vector>

const int steps = 24;
const float angle = 3.1415926 * 2.f / steps;

Particle::Particle(std::array<float, 2> p, float m, std::array<float, 3> c, std::array<float, 2> v, std::array<float, 2> a) {
    pos = p;
    nextPos = p;
    mass = m;
    color = c;
    vel = v;
    acc = a;
    radius = mass / 300;
}

void Particle::setPosition(std::array<float, 2> p) {
    pos = p;
}

void Particle::setNextPosition(std::array<float, 2> np) {
    nextPos = np;
}

void Particle::setXPosition(float x) {
    pos[0] = x;
}

void Particle::setYPosition(float y) {
    pos[1] = y;
}

void Particle::setVelocity(std::array<float, 2> v) {
    vel = v;
}

void Particle::setXVelocity(float vx) {
    vel[0] = vx;
}

void Particle::setYVelocity(float vy) {
    vel[1] = vy;
}

void Particle::setAcceleration(std::array<float, 2> a) {
    acc = a;
}

void Particle::setXAcceleration(float ax) {
    acc[0] = ax;
}

void Particle::setYAcceleration(float ay) {
    acc[1] = ay;
}

void Particle::setColor(std::array<float, 3> c) {
    color = c;
}

void Particle::setMass(float m) {
    mass = m;
}

std::array<float, 2> Particle::getPosition() {
    return pos;
}

std::array<float, 2> Particle::getNextPosition() {
    return nextPos;
}

float Particle::getXPosition() {
    return pos[0];
}

float Particle::getYPosition() {
    return pos[1];
}

std::array<float, 2> Particle::getVelocity() {
    return vel;
}

float Particle::getXVelocity() {
    return vel[0];
}

float Particle::getYVelocity() {
    return vel[1];
}

std::array<float, 2> Particle::getAcceleration() {
    return acc;
}

float Particle::getXAcceleration() {
    return acc[0];
}

float Particle::getYAcceleration() {
    return acc[1];
}

std::array<float, 3> Particle::getColor() {
    return color;
}

float Particle::getMass() {
    return mass;
}

float Particle::getRadius() {
    return radius;
}

void Particle::draw() {
    // Definir los vértices del cuadrado
    std::vector<float> vertices;
    for (int i = 0; i < steps; ++i) {
        float x = pos[0] + radius * cos(angle * i);
        float y = pos[1] + radius * sin(angle * i);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    // Crear y configurar el VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Crear y configurar el VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Compilar shaders
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "}";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main() {\n"
        "    FragColor = vec4(color, 1.0);\n"
        "}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Pasar el color al shader
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorLoc, 1, color.data());

    // Dibujar el círculo
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);

    // Limpiar estado
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
