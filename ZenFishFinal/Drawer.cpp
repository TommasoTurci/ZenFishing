#include "Lib.h"

vector<Figura> drawBG(GLuint MatModel, unsigned int programId, vector<Figura> world) {
	
	//Disegno lo sfondo;

	glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 1);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(world[0].Model));
	glBindVertexArray(world[0].VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, world[0].nv);
	glBindVertexArray(0);

	return world;
}

vector<Figura> drawSun(GLuint MatModel, unsigned int programId, vector<Figura> world) {

	//Disegno il sole;

	glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 0);
	world[1].Model = mat4(1.0);
	world[1].Model = translate(world[1].Model, vec3((float)world[1].posx, (float)world[1].posy, 0.0f));
	world[1].Model = scale(world[1].Model, vec3(5, 5, 1.0));
	world[1].corner_b = world[1].corner_b_obj;
	world[1].corner_t = world[1].corner_t_obj;
	world[1].corner_b = world[1].Model * world[1].corner_b;
	world[1].corner_t = world[1].Model * world[1].corner_t;
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(world[1].Model));
	glBindVertexArray(world[1].VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, world[1].vertici.size());
	glBindVertexArray(0);

	return world;
}

vector<Figura> drawFish(GLuint MatModel, unsigned int programId, vector<Figura> world) {

	//Disegno i pesci;

	glUniform1i(glGetUniformLocation(programId, "sceltaVS"), 1);
	for (int i = 3; i < 23; i++) {
		glUniform1f(glGetUniformLocation(programId, "fishDir"), world[i].dir);
		glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 2);
		world[i].Model = mat4(1.0);
		world[i].Model = translate(world[i].Model, vec3((float)world[i].posx, (float)world[i].posy, 0.0f));
		world[i].Model = scale(world[i].Model, vec3(2, 2, 1.0));
		world[i].corner_b = world[i].corner_b_obj;
		world[i].corner_t = world[i].corner_t_obj;
		world[i].corner_b = world[i].Model * world[i].corner_b;
		world[i].corner_t = world[i].Model * world[i].corner_t;
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(world[i].Model));
		glBindVertexArray(world[i].VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, world[i].vertici.size());
		glBindVertexArray(0);
	}

	return world;
}

vector<Figura> drawHook(GLuint MatModel, unsigned int programId, vector<Figura> world, vec2 mouse, int wWidth, int wHeight, float w_update, float h_update) {

	//Disegno l'amo;

	glUniform1i(glGetUniformLocation(programId, "sceltaVS"), 0);
	glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 2);
	world[2].Model = mat4(1.0);
	world[2].Model = translate(world[2].Model, vec3(mouse.x * w_update, (-mouse.y * h_update) + wHeight, 0.0f));
	world[2].Model = scale(world[2].Model, vec3(10, 10, 1.0));
	world[2].corner_b = world[2].corner_b_obj;
	world[2].corner_t = world[2].corner_t_obj;
	world[2].corner_b = world[2].Model * world[2].corner_b;
	world[2].corner_t = world[2].Model * world[2].corner_t;
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(world[2].Model));
	glBindVertexArray(world[2].VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, world[2].vertici.size());
	glBindVertexArray(0);

	return world;
}