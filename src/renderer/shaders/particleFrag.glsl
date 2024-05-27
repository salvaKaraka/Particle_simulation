#version 330 core

out vec4 FragColor;

uniform vec2 position;
uniform float velocity;
uniform float radius;
uniform ivec2 resolution;

float sd_circle(vec2 pos, float r){
    return length(pos) - r;
}

void main() {
    float aspect_ratio = float(resolution.x) / float(resolution.y);
    vec2 uv = gl_FragCoord.xy / resolution;
    uv.x *= aspect_ratio;
    vec2 particle_pos = (position.xy+1)/2;


    float dist = sd_circle(uv - particle_pos, radius/2);

    float t = clamp(velocity/2, 0.0, 1.0);

    vec3 low_vel = vec3(0.3, 0.8, 0.9);
    vec3 mid_vel = vec3(0.9, 0.5, 0.4); 
    vec3 high_vel = vec3(0.9, 0.3, 0.3); 

    vec3 low_vel_border = vec3(0.4, 0.9, 1.0);
    vec3 mid_vel_border = vec3(1.0, 0.6, 0.5);
    vec3 high_vel_border = vec3(1.0, 0.4, 0.4);

    vec3 color_mix;


    if (t < 0.5) {
        color_mix = mix(low_vel, mid_vel, t * 2.0);
    } else {
        color_mix = mix(mid_vel, high_vel, (t - 0.5) * 2.0);
    }

    vec3 color_mix_border;

    if (t < 0.5) {
        color_mix_border = mix(low_vel_border, mid_vel_border, t * 2.0);
    } else {
        color_mix_border = mix(mid_vel_border, high_vel_border, (t - 0.5) * 2.0);
    }

    vec4 color = vec4(0.0);

    if (dist < -0.001){
        color = vec4(color_mix, 1.0);
    }else if (dist < 0.001){
        color = vec4(color_mix_border, 1.0);
    }

    if(color.a == 0.0) discard;

    FragColor = color;
}
