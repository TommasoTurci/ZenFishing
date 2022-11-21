uniform vec2 resolution;
uniform int sceltaFS;
uniform vec2 sunPos;
uniform float iTime;
uniform vec2 mouse;
uniform float waterImpact;
in vec4 ourColor;

float noise(float x)
{
	float n = sin(x * 8.0) + 0.4 * sin(x * 20.0) - 0.3 * sin(1.0);
	return (n / 8.0) - 0.2;
}

float reflection_alpha(vec2 ndc)
{
float n = sin((sin(ndc.x) * 10.0) + iTime)/20.0;
return 0.0;
}

float calculate_alpha(vec2 ndc)
{
	float thresh = (sin((sin(ndc.x) * 10.0) + iTime) / 20.0) *  (1.0 + waterImpact);
	if (ndc.y < 0.3 - thresh) {
		return 0.5;
	}
	return 0.0;
}

float calculate_alpha_mountain(vec2 ndc)
{
	float thresh = noise(ndc.x);
	if (ndc.y < 0.3 - thresh) {
		return 1.0;
	}
	return 0.0;
}

vec2 convert_to_ndc()
{
	return vec2(
		gl_FragCoord.x / resolution.x,
		gl_FragCoord.y / resolution.y
	);
}

void main(){
    vec2 center = vec2(sunPos.x * resolution.x,sunPos.y * resolution.y);
	vec2 rCenter = vec2(sunPos.x * resolution.x,sunPos.y * resolution.y);
    float d = distance(center.xy,gl_FragCoord.xy)*(-sin(iTime*0.09)+0.1)*0.001;
    vec4 gradient = mix(vec4(0.5, 0.5, 1.0, 1.0), vec4(0.0, 0.0, 0.2, 1.0), d);
	vec4 mountain = mix(gradient, vec4(0.2, 0.6, 0.2, 1.0), calculate_alpha_mountain(convert_to_ndc()));
	vec4 color = mix(mountain, vec4(1.0, 1.0, 1.0, 1.0), calculate_alpha(convert_to_ndc()));
  if(sceltaFS == 0){
	vec4 sun = mix(ourColor, color, calculate_alpha_mountain(convert_to_ndc()));
	gl_FragColor = vec4(sun);
  }
  else if(sceltaFS ==1){
    gl_FragColor = color;
  } else if(sceltaFS ==2) {
	gl_FragColor = ourColor;
  }
}