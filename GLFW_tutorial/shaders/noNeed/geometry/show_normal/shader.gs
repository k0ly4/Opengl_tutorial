#version 330 core

layout(triangles) in;
layout(line_strip,max_vertices=6) out;

in VS_OUT{
   vec3 normal;
}gs_in[];
uniform float magnitude;

uniform mat4 projection;
void generate(int index){
    gl_Position = projection*gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection*(gl_in[index].gl_Position + vec4(gs_in[index].normal*magnitude,0.0f));
    EmitVertex();
    EndPrimitive();
}
void main()
{  
    generate(0);
    generate(1);
    generate(2);
}