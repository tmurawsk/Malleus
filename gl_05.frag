#version 330 core
in vec3 vecColor;
in vec2 TexCoord;
in vec3 vecNormal;
in float TexID;
in vec3 vPosition;

out vec4 color;

uniform sampler2D FloorTex;
uniform sampler2D HammerTex;
uniform sampler2D HammersideTex;
uniform sampler2D WoodTex;
uniform sampler2D MoldTex;
uniform sampler2D FormTex;
uniform sampler2D BarTex;
uniform sampler2D TriangleTex;
uniform sampler2D SkyboxTex;
uniform sampler2D GrassTex;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform vec3 lightAmbientColor;

vec3 light() {
    float distance = length(lightPosition - vPosition);
    float attenuation = 1.0f / (0.1f + 0.025*distance + 0.05*distance*distance);

    vec3 lightDir = normalize(lightPosition - vPosition);
    float diff = max(dot(vecNormal, lightDir), 0.0);

    vec3 diffuse = lightDiffuseColor * diff;

	vec3 viewDir = normalize(lightDirection - vPosition);
	vec3 reflectDir = reflect(-lightDir, vecNormal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = 0.5f * spec * lightDiffuseColor;  

    return vec3(lightAmbientColor + attenuation * (diffuse + specular));
}

void main()
{
	vec3 colorNonLinear;

	if(TexID == 1.0f)
		colorNonLinear = vec3(texture(FloorTex, TexCoord));
	else if(TexID == 2.0f)
		colorNonLinear = vec3(texture(HammerTex, TexCoord));
	else if(TexID == 3.0f)
		colorNonLinear = vec3(texture(HammersideTex, TexCoord));
	else if(TexID == 4.0f)
		colorNonLinear = vec3(texture(WoodTex, TexCoord));
	else if(TexID == 5.0f)
		colorNonLinear = vec3(texture(MoldTex, TexCoord));
	else if(TexID == 6.0f)
		colorNonLinear = vec3(texture(FormTex, TexCoord));
	else if(TexID == 7.0f)
		colorNonLinear = vec3(texture(BarTex, TexCoord));
	else if(TexID == 8.0f)
		colorNonLinear = vec3(texture(TriangleTex, TexCoord));
	else if(TexID == -2.0f)
		colorNonLinear = vec3(texture(GrassTex, TexCoord));
	else if(TexID == -1.0f)
		colorNonLinear = vec3(texture(SkyboxTex, TexCoord));
	else
		colorNonLinear = vecColor;

	vec3 gamma = vec3(1.0/2.2);
	vec3 invGamma = vec3(2.2);
	vec3 colorLinear = vec3(pow(colorNonLinear, invGamma)) * light();
	color = vec4(pow(colorLinear, gamma), 1.0f);
}
