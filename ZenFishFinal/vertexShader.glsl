#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
  
out vec4 ourColor;
uniform mat4 Projection;
uniform mat4 Model;
uniform int sceltaVS;
uniform float fishDir;
uniform vec2 resolution;
int wWidth = 1200;
int wHeight = 600;

void main(){
    if(sceltaVS ==0) {
        gl_Position = Projection*Model*vec4(aPos, 1.0);
        ourColor = aColor;
    } else if (sceltaVS ==1){
        gl_Position = Projection*Model*vec4(aPos*vec3(fishDir * (wWidth / resolution.x), 1.0 * (wHeight / resolution.y), 1.0), 1.0);
        ourColor = aColor;
    } else if (sceltaVS ==2){
        gl_Position = Projection*Model*vec4(aPos, 1.0);
        ourColor = aColor;
    }
} 
