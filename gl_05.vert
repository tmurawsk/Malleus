#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;
layout (location = 4) in float texID;

out vec3 vecColor;
out vec2 TexCoord;
out vec3 vecNormal;
out float TexID;
out vec3 vecPosition;

uniform mat4 rotate;		//macierz obrotu
uniform mat4 view;			//macierz widoku
uniform mat4 projection;	//macierz perspektywy (rzutowania)
uniform mat4 translate;		//macierz translacji

void main()
{
	if((gl_VertexID >= 114 && gl_VertexID <= 178) || (gl_VertexID >= 32 && gl_VertexID <= 55))		//mlot
	{
		gl_Position = projection * view * rotate * vec4(position, 1.0f);
		vecPosition = (rotate * vec4(position, 1.0f)).xyz;
		vecNormal = normalize(transpose(inverse(mat3(rotate))) * normal);
	}

	else if((gl_VertexID >= 56 && gl_VertexID <= 113) || (gl_VertexID >= 179 && gl_VertexID <= 186))
	{
		gl_Position = projection * view * translate * vec4(position, 1.0f);				//odlewy
		vecPosition = (translate * vec4(position, 1.0f)).xyz;
		vecNormal = normalize(transpose(inverse(mat3(translate))) * normal);
	}
	
	else
	{
		gl_Position = projection * view * vec4(position, 1.0f);				//reszta
		vecPosition = position;
		vecNormal = normal;
	}

    vecColor = color;
    TexCoord = texCoord;
	TexID = texID;

	if(gl_VertexID >= 187 && gl_VertexID <= 206) //skybox
		vecNormal = vec3(0.0f);
} 