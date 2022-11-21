#include <iostream>
#include <GL/glew.h>
#include "ShaderMaker.h"
#include "lib.h"
#include <GL/freeglut.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include "Hermite.h"
#include "Entities.h"
#include "Drawer.h"

static unsigned int programId;
int score = 0;
double timer = 0;
mat4 target;
int wWidth = 1200;
int wHeight = 600;
float waterImpact = 0;
vec2 mouse;
GLuint MatProj, MatModel;
float w_update, h_update;
Figura background, sun, fish, hook, lenza;
vector<Figura> world;


void INIT_VAO(void)
{
	//Creo lo sfondo
	vec4 col_top = vec4{ 1.0, 0.0, 1.0, 1.0};
	vec4 col_bottom = vec4{ 0.0, 0.0, 1.0, 1.0};
	createBackground(col_top, col_bottom, &background, wWidth, wHeight);
	createVAO(&background);
	world.push_back(background);

	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	glViewport(0, 0, wWidth, wHeight);
	target = ortho(0.0f, float(wWidth), 0.0f, float(wHeight));

	//Creo il sole

	col_top = vec4{ 1.0, 1.0, 1.0, 1.0 };
	col_bottom = vec4{ 1.0, 1.0, 1.0, 1.0 };
	float posx = (float)wWidth / 2;
	float posy = (float)wHeight - (wHeight / 2);
	createSun(col_top, col_bottom, posx, posy, &sun);
	sun.posx = posx;
	sun.posy = posy;
	createVAO(&sun);
	world.push_back(sun);

	//Creo l'amo

	col_top = vec4{ 0.8, 0.8, 0.8, 1.0 };
	col_bottom = vec4{ 1.0, 1.0, 0.0, 1.0 };
	createHook(col_top, col_bottom, &hook);
	hook.posx = posx;
	hook.posy = posy;
	createVAO(&hook);
	world.push_back(hook);

	//Creo i pesci
	for (int i = 0; i < 20; i++) {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		col_top = vec4{ r, g, b, 1.0 };
		col_bottom = vec4{ 1.0, 1.0, 0.0, 1.0 };
		createFish(col_top, col_bottom, &fish);
		fish.posx = 0.0f;
		fish.posy = posy;
		fish.speed = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + 1.0f) * 2.0f;
		createVAO(&fish);
		world.push_back(fish);
	}
}

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

void renderScene(void)
{
	glUniform1i(glGetUniformLocation(programId, "sceltaVS"), 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(target));
	float time = glutGet(GLUT_ELAPSED_TIME) / 1500.0f;
	vec2 resolution = vec2(wWidth, wHeight);
	glUniform1f(glGetUniformLocation(programId, "iTime"), time);
	glUniform2f(glGetUniformLocation(programId,"resolution"), resolution.x, resolution.y);

	world = drawBG(MatModel, programId, world);

	glUniform2f(glGetUniformLocation(programId, "sunPos"), 0, 0);
	glUniform2f(glGetUniformLocation(programId, "mouse"), mouse.x * wWidth, mouse.y * wHeight);
	glUniform1f(glGetUniformLocation(programId, "waterImpact"), waterImpact);
	waterImpact = waterImpact / 1.1f;

	world = drawSun(MatModel, programId, world);

	world = drawHook(MatModel, programId, world, mouse, wWidth, wHeight, w_update, h_update);

	world = drawFish(MatModel, programId, world);

	glutSwapBuffers();
}

void reshapeFunc(int w, int h)
{
	float Ratio = (float)(16) / (float)(8);

	if (Ratio > w / h) {
		w_update = (float)w;
		h_update = w / Ratio;
		glViewport(0, 0, w_update, h_update);
	}
	else {
		w_update = h * Ratio;
		h_update = (float)h;
		glViewport(0, 0, w_update, h_update);
	}
}


void myKeyboard(unsigned char key, int x, int y)
{
	{
		switch (key)
		{
		case ' ':
			for (int i = 3; i < 23; i++) {
				if (wHeight - y < world[i].corner_b.y + 30.0
					&& wHeight - y > world[i].corner_b.y - 30.0
					&& x < world[i].corner_b.x + 30.0
					&& x > world[i].corner_b.x - 30.0) {
					world[i].caught = true;
				}
			}
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}

void passiveMouseFunc(int x, int y)
{
	vec2 resolution = vec2(wWidth, wHeight);
	float uvx = x / resolution.x;
	float uvy = y / resolution.y;
	float time = glutGet(GLUT_ELAPSED_TIME) / 1500.0f;
	float thresh = (sin(((sin(uvx) + 0.5) * 5.0) + time) / (20.0)) - 0.05;

	if ((1.0 - uvy) < (0.25 - thresh) && (1.0 - uvy) > (0.20 - thresh)) {
		waterImpact = 1.0;
	}
	
	mouse.x = uvx;
	mouse.y = uvy;
}

void updateFunc(int value)
{
	timer++;
	world[1].posx = -cos(timer/1000.0f) * 400.0f + (wWidth/2.0f);
	world[1].posy = sin(timer / 1000.0f) * 400.0f;

	for (int i = 3; i < 23; i++) {
		if (world[i].caught) {
			world[i].posy += world[i].speed + 2.0f;
			world[i].speed -= 0.1f;
		}
		else {
			world[i].posx += world[i].dir * world[i].speed;
			world[i].posy = sin((timer) / (20.0f * world[i].speed) + (10.0 * world[i].speed)) * 50.0f + (wHeight / 6.0f);
			if (world[i].posx > wWidth) {
				world[i].dir = -1.0;
			}
			else if (world[i].posx < 0) {
				world[i].dir = 1.0;
			}
		}
	}

	glUniform2f(glGetUniformLocation(programId, "sunPos"), world[1].posx / wWidth, world[1].posy / wHeight);
	glutPostRedisplay();
	glutTimerFunc(1, updateFunc, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	srand(time(NULL));
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(wWidth, wHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Zen Fishing");
	glutKeyboardFunc(myKeyboard);
	glutPassiveMotionFunc(passiveMouseFunc);
	glutReshapeFunc(reshapeFunc);
	glutTimerFunc(1, updateFunc, 0);
	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glutDisplayFunc(renderScene);

	glutMainLoop();
}