#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
//layout (location = 3) in vec3 normal; //??

out vec3 vecColor;
out vec2 TexCoord;
  
uniform mat4 transform;		//macierz transformacji
uniform mat4 view;			//macierz widoku
uniform mat4 projection;	//macierz perspektywy (rzutowania)

void main()
{
	//gl_Position = projection * view * transform * vec4(position, 1.0f);
	gl_Position = projection * view * vec4(position, 1.0f);

    vecColor = color;
    TexCoord = texCoord;
} 