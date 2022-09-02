#version 330 core
 
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 VP;
uniform vec2 size;
uniform vec3 up;
uniform vec3 right;

out vec2 texCoord;

void main()
{
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 v_up= up*size.y;
    vec3 v_right= right*size.x;

    pos -= (v_right + v_up)* 0.5f;
    gl_Position = VP* vec4(pos, 1.0);
    texCoord = vec2(0.f, 0.f);
    EmitVertex();
 
    pos += v_up;
    gl_Position = VP* vec4(pos, 1.0);
    texCoord = vec2(0.f, 1.f);
    EmitVertex();
 
    pos += (v_right-v_up);
    gl_Position = VP* vec4(pos, 1.0);
    texCoord = vec2(1.f, 0.f);
    EmitVertex();
 
    pos += v_up;
    gl_Position = VP* vec4(pos, 1.0);
    texCoord = vec2(1.f, 1.f);
    EmitVertex();
 
    EndPrimitive();
}