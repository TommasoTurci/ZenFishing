#pragma once

vector<Figura> drawBG(GLuint MatModel, unsigned int programId, vector<Figura> world);

vector<Figura> drawSun(GLuint MatModel, unsigned int programId, vector<Figura> world);

vector<Figura> drawFish(GLuint MatModel, unsigned int programId, vector<Figura> world);

vector<Figura> drawHook(GLuint MatModel, unsigned int programId, vector<Figura> world, vec2 mouse, int wWidth, int wHeight, float w_update, float h_update);