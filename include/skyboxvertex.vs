#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	TexCoord = vec3(model * vec4(aPos, 1.0));
	vec4 pos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}