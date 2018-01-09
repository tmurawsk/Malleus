#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#define MOVE_SPEED 0.1f
#define RADIUS 3.0f
#define SKYBOX 50.0f
#define SKYMOVE 5.0f

const GLuint WIDTH = 1280, HEIGHT = 1024;
GLfloat posX = 0.0f, posY = 1.0f, posZ = -4.0f;
GLfloat lookAngleH = 90.0f, lookAngleV = 0.0f;
GLfloat lookX = posX + RADIUS * cos(glm::radians(lookAngleV)) * cos(glm::radians(lookAngleH));
GLfloat lookY = posY + RADIUS * sin(glm::radians(lookAngleV));
GLfloat lookZ = posZ + RADIUS * cos(glm::radians(lookAngleV)) * sin(glm::radians(lookAngleH));
GLfloat rotate_speed = 0.1f, translate_speed = 0.000333333333333f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << "key: " << key << endl;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		GLfloat trans;
		bool move = false;
		
		switch (key)
		{		
		case GLFW_KEY_W:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV)) * cos(glm::radians(lookAngleH));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV)) * sin(glm::radians(lookAngleH));
			posZ += trans;
			lookZ += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV));
			posY += trans;
			lookY += trans;
			move = true;
			break;

		case GLFW_KEY_S:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV)) * cos(glm::radians(lookAngleH));
			posX -= trans;
			lookX -= trans;
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV)) * sin(glm::radians(lookAngleH));
			posZ -= trans;
			lookZ -= trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV));
			posY -= trans;
			lookY -= trans;
			move = true;
			break;

		case GLFW_KEY_A:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH - 90.0f));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH - 90.0f));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_D:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleH + 90.0f));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleH + 90.0f));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_R:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV));
			posY += trans;
			lookY += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV)) * cos(glm::radians(lookAngleH));
			posX -= trans;
			lookX -= trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV)) * sin(glm::radians(lookAngleH));
			posZ -= trans;
			lookZ -= trans;
			move = true;
			break;

		case GLFW_KEY_F:
			trans = MOVE_SPEED * cos(glm::radians(lookAngleV));
			posY -= trans;
			lookY -= trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV)) * cos(glm::radians(lookAngleH));
			posX += trans;
			lookX += trans;
			trans = MOVE_SPEED * sin(glm::radians(lookAngleV)) * sin(glm::radians(lookAngleH));
			posZ += trans;
			lookZ += trans;
			move = true;
			break;

		case GLFW_KEY_RIGHT:
			lookAngleH = (lookAngleH > 360.0f ? (lookAngleH - 355.0f) : (lookAngleH + 5.0f));
			trans = RADIUS * cos(glm::radians(lookAngleV));
			lookX = posX + trans * cos(glm::radians(lookAngleH));
			lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			break;

		case GLFW_KEY_LEFT:
			lookAngleH = (lookAngleH < -360.0f ? (lookAngleH + 355.0f) : (lookAngleH - 5.0f));
			trans = RADIUS * cos(glm::radians(lookAngleV));
			lookX = posX + trans * cos(glm::radians(lookAngleH));
			lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			break;

		case GLFW_KEY_UP:
			if (lookAngleV+5.0f < 90.0f)
			{
				lookAngleV += 5.0f;
				trans = RADIUS * cos(glm::radians(lookAngleV));
				lookY = posY + RADIUS * sin(glm::radians(lookAngleV));
				lookX = posX + trans * cos(glm::radians(lookAngleH));
				lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			}
			break;

		case GLFW_KEY_DOWN:
			if (lookAngleV-5.0f > -90.0f)
			{
				lookAngleV -= 5.0f;
				trans = RADIUS * cos(glm::radians(lookAngleV));
				lookY = posY + RADIUS * sin(glm::radians(lookAngleV));
				lookX = posX + trans * cos(glm::radians(lookAngleH));
				lookZ = posZ + trans * sin(glm::radians(lookAngleH));
			}
			break;

		case GLFW_KEY_EQUAL:
			if (rotate_speed < 4.0f)
			{
				rotate_speed += 0.1f;
				translate_speed += 0.000333333333333f;
			}
			break;

		case GLFW_KEY_MINUS:
			if (rotate_speed - 0.1f >= 0.0f)
			{
				rotate_speed -= 0.1f;
				translate_speed -= 0.000333333333333f;
			}
			else
			{
				rotate_speed = 0.0f;
				translate_speed = 0.0f;
			}
			break;

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		default:
			break;
		}
		if (move)
			cout << "x: " << posX << " y: " << posY << " z: " << posZ << endl;
	}
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

