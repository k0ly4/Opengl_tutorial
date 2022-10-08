#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;
uniform float time;
vec3 normal(){
vec3 a = vec3(gl_in[0].gl_Position)- vec3(gl_in[1].gl_Position);
vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
return normalize(cross(a,b));
}
vec4 explode(vec3 normal, float offset){
    return vec4(normal* offset,0.f);
}
void main()
{  
  vec3 norm = normal();
  float magnitude = 20.f;
  float offset = time*magnitude;
  gl_Position= gl_in[0].gl_Position + explode(norm,offset);
  EmitVertex();
  gl_Position= gl_in[1].gl_Position + explode(norm,offset);
  EmitVertex();
  gl_Position= gl_in[2].gl_Position + explode(norm,offset);
  EmitVertex();
  EndPrimitive();
}