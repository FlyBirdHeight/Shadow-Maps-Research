#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrix[6];

out vec4 FragPos;

void main(){
    for(int i = 0;i < 6; ++i){
        gl_Layer = i;
        for(int j = 0; j < 3; ++j){
            FragPos = gl_in[j].gl_Position;
            gl_Position = shadowMatrix[i] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
