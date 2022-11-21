#include "Hermite.h"
#pragma once

void createVAO(Figura* fig);
void createBackground(vec4 color_top, vec4 color_bot, Figura* background, int width, int height);
void createSun(vec4 color_top, vec4 color_bot, float posx, float posy, Figura* sun);
void createFish(vec4 color_top, vec4 color_bot, Figura* virus);
void createHook(vec4 color_top, vec4 color_bot, Figura* virus);