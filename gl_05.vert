#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec3 vecColor;
out vec2 TexCoord;
smooth out vec3 vecNormal;

uniform mat4 rotate;		//macierz obrotu
uniform mat4 view;			//macierz widoku
uniform mat4 projection;	//macierz perspektywy (rzutowania)
uniform mat4 translate;		//macierz translacji
uniform mat4 normals;		//macierz normalnych

void main()
{
	if(gl_VertexID >= 114 || (gl_VertexID >= 32 && gl_VertexID <= 55))
		gl_Position = projection * view * rotate * vec4(position, 1.0f);
	else if(gl_VertexID >= 56 && gl_VertexID <= 113)
		gl_Position = projection * view * translate * vec4(position, 1.0f);
	else
		gl_Position = projection * view * vec4(position, 1.0f);

    vecColor = color;
    TexCoord = texCoord;

	vec4 vRes = normals*vec4(normal, 0.0f);
	vecNormal = vRes.xyz;
} 