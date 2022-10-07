#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;
layout (location = 2) in vec4 rect;
layout (location = 3) in vec2 size;
layout (location = 4) in mat4 local;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec2 texCoord;

void main()
{  
    texCoord = vec2(
        in_tex_coord.x == 1.f? rect.z:rect.x,
        in_tex_coord.y == 1.f? rect.w:rect.y
    );
    gl_Position = projection * camera * model * local * vec4(in_pos.xy* size,in_pos.z,1.f);
}