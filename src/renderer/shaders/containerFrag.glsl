#version 330 core

out vec4 FragColor;

uniform vec2 container_size;
uniform bool container;
uniform ivec2 resolution;


float sd_circle(vec2 pos, float r){
    return length(pos) - r;
}

float sd_square (vec2 pos, vec2 size){
	vec2 d = abs(pos) - size;
	return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}   

void main() {
    
    float aspect_ratio = float(resolution.x) / float(resolution.y);

    vec2 uv = gl_FragCoord.xy / resolution;
    uv.x *= aspect_ratio;


    
    float dist = 0;
    if (container)
    dist = sd_circle(uv + vec2(-.5f), container_size.x/2);
    else
    dist = sd_square(uv + vec2(-.5f), container_size/4);


    vec4 color = vec4(0.0);

    if (dist < -0.001){
        color = vec4(0.0f);
    }else if (dist < 0.01){
		color = vec4(0.8f);
	}

    if( color.a == 0.0f ) discard;

    FragColor = color;
}