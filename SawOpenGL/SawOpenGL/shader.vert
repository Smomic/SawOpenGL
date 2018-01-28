#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;


out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 animation;

void main()
{
	FragPos = vec3(animation * vec4(position, 1.0));
	Normal = vec3(animation * vec4(aNormal, 1.0));
	
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoord = texCoord;
} 