int main()
{
	{
		glm::mat4 trans;
		cout << trans << endl;
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //drugi parametr - kat w radianach, trzeci - os obrotu przechodzacej przez srodek uklady wsp. (tutaj - obrot o 90 stopnia wokol osi Z)
		cout << trans << endl;
	}
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 05", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);

		glEnable(GL_DEPTH_TEST); //wlacza Z-bufor, ktory sprawdza ktore piksele rysujemy najpierw, zeby tekstury sie nie nakladaly

		// Let's check what are maximum parameters counts
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		cout << "Max vertex attributes allowed: " << nrAttributes << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &nrAttributes);
		cout << "Max texture coords allowed: " << nrAttributes << std::endl;

		// Build, compile and link shader program
		ShaderProgram theProgram("gl_05.vert", "gl_05.frag");

		// Set up vertex data
		/*GLfloat vertices[] = { //powinny byc tu tez trzy kolejne wartosci do kazdego wierzcholka - jego normalna, dzieki ktorej mozna okreslic oswietlenie
			// coordinates			// color			// texture		// normals
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//0
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//1
			- 0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,	//2
			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,	//3

			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//4
			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	//5
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	//6
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//7

			-0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//8
			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//9
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//10
			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//11

			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//12
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	//13
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	//14
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//15

			-0.25f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		-0.8944f, -0.4472f, 0.0f,	//16
			-0.75f, 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		-0.8944f, -0.4472f, 0.0f,	//17
			-0.25f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		-0.8944f, -0.4472f, 0.0f,	//18
			-0.75f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		-0.8944f, -0.4472f, 0.0f,	//19

			0.75f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,		0.8944f, 0.4472f, 0.0f,	//20
			0.75f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	0.0f, 0.0f,		0.8944f, 0.4472f, 0.0f,	//21
			0.25f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		0.8944f, 0.4472f, 0.0f,	//22
			0.25f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.8944f, 0.4472f, 0.0f	//23
		};

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 13, 14,
			12, 14, 15,
			16, 17, 18,
			18, 17, 19,
			20, 21, 22,
			20, 22, 23
		};*/

		GLfloat vertices[211*12] = {
			// coordinates			// color			// texture		// normals
			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 2.0f,		0.0f, 1.0f, 0.0f,	1.0f,	//0
			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f,	//1
			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f,	//2
			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 2.0f,		0.0f, 1.0f, 0.0f,	1.0f,	//3

			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 2.0f,		0.0f, 0.0f, -1.0f,	1.0f,	//4
			2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 1.8f,		0.0f, 0.0f, -1.0f,	1.0f,	//5
			-2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 1.8f,		0.0f, 0.0f, -1.0f,	1.0f,	//6
			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 2.0f,		0.0f, 0.0f, -1.0f,	1.0f,	//7

			-2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 2.0f,		-1.0f, 0.0f, 0.0f,	1.0f,	//8
			-2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 1.9f,		-1.0f, 0.0f, 0.0f,	1.0f,	//9
			-2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 1.9f,		-1.0f, 0.0f, 0.0f,	1.0f,	//10
			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 2.0f,		-1.0f, 0.0f, 0.0f,	1.0f,	//11

			-2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 0.2f,		0.0f, 0.0f, 1.0f,	1.0f,	//12
			-2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f,	//13
			2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f,	//14
			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 0.2f,		0.0f, 0.0f, 1.0f,	1.0f,	//15

			2.0f, 0.2f, 1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 2.0f,		1.0f, 0.0f, 0.0f,	1.0f,	//16
			2.0f, 0.0f, 1.0f,		0.4f, 0.4f, 0.4f,	0.0f, 1.9f,		1.0f, 0.0f, 0.0f,	1.0f,	//17
			2.0f, 0.0f, -1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 1.9f,		1.0f, 0.0f, 0.0f,	1.0f,	//18
			2.0f, 0.2f, -1.0f,		0.4f, 0.4f, 0.4f,	2.0f, 2.0f,		1.0f, 0.0f, 0.0f,	1.0f,	//19

			-1.2f, 1.0f, -0.8f,		0.0f, 0.55f, 0.0f,	1.0f, 2.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//20
			-0.8f, 0.2f, -0.8f,		0.0f, 0.55f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//21
			-1.6f, 0.2f, -0.8f,		0.0f, 0.55f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//22

			-1.2f, 1.0f, -0.4f,		0.0f, 0.55f, 0.0f,	0.0f, 2.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//23
			-0.8f, 0.2f, -0.4f,		0.0f, 0.55f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//24
			-1.6f, 0.2f, -0.4f,		0.0f, 0.55f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//25

			-1.2f, 1.0f, 0.4f,		0.0f, 0.55f, 0.0f,	1.0f, 2.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//26
			-0.8f, 0.2f, 0.4f,		0.0f, 0.55f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//27
			-1.6f, 0.2f, 0.4f,		0.0f, 0.55f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	8.0f,	//28

			-1.2f, 1.0f, 0.8f,		0.0f, 0.55f, 0.0f,	0.0f, 2.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//29
			-0.8f, 0.2f, 0.8f,		0.0f, 0.55f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//30
			-1.6f, 0.2f, 0.8f,		0.0f, 0.55f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	8.0f,	//31

			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	2.0f,	//32
			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	2.0f,	//33
			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	2.0f,	//34
			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	2.0f,	//35

			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	2.0f,	//36
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	2.0f,	//37
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	2.0f,	//38
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	2.0f,	//39

			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,	3.0f,	//40
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	3.0f,	//41
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,	3.0f,	//42
			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,	3.0f,	//43

			1.104f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	3.0f,	//44
			1.104f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	3.0f,	//45
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	3.0f,	//46
			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	3.0f,	//47

			1.104f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	3.0f,	//48
			1.104f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	3.0f,	//49
			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	3.0f,	//50
			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	3.0f,	//51

			1.696f, 1.24f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	3.0f,	//52
			1.696f, 0.36f, 0.296f,	0.3f, 0.3f, 0.3f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	3.0f,	//53
			1.696f, 0.36f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	3.0f,	//54
			1.696f, 1.24f, -0.296f,	0.3f, 0.3f, 0.3f,	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	3.0f,	//55

			1.743f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	0.0f, 2.01f,		1.0f, 0.0f, 0.0f,	6.0f,	//56
			1.743f, 0.2f, 0.93f,	1.0f, 0.5f, 0.0f,	0.165f, 2.01f,		1.0f, 0.0f, 0.0f,	6.0f,	//57
			1.743f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	0.165f, 0.0f,		1.0f, 0.0f, 0.0f,	6.0f,	//58
			1.743f, 0.305f, -0.344f,1.0f, 0.5f, 0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	6.0f,	//59

			1.07f, 0.305f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	6.0f,	//60
			1.07f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	0.835f, 0.0f,		-1.0f, 0.0f, 0.0f,	6.0f,	//61
			1.07f, 0.2f, 0.93f,		1.0f, 0.5f, 0.0f,	0.835f, 2.01f,		-1.0f, 0.0f, 0.0f,	6.0f,	//62
			1.07f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 2.01f,		-1.0f, 0.0f, 0.0f,	6.0f,	//63

			1.4f, 0.369f, 0.6f,		0.4f, 0.0f, 1.0f,	0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	5.0f	//64
		};

		float angle = 0.0f;
		for (int i = 65 * 12; i < 89 * 12; i += 2*12) //65-88, 90-113 (odlewy)
		{
			vertices[i] = 1.4f - 0.237f * sin(glm::radians(angle));	//x
			vertices[i + 1] = 0.369f;	//y
			vertices[i + 2] = 0.6f + 0.237f * cos(glm::radians(angle)); //z
			vertices[i + 3] = 0.4f;	//r
			vertices[i + 4] = 0.0f;	//g
			vertices[i + 5] = 1.0f;	//b
			vertices[i + 6] = 0.5f + 0.5f * sin(glm::radians(angle));	//tx
			vertices[i + 7] = 0.5f + 0.5f * cos(glm::radians(angle));	//ty
			vertices[i + 8] = sin(glm::radians(angle));	//nx
			vertices[i + 9] = 0.0f;	//ny
			vertices[i + 10] = cos(glm::radians(angle));//nz
			vertices[i + 11] = 5.0f;

			vertices[i + 12] = 1.4f - 0.237f * sin(glm::radians(angle));	//x
			vertices[i + 13] = 0.305f;	//y
			vertices[i + 14] = 0.6f + 0.237f * cos(glm::radians(angle)); //z
			vertices[i + 15] = 0.4f;	//r
			vertices[i + 16] = 0.0f;	//g
			vertices[i + 17] = 1.0f;	//b
			vertices[i + 18] = 0.5f + 0.5f * sin(glm::radians(angle));	//tx
			vertices[i + 19] = 0.5f + 0.5f * cos(glm::radians(angle));	//ty
			vertices[i + 20] = sin(glm::radians(angle));	//nx
			vertices[i + 21] = 0.0f;	//ny
			vertices[i + 22] = cos(glm::radians(angle));//nz
			vertices[i + 23] = 5.0f;

			int pos = i + 25 * 12;
			vertices[pos] = 1.4f - 0.237f * sin(glm::radians(angle));	//x
			vertices[pos + 1] = 0.369f;	//y
			vertices[pos + 2] = 0.237f * cos(glm::radians(angle)); //z
			vertices[pos + 3] = 0.4f;	//r
			vertices[pos + 4] = 0.0f;	//g
			vertices[pos + 5] = 1.0f;	//b
			vertices[pos + 6] = 0.5f + 0.5f * sin(glm::radians(angle));	//tx
			vertices[pos + 7] = 0.5f + 0.5f * cos(glm::radians(angle));	//ty
			vertices[pos + 8] = sin(glm::radians(angle));	//nx
			vertices[pos + 9] = 0.0f;	//ny
			vertices[pos + 10] = cos(glm::radians(angle));//nz
			vertices[pos + 11] = 5.0f;

			vertices[pos + 12] = 1.4f - 0.237f * sin(glm::radians(angle));	//x
			vertices[pos + 13] = 0.305f;//y
			vertices[pos + 14] = 0.237f * cos(glm::radians(angle)); //z
			vertices[pos + 15] = 0.4f;	//r
			vertices[pos + 16] = 0.0f;	//g
			vertices[pos + 17] = 1.0f;	//b
			vertices[pos + 18] = 0.5f + 0.5f * sin(glm::radians(angle));	//tx
			vertices[pos + 19] = 0.5f + 0.5f * cos(glm::radians(angle));	//ty
			vertices[pos + 20] = sin(glm::radians(angle));	//nx
			vertices[pos + 21] = 0.0f;	//ny
			vertices[pos + 22] = cos(glm::radians(angle));//nz
			vertices[pos + 23] = 5.0f;

			angle += 30.0f;
		}

		int pos = 89 * 12; //89
		vertices[pos] = 1.4f;	//x
		vertices[pos + 1] = 0.369f;	//y
		vertices[pos + 2] = 0.0f;	//z
		vertices[pos + 3] = 0.4f;	//r
		vertices[pos + 4] = 0.0f;	//g
		vertices[pos + 5] = 1.0f;	//b
		vertices[pos + 6] = 0.5f;	//tx
		vertices[pos + 7] = 0.5f;	//ty
		vertices[pos + 8] = 0.0f;	//nx
		vertices[pos + 9] = 1.0f;	//ny
		vertices[pos + 10] = 0.0f;	//nz
		vertices[pos + 11] = 5.0f;

		angle = 0.0f;
		float texX = 0.0f;
		for (int i = 114 * 12; i < 130 * 12; i += 2 * 12) //114-129 (poprzeczka)
		{
			vertices[i] = -1.2f - 0.059f * sin(glm::radians(angle));	//x
			vertices[i + 1] = 0.8f + 0.059f * cos(glm::radians(angle));	//y
			vertices[i + 2] = -0.4f; //z
			vertices[i + 3] = 0.7f;	//r
			vertices[i + 4] = 0.7f;	//g
			vertices[i + 5] = 0.7f;	//b
			vertices[i + 6] = texX;	//tx
			vertices[i + 7] = 0.0f;	//ty
			vertices[i + 8] = sin(glm::radians(angle));	//nx
			vertices[i + 9] = cos(glm::radians(angle));	//ny
			vertices[i + 10] = 0.0f;//nz
			vertices[i + 11] = 7.0f;

			vertices[i + 12] = -1.2f - 0.059f * sin(glm::radians(angle));	//x
			vertices[i + 13] = 0.8f + 0.059f * cos(glm::radians(angle));	//y
			vertices[i + 14] = 0.4f;	//z
			vertices[i + 15] = 0.7f;	//r
			vertices[i + 16] = 0.7f;	//g
			vertices[i + 17] = 0.7f;	//b
			vertices[i + 18] = texX;	//tx
			vertices[i + 19] = 2.0f;	//ty
			vertices[i + 20] = sin(glm::radians(angle));	//nx
			vertices[i + 21] = cos(glm::radians(angle));	//ny
			vertices[i + 22] = 0.0f;//nz
			vertices[i + 23] = 7.0f;

			angle += 45.0f;
			texX += 0.125f;
		}

		pos = 130 * 12; //130
		vertices[pos] = -1.695f;	//x
		vertices[pos + 1] = 0.8f;	//y
		vertices[pos + 2] = 0.0f;	//z
		vertices[pos + 3] = 0.4f;	//r
		vertices[pos + 4] = 0.2f;	//g
		vertices[pos + 5] = 0.0f;	//b
		vertices[pos + 6] = 0.3f;	//tx
		vertices[pos + 7] = 0.3f;	//ty
		vertices[pos + 8] = 1.0f;	//nx
		vertices[pos + 9] = 0.0f;	//ny
		vertices[pos + 10] = 0.0f;	//nz
		vertices[pos + 11] = 4.0f;

		angle = 0.0f;
		texX = 0.0f;
		for (int i = 131 * 12; i < 179 * 12; i += 2 * 12) //131-178 (rekojesc)
		{
			vertices[i] = -1.695f;	//x
			vertices[i + 1] = 0.8f + 0.148f * cos(glm::radians(angle));	//y
			vertices[i + 2] = 0.148f * sin(glm::radians(angle));	//z
			vertices[i + 3] = 0.4f;	//r
			vertices[i + 4] = 0.2f;	//g
			vertices[i + 5] = 0.0f;	//b
			vertices[i + 6] = texX;	//tx
			vertices[i + 7] = 0.0f;	//ty
			vertices[i + 8] = 0.0f;	//nx
			vertices[i + 9] = cos(glm::radians(angle));	//ny
			vertices[i + 10] = sin(glm::radians(angle));	//nz
			vertices[i + 11] = 4.0f;

			vertices[i + 12] = 1.104f;	//x
			vertices[i + 13] = 0.8f + 0.148f * cos(glm::radians(angle));	//y
			vertices[i + 14] = 0.148f * sin(glm::radians(angle));	//z
			vertices[i + 15] = 0.4f;	//r
			vertices[i + 16] = 0.2f;	//g
			vertices[i + 17] = 0.0f;	//b
			vertices[i + 18] = texX;	//tx
			vertices[i + 19] = 1.0f;	//ty
			vertices[i + 20] = 0.0f;	//nx
			vertices[i + 21] = cos(glm::radians(angle));	//ny
			vertices[i + 22] = sin(glm::radians(angle));	//nz
			vertices[i + 23] = 4.0f;

			angle += 20.0f;
			texX += 1.0f / 18.0f;
		}

		GLfloat formSides[] = { //sciany formy (179-168)
			1.743f, 0.305f, -0.344f,	1.0f, 0.5f, 0.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,	6.0f,	//179
			1.743f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	0.0f, 0.835f,		0.0f, 0.0f, -1.0f,	6.0f,	//180
			1.07f, 0.2f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 0.835f,		0.0f, 0.0f, -1.0f,	6.0f,	//181
			1.07f, 0.305f, -0.344f,	1.0f, 0.5f, 0.0f,	1.0f, 1.0f,			0.0f, 0.0f, -1.0f,	6.0f,	//182

			1.07f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	0.0f, 1.0f,			0.0f, 0.0f, 1.0f,	6.0f,	//183
			1.07f, 0.2f, 0.93f,		1.0f, 0.5f, 0.0f,	0.0f, 0.835f,		0.0f, 0.0f, 1.0f,	6.0f,	//184
			1.743f, 0.2f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 0.835f,		0.0f, 0.0f, 1.0f,	6.0f,	//185
			1.743f, 0.305f, 0.93f,	1.0f, 0.5f, 0.0f,	1.0f, 1.0f,			0.0f, 0.0f, 1.0f,	6.0f	//186
		};

		for (int i = 179 * 12; i < (179 + 8) * 12; i++)
			vertices[i] = formSides[i - 179*12];

		GLfloat skybox[] = {
			//top
			SKYBOX, SKYBOX+SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.0f,		0.0f, -1.0f, 0.0f,	-1.0f,	//187
			SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.33333f,	0.0f, -1.0f, 0.0f,	-1.0f,	//188
			-SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.33333f,		0.0f, -1.0f, 0.0f,	-1.0f,	//189
			-SKYBOX, SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.0f,			0.0f, -1.0f, 0.0f,	-1.0f,	//190

			//front
			SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.33333f,	0.0f, 0.0f, -1.0f,	-1.0f,	//191
			SKYBOX, -SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.66666f,	0.0f, 0.0f, -1.0f,	-1.0f,	//192
			-SKYBOX, -SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.66666f,		0.0f, 0.0f, -1.0f,	-1.0f,	//193
			-SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.33333f,		0.0f, 0.0f, -1.0f,	-1.0f,	//194

			//left
			SKYBOX, SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.0f, 0.33333f,		-1.0f, 0.0f, 0.0f,	-1.0f,	//195
			SKYBOX, -SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.0f, 0.66666f,		-1.0f, 0.0f, 0.0f,	-1.0f,	//196
			SKYBOX, -SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.66666f,	-1.0f, 0.0f, 0.0f,	-1.0f,	//197
			SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.25f, 0.33333f,	-1.0f, 0.0f, 0.0f,	-1.0f,	//198

			//right
			-SKYBOX, SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.33333f,		1.0f, 0.0f, 0.0f,	-1.0f,	//199
			-SKYBOX, -SKYBOX + SKYMOVE, SKYBOX,	0.4f, 0.4f, 0.4f,	0.5f, 0.66666f,		1.0f, 0.0f, 0.0f,	-1.0f,	//200
			-SKYBOX, -SKYBOX + SKYMOVE, -SKYBOX,0.4f, 0.4f, 0.4f,	0.75f, 0.66666f,	1.0f, 0.0f, 0.0f,	-1.0f,	//201
			-SKYBOX, SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.75f, 0.33333f,	1.0f, 0.0f, 0.0f,	-1.0f,	//202

			//back
			-SKYBOX, SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	0.75f, 0.33333f,	0.0f, 0.0f, 1.0f,	-1.0f,	//203
			-SKYBOX, -SKYBOX + SKYMOVE, -SKYBOX,0.4f, 0.4f, 0.4f,	0.75f, 0.66666f,	0.0f, 0.0f, 1.0f,	-1.0f,	//204
			SKYBOX, -SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	1.0f, 0.66666f,		0.0f, 0.0f, 1.0f,	-1.0f,	//205
			SKYBOX, SKYBOX + SKYMOVE, -SKYBOX,	0.4f, 0.4f, 0.4f,	1.0f, 0.33333f,		0.0f, 0.0f, 1.0f,	-1.0f,	//206

			//bottom
			SKYBOX/2, 0.0f, SKYBOX/2,		0.4f, 0.4f, 0.4f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	-2.0f,	//207
			SKYBOX/2, 0.0f, -SKYBOX/2,		0.4f, 0.4f, 0.4f,	0.0f, 5.0f,		0.0f, 1.0f, 0.0f,	-2.0f,	//208
			-SKYBOX/2, 0.0f, -SKYBOX/2,		0.4f, 0.4f, 0.4f,	5.0f, 5.0f,		0.0f, 1.0f, 0.0f,	-2.0f,	//209
			-SKYBOX/2, 0.0f, SKYBOX/2,		0.4f, 0.4f, 0.4f,	5.0f, 0.0f,		0.0f, 1.0f, 0.0f,	-2.0f	//210
		};

		for (int i = 0; i < 24 * 12; i++)
			vertices[187 * 12 + i] = skybox[i];

		GLuint indices[(44+24+48+16+54+12)*3] = {
			0, 1, 2,
			0, 2, 3,	//Podstawa-top
			4, 5, 6,
			4, 6, 7,	//Podstawa-right
			8, 9, 10,
			8, 10, 11,	//Podstawa-back
			12, 13, 14,
			12, 14, 15,	//Podstawa-left
			16, 17, 18,
			16, 18, 19, //Podstawa-front
			20, 21, 22,
			20, 22, 25,
			20, 25, 23,
			20, 23, 24,
			20, 24, 21,
			23, 25, 24,	//Trzymak-right
			26, 27, 28,
			26, 28, 31,
			26, 31, 29,
			26, 29, 30,
			26, 30, 27,
			29, 31, 30,	//Trzymak-left
			32, 33, 34,
			32, 34, 35,
			39, 38, 37,
			39, 37, 36,
			40, 41, 42,
			40, 42, 43,
			44, 45, 46,
			44, 46, 47,
			48, 49, 50,
			48, 50, 51,
			52, 53, 54,
			52, 54, 55,	//Mlot
			56, 57, 58,
			56, 58, 59,
			60, 61, 62,
			60, 62, 63,
			179, 180, 181,
			179, 181, 182,
			183, 184, 185,
			183, 185, 186,
			63, 56, 59,
			63, 59, 60	//Odlewnia, 44 sciany
		};

		pos = 1;
		for (int i = 44*3; i < (44+24+48)*3; i += 18) //Odlewy
		{
			indices[i] = 64;
			indices[i + 1] = 64 + pos;
			if (64 + pos + 2 < 89)
				indices[i + 2] = 64 + pos + 2;
			else
				indices[i + 2] = 65;

			indices[i + 3] = 64 + pos;
			indices[i + 4] = 64 + pos + 1;
			if (64 + pos + 3 < 89)
				indices[i + 5] = 64 + pos + 3;
			else
				indices[i + 5] = 66;
			
			indices[i + 6] = 64 + pos;
			if (64 + pos + 3 < 89)
				indices[i + 7] = 64 + pos + 3;
			else
				indices[i + 7] = 66;
			if (64 + pos + 2 < 89)
				indices[i + 8] = 64 + pos + 2;
			else
				indices[i + 8] = 65;


			indices[i + 9] = 89;
			indices[i + 10] = 89 + pos;
			if (89 + pos + 2 < 114)
				indices[i + 11] = 89 + pos + 2;
			else
				indices[i + 11] = 90;
			
			indices[i + 12] = 89 + pos;
			indices[i + 13] = 89 + pos + 1;
			if (89 + pos + 3 < 114)
				indices[i + 14] = 89 + pos + 3;
			else
				indices[i + 14] = 91;

			indices[i + 15] = 89 + pos;
			if (89 + pos + 3 < 114)
				indices[i + 16] = 89 + pos + 3;
			else
				indices[i + 16] = 91;
			if (89 + pos + 2 < 114)
				indices[i + 17] = 89 + pos + 2;
			else
				indices[i + 17] = 90;

			pos += 2;
		}

		pos = 1;
		for (int i = (44 + 24 + 48) * 3; i < (44 + 24 + 48 + 16)*3; i += 6)
		{
			indices[i] = 113 + pos;
			indices[i + 1] = 113 + pos + 1;
			if (113 + pos + 3 < 130)
				indices[i + 2] = 113 + pos + 3;
			else
				indices[i + 2] = 115;

			indices[i + 3] = 113 + pos;
			if (113 + pos + 3 < 130)
				indices[i + 4] = 113 + pos + 3;
			else
				indices[i + 4] = 115;
			if (113 + pos + 2 < 130)
				indices[i + 5] = 113 + pos + 2;
			else
				indices[i + 5] = 114;

			pos += 2;
		}

		pos = 1;
		for (int i = (44 + 24 + 48 + 16) * 3; i < (44 + 24 + 48 + 16 + 54) * 3; i += 9) //rekojesc
		{
			indices[i] = 130;
			indices[i + 1] = 130 + pos;
			if (130 + pos + 2 < 179)
				indices[i + 2] = 130 + pos + 2;
			else
				indices[i + 2] = 131;

			indices[i + 3] = 130 + pos;
			indices[i + 4] = 130 + pos + 1;
			if (130 + pos + 3 < 179)
				indices[i + 5] = 130 + pos + 3;
			else
				indices[i + 5] = 132;

			indices[i + 6] = 130 + pos;
			if (130 + pos + 3 < 179)
				indices[i + 7] = 130 + pos + 3;
			else
				indices[i + 7] = 132;
			if (130 + pos + 2 < 179)
				indices[i + 8] = 130 + pos + 2;
			else
				indices[i + 8] = 131;

			pos += 2;
		}

		//skybox:
		GLuint skyboxIndices[] = {
			187, 188, 189,
			187, 189, 190,
			191, 192, 193,
			191, 193, 194,
			195, 196, 197,
			195, 197, 198,
			199, 200, 201,
			199, 201, 202,
			203, 204, 205,
			203, 205, 206,
			207, 208, 209,
			207, 209, 210
		};

		for (int i = 0; i < 36; i++)
			indices[i + (44 + 24 + 48 + 16 + 54) * 3] = skyboxIndices[i];

		GLuint VBO, EBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// vertex geometry data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// vertex color data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// vertex texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// vertex normal coordinates
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		// vertex texture ID
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
		

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "floor.png");
		GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "hammer.png");
		GLuint texture2 = LoadMipmapTexture(GL_TEXTURE2, "hammer_side.png");
		GLuint texture3 = LoadMipmapTexture(GL_TEXTURE3, "wood.png");
		GLuint texture4 = LoadMipmapTexture(GL_TEXTURE4, "mold.png");
		GLuint texture5 = LoadMipmapTexture(GL_TEXTURE5, "form.png");
		GLuint texture6 = LoadMipmapTexture(GL_TEXTURE6, "bar.png");
		GLuint texture7 = LoadMipmapTexture(GL_TEXTURE7, "triangle.png");
		GLuint skyboxTex = LoadMipmapTexture(GL_TEXTURE8, "skybox.png");
		GLuint grass = LoadMipmapTexture(GL_TEXTURE9, "grass.png");

		bool goingUp = true, goingToRight = true;
		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie bufora koloru i bufora glebokosci

			// Bind Textures using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture0);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture0"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture1"), 1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture2"), 2);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, texture3);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture3"), 3);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, texture4);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture4"), 4);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, texture5);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture5"), 5);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, texture6);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture6"), 6);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, texture7);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture7"), 7);
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, skyboxTex);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Skybox"), 8);
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, grass);
			glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Grass"), 9);


			glm::mat4 rotate;
			static GLfloat rot_angle = 0.0f;
			rotate = glm::translate(rotate, glm::vec3(-1.2f, 0.8f, 0.0f));
			rotate = glm::rotate(rotate, glm::radians(rot_angle), glm::vec3(0.0, 0.0, 1.0)); //obrot wokol osi Z
			rotate = glm::translate(rotate, glm::vec3(1.2f, -0.8f, 0.0f));
			if (goingUp)
				rot_angle += rotate_speed; //predkosc obrotu
			else
				rot_angle -= rotate_speed;
			if (rot_angle >= 90.0f)
				goingUp = false;
			if (rot_angle <= 0.0f)
				goingUp = true;
			GLuint rotateLoc = glGetUniformLocation(theProgram.get_programID(), "rotate");
			glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, glm::value_ptr(rotate));


			glm::mat4 trans;
			static GLfloat trans_z = 0.0f;
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -trans_z));
			if (goingToRight)
				trans_z += translate_speed;
			else
				trans_z -= translate_speed;
			if (trans_z >= 0.6f)
			{
				goingToRight = false;
				goingUp = true;
				trans_z = 0.6f;
				rot_angle = 0.0f;
			}
			if (trans_z <= 0.0f)
			{
				goingToRight = true;
				goingUp = true;
				trans_z = 0.0f;
				rot_angle = 0.0f;
			}
			GLuint translateLoc = glGetUniformLocation(theProgram.get_programID(), "translate");
			glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(trans));

			//----------------
			//glm::mat4 camRot;
			//camRot = glm::rotate(camRot, glm::radians(rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
			//glm::vec3 cameraPos = glm::vec3(camRot * glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
			glm::vec3 cameraPos = glm::vec3(posX, posY, posZ);

			glUniform3fv(glGetUniformLocation(theProgram.get_programID(), "viewPos"), 1, glm::value_ptr(cameraPos)); //chyba niepotrzebne
			//----------------

			glm::mat4 view;
			glm::mat4 projection;

			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f)); //to zamiast glm::lookAt i linijek miedzy kreskami
			//view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::lookAt(cameraPos, glm::vec3(lookX, lookY, lookZ), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

			GLint viewLoc = glGetUniformLocation(theProgram.get_programID(), "view");
			GLint projectionLoc = glGetUniformLocation(theProgram.get_programID(), "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



			// Draw our first triangle
			theProgram.Use();

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}
