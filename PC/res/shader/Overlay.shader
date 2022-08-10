#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vertexCoords;

out vec2 uv;

void main(){
    gl_Position = vec4(position, 0, 1);
    uv = vertexCoords;
}


#shader fragment
#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureSampler;

void main(){
    //color = vec3(0.5, 1.0, 0.25);
    color = texture(textureSampler, uv);
}