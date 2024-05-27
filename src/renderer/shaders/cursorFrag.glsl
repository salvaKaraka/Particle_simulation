#version 330 core

out vec4 FragColor;

uniform float cursor_radius;
uniform vec2 cursor_pos;
uniform bool left_clicked;
uniform bool right_clicked;
uniform float time;
uniform ivec2 resolution;


float sd_circle(vec2 pos, float r){
    return length(pos) - r;
}

void main() {
    
    float aspect_ratio = float(resolution.x) / float(resolution.y);

    vec2 uv = gl_FragCoord.xy / resolution;
    uv.x *= aspect_ratio;
    vec2 cursor = cursor_pos / resolution;
    cursor.x *= aspect_ratio;

    float animated_radius = cursor_radius / 4.0;

    vec4 fill=vec4(0.2f);

    vec4 border = vec4(0.5);

    
    if(left_clicked){
        animated_radius += sin(time * 5.0) * 0.002;
        fill = vec4(0.9, 0.7, 0.2, 0.2);
        border = vec4(1.0, 0.8, 0.3, 1.0);

    }else if(right_clicked){
        animated_radius += sin(time * 5.0) * 0.002;
		fill = vec4(0.2, 0.7, 0.9, 0.2);
        border = vec4(0.3, 0.8, 1.0, 1.0);
    }
    
    float dist = sd_circle(uv - cursor, animated_radius);

    vec4 color = vec4(0.0);

    if (dist < -0.001){
        color = fill;
    }else if (dist < 0.001){
		color = border;
	}

    if( color.a == 0.0f ) discard;

    FragColor = color;
}