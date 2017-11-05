#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoord;

layout (std140) uniform PTE_Camera
{
	 mat4 model;
	 mat4 view;
	 mat4 projection;
};

void main()
{	
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = aTexCoords;
}