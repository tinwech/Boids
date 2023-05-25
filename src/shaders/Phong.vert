#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float revertNormal;

out vec4 worldPos;
out vec3 normal;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	worldPos = M * vec4(aPos, 1.0);
	normal = revertNormal * normalize((transpose(inverse(M)) * vec4(aNormal, 0.0)).xyz);
}