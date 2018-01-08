#version 330 core
in vec3 vecColor;
in vec2 TexCoord;
smooth in vec3 vecNormal;
in float TexID;

out vec4 color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;

void main()
{
    //color = vec4(vecColor, 1.0f) * mix(texture(Texture0, TexCoord), texture(Texture1, TexCoord),0.4);
	if(TexID == 1.0f)
		color = texture2D(Texture0, TexCoord);
	else if(TexID == 2.0f)
		color = texture2D(Texture1, TexCoord);
	else if(TexID == 3.0f)
		color = texture2D(Texture2, TexCoord);
	else if(TexID == 5.0f)
		color = texture2D(Texture4, TexCoord);
	else if(TexID == 6.0f)
		color = texture2D(Texture5, TexCoord);
	else
		color = vec4(vecColor, 1.0f);
}
