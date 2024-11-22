
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "LadeShader.h"

#include <stdlib.h>

#define PI 3.1415926536

struct myVertexType //selbst definierter Datentyp für die Shaderprogramme
{
	float x, y, z;
	float r, g, b;


	myVertexType(myVertexType other, vec3 norm)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		r = other.r;
		g = other.g;
		b = other.b;

	}
	myVertexType(float x, float y, float z, float r, float g, float b)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;



	}
	myVertexType() { x = y = z = r = g = b = 0; }

};

myVertexType vertices[6];//Array mit drei Vertexen

float ambfac = 0.7;

//Key-Callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != 0)//nur beim Hinunterdrücken einer Taste
	{
		if (key == GLFW_KEY_A)//Wenn diese Taste "A" war
		{
			if (mods == 1)//Wenn Shift zusätzlich gehalten ist
				ambfac += .1f;//ambienten Faktor erhöhen
			else
				ambfac -= .1f;//ambienten Faktor senken
		}
	}
}

int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, complete_shader_program;
	GLint  matrix_access, ambient, position_access, color_access;

	myVertexType cubeEdges[8]
	{
		/*0*/myVertexType(+1.0f,+1.0f,+1.0f,1.0f,0.0f,0.0f),//Koordinate 1,1,1 mit roter Farbe
		/*1*/myVertexType(+1.0f,+1.0f,-1.0f,0.0f,1.0f,0.0f),//Koordinate 1,1,-1 mit grüner Farbe
		/*2*/myVertexType(+1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f),//blau
		/*3*/myVertexType(+1.0f,-1.0f,+1.0f,1.0f,0.0f,1.0f),//magenta
		/*4*/myVertexType(-1.0f,+1.0f,+1.0f,0.0f,1.0f,1.0f),//cyan
		/*5*/myVertexType(-1.0f,-1.0f,+1.0f,1.0f,1.0f,0.0f),//gelb
		/*6*/myVertexType(-1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f),//weiß
		/*7*/myVertexType(-1.0f,+1.0f,-1.0f,0.0f,0.0f,0.0f) //schwarz
	};

	//verkleinere Würfelkoordinaten
	for (int i = 0; i < 8; i++)
	{
		cubeEdges[i].x *= 0.5;
		cubeEdges[i].y *= 0.5;
		cubeEdges[i].z *= 0.5;
	}

	//linke Seite erstes Dreieck
	vertices[0] = cubeEdges[2];
	vertices[1] = cubeEdges[1];
	vertices[2] = cubeEdges[0];



	if (!glfwInit())//GLFW initialisieren
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(1280, 1024, "Happy Cube", NULL, NULL);//Fenster mit 1280x1024 Pixel erstellen
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);//Key-Callback setzten (Funktion oben)
	glfwMakeContextCurrent(window);//Verbindung mit dem Fenster
	gladLoadGL(glfwGetProcAddress);//Laden der Fuktionszeiger

	//Erzeugen des Vertex-Shaders
	char* vertex_shader_code = readTextFileIntoString("Shaders/VertexShaderStart.glsl");//Shader aus Datei laden (LadeShader.h)
	if (vertex_shader_code == 0) exit(EXIT_FAILURE);// Sicherstellen, dass die Datei erfolgreich geladen wurde!
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	char info[500];
	int num;
	glGetShaderInfoLog(vertex_shader, 500, &num, info);
	printf("%s", info);

	//Erzeugen des Fragment-Shaders
	char* fragment_shader_code = readTextFileIntoString("Shaders/FragmentShaderStart.glsl");//Shader aus Datei laden (LadeShader.h)
	if (fragment_shader_code == 0) exit(EXIT_FAILURE);// Sicherstellen, dass die Datei erfolgreich geladen wurde!
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderInfoLog(fragment_shader, 500, &num, info);
	printf("%s", info);

	//Linken des Shader-Progamms
	complete_shader_program = glCreateProgram();
	glAttachShader(complete_shader_program, vertex_shader);
	glAttachShader(complete_shader_program, fragment_shader);
	glLinkProgram(complete_shader_program);

	//Uniforme Positionen für den Zugriff während des Renderns
	matrix_access = glGetUniformLocation(complete_shader_program, "matrix");
	ambient = glGetUniformLocation(complete_shader_program, "ambientFactor");


	//Zugriff auf Position und Farbe innerhalb des Vertex-Buffers
	position_access = glGetAttribLocation(complete_shader_program, "position");
	color_access = glGetAttribLocation(complete_shader_program, "color");

	//Aufbau und Übermittlung des Vertex-Buffers
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Die Daten für Position und Farbe werden dem Shader mitgeteilt
	glEnableVertexAttribArray(position_access);
	glVertexAttribPointer(position_access, 3, GL_FLOAT, GL_FALSE,
		sizeof(myVertexType), (void*)0);

	glEnableVertexAttribArray(color_access);
	glVertexAttribPointer(color_access, 3, GL_FLOAT, GL_FALSE,
		sizeof(myVertexType), (void*)(sizeof(float) * 3));


	//Z-Achse positiv defineren
	glClearDepth(0);
	glDepthFunc(GL_GREATER);//größere Z-Werte überschreiben kleinere
	glEnable(GL_DEPTH_TEST);//Verdeckung der Dreiecke untereinander aktivieren

	while (!glfwWindowShouldClose(window))//Hauptscheife
	{
		mat4x4 m;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(complete_shader_program);//Ab jetzt dieses Shader-Programm nutzen

		//Rotationsmatrix erstellen
		mat4x4_identity(m);//Über die Linmath.h wird eine 4x4 Einheitsmatrix erzeugt
		//	mat4x4_rotate_Z(m, m, (float)glfwGetTime());//Auf die Matrix m wird eine Rotation um Z aufmultipliziert
		//	mat4x4_rotate_Y(m, m, 10*2*PI/360/*(float)glfwGetTime() / 100*/);//Auf die Matrix m wird eine Rotation um Y aufmultipliziert
		mat4x4_rotate_Y(m, m, 2 * PI * 10 / 360 * sin((float)glfwGetTime()));
		glUniformMatrix4fv(matrix_access, 1, GL_FALSE, (const GLfloat*)m);// Die Matrix wird in das Shader-Programm übertragen

		glUniform1f(ambient, ambfac);// Der ambiente Faktor wird in das Shader-Programm übertragen

		glDrawArrays(GL_TRIANGLES, 0, 3);//Ein Dreieck wird gezeichnet

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


