#include "Renderer.h"

void enableBlending(bool enable) {
	if (enable) {
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	else {
		GLCall(glDisable(GL_BLEND));
	}
}

void clear(float r, float g, float b, float a){
	GLCall(glClearColor(r,g,b,a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void drawParticle(VAO& vao, EBO& ebo, ShaderProgram& program, const std::array<float, 2>& position, const std::array<float, 2>& velocity, float radius, int screen_width, int screen_height) {

	float abs_velocity = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);

	program.Use();
	vao.Bind();
	ebo.Bind();

	program.setUniform1f("velocity", abs_velocity);
	program.setUniform2f("position", position[0], position[1]);
	program.setUniform1f("radius", radius);
	program.setUniform2i("resolution", screen_width, screen_height);


	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}


void drawCursor(VAO& vao, EBO& ebo, ShaderProgram& program, const std::array<float, 2>& position, float radius, bool left_clicked, bool right_clicked, float time, int screen_width, int screen_height) {
	program.Use();
	vao.Bind();
	ebo.Bind();

	program.setUniform1f("cursor_radius", radius);
	program.setUniform2f("cursor_pos", position[0], screen_height - position[1]);
	program.setUniform1i("left_clicked", left_clicked);
	program.setUniform1i("right_clicked", right_clicked);
	program.setUniform1f("time", time);
	program.setUniform2i("resolution", screen_width, screen_height);


	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}
