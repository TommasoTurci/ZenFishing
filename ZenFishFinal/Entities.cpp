#include "Hermite.h"

#define  PI   3.14159265358979323846

void createVAO(Figura* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3), fig->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void createBackground(vec4 color_top, vec4 color_bot, Figura* background, int width, int height) 
{
	background->vertici.push_back(vec3(0.0, 0.0, 0.0));
	background->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	background->vertici.push_back(vec3(0.0, 1.0, 0.0));
	background->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	background->vertici.push_back(vec3(1.0, 0.0, 0.0));
	background->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	background->vertici.push_back(vec3(1.0, 1.0, 0.0));
	background->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
	background->Model = mat4(1.0);
	background->Model = scale(background->Model, vec3(float(width), float(height), 1.0));
	background->nv = background->vertici.size();
	background->sceltaFs = 1;
}

void createSun(vec4 color_top, vec4 color_bot, float posx, float posy, Figura* sun)
{
	sun->nTriangles = 20;
	int i;
	float stepA = (2 * PI) / sun->nTriangles;
	float t;
	sun->vertici.push_back(vec3(0.0, 0.0, 0.0));
	sun->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	sun->posx = posx;
	sun->posy = posy;

	for (i = 0; i <= sun->nTriangles; i++)
	{
		t = (float)i * stepA;
		sun->vertici.push_back(vec3(cos(t)*5, sin(t)*5, 0.0));
		//Colore 
		if (i % 3 == 0) {
			sun->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
		}
		else {
			sun->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
		}
	}
	sun->nv = sun->vertici.size();

	float xmax = 0.0;
	float xmin = 0.0;
	float ymax = 0.0;
	float ymin = 0.0;
	for (i = 1; i < sun->nv; i++) {
		if (sun->vertici[i].x < xmin)
			xmin = sun->vertici[i].x;
	}
	for (i = 1; i < sun->nv; i++) {
		if (sun->vertici[i].x > xmax)
			xmax = sun->vertici[i].x;
	}
	for (i = 1; i < sun->nv; i++) {
		if (sun->vertici[i].y <= ymin)
			ymin = sun->vertici[i].y;
	}
	for (i = 1; i < sun->nv; i++) {
		if (sun->vertici[i].y > ymax)
			ymax = sun->vertici[i].y;
	}
	//Aggiorno i valori del corner pi� in basso a sinistra (corner_b) e del corner pi� in alto a destra (conrner_t)
	sun->corner_b_obj = vec4(xmin, ymin, 0.0, 1.0);
	sun->corner_t_obj = vec4(xmax, ymax, 0.0, 1.0);
}

void createFish(vec4 color_top, vec4 color_bot, Figura* fish) {
	Hermite herm;
	herm.costruisci_curva(-3.0, 0.0, fish);
	herm.costruisci_curva(-4.0, 4.0, fish);
	herm.costruisci_curva(5.0, -4.0, fish);
	herm.costruisci_curva(10.0, 0.0, fish);
	herm.costruisci_curva(5.0, 4.0, fish);
	herm.costruisci_curva(-4.0, -4.0, fish);
	
	herm.costruisci_formaHermite(color_top, color_bot, fish);
	fish->sceltaFs = 0;
	fish->corner_b_obj = vec4(8.0, 8.0, 0.0, 1.0);
	fish->corner_t_obj = vec4(12.0, 12.0, 0.0, 1.0);
}

void createHook(vec4 color_top, vec4 color_bot, Figura* hook) {
	Hermite herm;
	herm.costruisci_curva(0.0, 0.0, hook);
	herm.costruisci_curva(0.0, -2.0, hook);
	herm.costruisci_curva(2.0, -2.0, hook);
	herm.costruisci_curva(0.0, 0.0, hook);
	herm.costruisci_curva(2.0, 2.0, hook);
	herm.costruisci_curva(0.0, 4.0, hook);
	herm.costruisci_curva(-2.0, 0.0, hook);
	herm.costruisci_curva(0.0, -2.0, hook);
	
	herm.costruisci_formaHermite(color_top, color_bot, hook);
	hook->sceltaFs = 0;
	hook->corner_b_obj = vec4(8.0, 8.0, 0.0, 1.0);
	hook->corner_t_obj = vec4(12.0, 12.0, 0.0, 1.0);
}