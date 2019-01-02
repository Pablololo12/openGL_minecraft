#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"
#include "ppm.h"
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#define __gl_h_
	#include <OpenGL/glu.h>
	//#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	//#include <GL/freeglut.h>
#endif
#include <GLFW/glfw3.h>

int WIDTH = 640;
int HEIGHT = 480;
GLFWwindow* win;

GLuint vao;
GLuint simple_frag;
GLuint simple_vert;
GLuint prog_simple;
GLuint view_loc_frag;
GLuint light_pos_frag;
GLuint simple_texture_loc;
GLuint textureID;
const GLint samplers[1] = {0};

float pers_matrix[16] = {0};
float camera_matrix[16] = {0};
float view_matrix[16] = {0};

float pos[3] = {2.0, 1.5, -2.0};
float pitch = 0.0;
float yaw = 90.0;
int keys_pressed[5] = {0};

static float cube[108] = {0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.1, 0.1 ,0.0,
			0.0, 0.0, 0.0, 0.1, 0.1, 0.0, 0.0, 0.1, 0.0, //BOTTOM
			0.0, 0.0, 0.1, 0.1, 0.0, 0.1, 0.1, 0.1 ,0.1,
			0.0, 0.0, 0.1, 0.1, 0.1, 0.1, 0.0, 0.1, 0.1, //TOP
			0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.1, 0.0, 0.1,
			0.0, 0.0, 0.0, 0.1, 0.0, 0.1, 0.0, 0.0, 0.1, //BACK
			0.0, 0.1, 0.0, 0.1, 0.1, 0.0, 0.1, 0.1, 0.1,
			0.0, 0.1, 0.0, 0.1, 0.1, 0.1, 0.0, 0.1, 0.1, //FRONT

			0.0, 0.1, 0.0, 0.0, 0.1, 0.1, 0.0, 0.0, 0.1,
			0.0, 0.1, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, //LEFT
			
			0.1, 0.1, 0.0, 0.1, 0.1, 0.1, 0.1, 0.0, 0.1,
			0.1, 0.1, 0.0, 0.1, 0.0, 0.1, 0.1, 0.0, 0.0}; //right

static float normals[108] = {0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
			0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
			0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
			0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
			0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
			-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
			-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
			1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
			1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

static float uv[72] = {
	0.335973f, 1.0f-0.000000f,
	0.667979f, 1.0f-0.000000f,
	0.667979f, 1.0f-0.335973f,
	0.335973f, 1.0f-0.000000f,
	0.667979f, 1.0f-0.335973f,
	0.335973f, 1.0f-0.335973f, //front

	0.667979f, 1.0f-0.000000f,
	1.000000f, 1.0f-0.000000f,
	1.000000f, 1.0f-0.335973f,
	0.667979f, 1.0f-0.000000f,
	1.000000f, 1.0f-0.335973f,
	0.667979f, 1.0f-0.335973f, //front

	0.0,0.0,
	0.0,0.0,
	0.0,0.0,
	0.0,0.0,
	0.0,0.0,
	0.0,0.0, //Bottom
	0.667979f, 1.0f-0.335973f,
	1.000000f, 1.0f-0.335973f,
	1.000000f, 1.0f-0.667979f,
	0.667979f, 1.0f-0.335973f,
	1.000000f, 1.0f-0.667979f,
	0.667979f, 1.0f-0.667979f, //top

	0.335973f, 1.0f-0.667979f,
	0.667979f, 1.0f-0.667979f,
	0.667979f, 1.0f-0.335973f,
	0.335973f, 1.0f-0.667979f,
	0.667979f, 1.0f-0.335973f,
	0.335973f, 1.0f-0.335973f, // Left

	0.000000f, 1.0f-0.667979f,
	0.335973f, 1.0f-0.667979f,
	0.335973f, 1.0f-0.335973f,
	0.000000f, 1.0f-0.667979f,
	0.335973f, 1.0f-0.335973f,
	0.000000f, 1.0f-0.335973f //right
};

int size_of_world = 256;
float *world_cubes;
float *world_normals;
float *world_uvs;
int *world_height;

void keyboard(GLFWwindow* win,int key,int s,int act,int mod);
static void cursor_pos_callback(GLFWwindow* win, double xpos, double ypos);
int world_display();
void ogl_display(GLFWwindow* win);
void ogl_reshape(GLFWwindow* win,int w,int h);

inline void normalize(float vec[3])
{
	float mod = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
	vec[0] = vec[0]/mod;
	vec[1] = vec[1]/mod;
	vec[2] = vec[2]/mod;
}

void crossProduct(float a[3],float b[3], float c[3])
{
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

// Function to check for errors
void glcheck(const char* msg)
{
	GLenum err;
	err = glGetError();
	switch(err)
	{
		case GL_NO_ERROR:
		break;
		case GL_INVALID_ENUM:
		printf("%s error: Invalid enun\n", msg);
		break;
		case GL_INVALID_VALUE:
		printf("%s error: Invalid ID value\n", msg);
		break;
		case GL_INVALID_OPERATION:
		printf("%s error: Invalid operation\n", msg);
		break;
		case GL_OUT_OF_MEMORY:
		printf("%s error: Out of memory\n", msg);
		break;
		default:
		printf("%s error: Undefined\n", msg);
		break;
	}
}

char* get_shader(const char *name)
{
	FILE *fp;
	int size;
	char *content = (char*) calloc(2048,sizeof(char));
	fp = fopen(name, "r");
	if (fp == NULL) {
		printf("Error opening shader\n");
		return NULL;
	}
	size = fread(content, sizeof(char), 2048, fp);
	if (size >= 2048) {
		printf("Error reading shader file (TOO LARGE)\n");
		return NULL;
	}
	fclose(fp);
	return content;
}

int create_cube(float x, float y, float z, float* pointer, float* pointer2, float* pointer3)
{
	int i;

	for (i=0; i<108; i=i+3) {
		pointer[i] = cube[i] + x;
		pointer[i+1] = cube[i+1] + y;
		pointer[i+2] = cube[i+2] + z;

		pointer2[i] = normals[i];
		pointer2[i+1] = normals[i+1];
		pointer2[i+2] = normals[i+2];
	}
	for (i=0; i<72; i+=2) {
		pointer3[i] = uv[i];
		pointer3[i+1] = uv[i+1];
	}
	return 0;
}

int generate_world(int size)
{
	int i,d;
	world_cubes = (float *)malloc(108*sizeof(float)*size*size);
	world_normals = (float *)malloc(108*sizeof(float)*size*size);
	world_height = (int *)malloc(size*size*sizeof(int));
	world_uvs = (float *)malloc(size*size*sizeof(float)*72);
	for (i=0; i<size; ++i) {
		for (d=0; d<size; ++d) {
			world_height[i*size+d] = (int)((sin(i*0.1) * sin(d*0.1) * 5.0) + 5.0);
		}
	}
	for (i=0; i<size; ++i) {
		for (d=0; d<size; ++d) {
			create_cube(i*0.1f, world_height[i*size+d]*0.1f, -d*0.1f, &world_cubes[i*size*108+d*108], &world_normals[i*size*108+d*108], &world_uvs[i*size*72+d*72]);
		}
	}
	return 0;
}

int init_buffers()
{
	generate_world(size_of_world);
	vao = 0;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	// Vertex and triangles of the OBJ
	GLuint vpbo = 0;
	glGenBuffers(1,&vpbo);
	glBindBuffer(GL_ARRAY_BUFFER,vpbo);
	glBufferData(GL_ARRAY_BUFFER,108*sizeof(float)*size_of_world*size_of_world,world_cubes,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,NULL);
	glcheck("vpbo");

	GLuint vnbo = 0;
	glGenBuffers(1,&vnbo);
	glBindBuffer(GL_ARRAY_BUFFER,vnbo);
	glBufferData(GL_ARRAY_BUFFER,108*sizeof(float)*size_of_world*size_of_world,world_normals,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,NULL);
	glcheck("vnbo");

	GLuint vnuv = 0;
	glGenBuffers(1,&vnuv);
	glBindBuffer(GL_ARRAY_BUFFER,vnuv);
	glBufferData(GL_ARRAY_BUFFER,72*sizeof(float)*size_of_world*size_of_world,world_uvs,GL_STATIC_DRAW);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(float)*2,NULL);
	glcheck("vnuv");

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
	int wi, he;
	float * img = open_image("./tex/ground.ppm", &wi, &he);
	if (img == NULL) {
		printf("Error opening texture\n");
		return 0;
	}
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,wi,he,0,GL_RGB,GL_FLOAT,img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
	glcheck("texture");


	char *simple_vertex_src = get_shader("shaders/vertex.vert");
	char *simple_fragment_src = get_shader("shaders/fragment.frag");
	if (simple_fragment_src==NULL || simple_vertex_src==NULL) return 0;

	simple_vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(simple_vert,1,(const GLchar **)&simple_vertex_src,NULL);
	glCompileShader(simple_vert);
	glcheck("Vertex Shader");

	simple_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(simple_frag,1,(const GLchar **)&simple_fragment_src,NULL);
	glCompileShader(simple_frag);
	glcheck("Fragment Shader");

	prog_simple = glCreateProgram();
	glAttachShader(prog_simple,simple_vert);
	glAttachShader(prog_simple,simple_frag);
	glLinkProgram(prog_simple);
	glcheck("Prog");
	GLint program_valid = 0;
	glValidateProgram(prog_simple);
	glGetProgramiv(prog_simple, GL_VALIDATE_STATUS, &program_valid);
	if (GL_TRUE != program_valid) {
		printf("Program validation failed.\n");
	}

	view_loc_frag = glGetUniformLocation(prog_simple, "view");
	light_pos_frag = glGetUniformLocation(prog_simple, "light");
	simple_texture_loc = glGetUniformLocation(prog_simple, "textur");
	free(simple_fragment_src);
	free(simple_vertex_src);
	glcheck("Location");
	glClearColor(0.0,0.3,0.9,0.0);
	glcheck("Clear");
	return 1;
}

int init_engine()
{
	int i;
	int vers[4] = {42,41,40,33};

	printf("Initializing OpenGL\n");
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

	for (i=0; i<4; i++) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,vers[i]/10);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,vers[i]%10);
		win = glfwCreateWindow(WIDTH,HEIGHT,"OpenGL",NULL,NULL);
		if (win!=NULL)
			break;
	}

	if (win==NULL) {
		printf("Error creating Window\n");
		return 0;
	}
	glfwMakeContextCurrent(win);

	printf("GL: %s\nSL: %s\n",glGetString(GL_VERSION),
				glGetString(GL_SHADING_LANGUAGE_VERSION));
	glfwSetKeyCallback(win,keyboard);
	glfwSetWindowRefreshCallback(win,ogl_display);
	glfwSetCursorPosCallback(win, cursor_pos_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(win,ogl_reshape);
	if(!init_buffers()) return 0;
	return 1;
}

int* get_height(int *size)
{
	*(size) = size_of_world;
	return world_height;
}

int get_keys(int* k)
{
	memcpy(k, keys_pressed, sizeof(int)*5);
	return 1;
}

int set_pos(float x, float y, float z)
{
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
	return 0;
}

int get_dir(float* ya)
{
	*(ya) = yaw;

	return 0;
}

void setWindowFPS (GLFWwindow* win)
{
	char buffer[32];
	static int nbFrames = 0;
	static double lastTime = 0;
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	nbFrames++;
	if ( delta >= 1.0 ){
		double fps = (double)(nbFrames) / delta;
		snprintf(buffer,32,"[%.3f]",fps);
		glfwSetWindowTitle(win, buffer);
		nbFrames = 0;
		lastTime = currentTime;
	}
}

int display(double *time)
{
	if (glfwWindowShouldClose(win)) return 1;
	glfwWaitEventsTimeout(0.0166);

	ogl_display(win);
	setWindowFPS(win);
	*(time) = glfwGetTime();
	return 0;
}

int end_engine()
{
	glfwTerminate();
	return 0;
}

void ogl_display(GLFWwindow* win)
{
	glfwMakeContextCurrent(win);
	glcheck("Make context");
	world_display();

	glfwSwapBuffers(win);
	glcheck("Swap");
}

int refresh()
{
	ogl_display(win);
	return 0;
}

void ogl_reshape(GLFWwindow* win,int w,int h)
{
	glfwMakeContextCurrent(win);
	WIDTH = w;
	HEIGHT = h;
	glViewport(0,0,WIDTH,HEIGHT);
}

static void cursor_pos_callback(GLFWwindow* win, double xpos, double ypos)
{
	static double xprev = 0.0;
	static double yprev = 0.0;
	static int first = 1;
	double xdiff = 0.0;
	double ydiff = 0.0;
	if (first==0) {
		xdiff = xpos - xprev;
		ydiff = ypos - yprev;
	}
	first = 0;
	xprev = xpos;
	yprev = ypos;
	yaw += xdiff * 0.5;
	pitch += ydiff * 0.5;
	if (pitch > 88.0) {
		pitch = 88.0;
	} else if (pitch < -88.0) {
		pitch = -88.0;
	}
}

void keyboard(GLFWwindow* win,int key,int s,int act,int mod)
{
	if (act==GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_W:
			keys_pressed[0]=0;
			break;
		case GLFW_KEY_S:
			keys_pressed[1]=0;
			break;
		case GLFW_KEY_A:
			keys_pressed[2]=0;
			break;
		case GLFW_KEY_D:
			keys_pressed[3]=0;
			break;
		case GLFW_KEY_SPACE:
			keys_pressed[4]=0;
			break;
		}
	} else {
		switch (key)
		{
		case GLFW_KEY_W:
			keys_pressed[0]=1;
			break;
		case GLFW_KEY_S:
			keys_pressed[1]=1;
			break;
		case GLFW_KEY_A:
			keys_pressed[2]=1;
			break;
		case GLFW_KEY_D:
			keys_pressed[3]=1;
			break;
		case GLFW_KEY_SPACE:
			keys_pressed[4]=1;
			break;
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win,GL_TRUE);
			break;
		default:
			printf("Unkown Key %d\n", key);
			break;
		}
	}
}

// Presentación de graficos
int init_pers_matrix(float aspect)
{
	float rangeMin = 0.01f;
	float rangeMax = 1000.0f;
	float range = rangeMax-rangeMin;
	float tanHalfFOV = tanf(toRad(60.0 / 2.0));
	pers_matrix[0] = 1/(aspect*tanHalfFOV);
	pers_matrix[5] = 1/(tanHalfFOV);
	pers_matrix[10] = -(rangeMax + rangeMin) / range;
	pers_matrix[11] = -1.0f;
	pers_matrix[14] = -(2.0f * rangeMax * rangeMin) / range;
	return 0;
}

int lookAt(float eye[3], float center[3], float up[3])
{
	float f[3];
	f[0] = center[0]-eye[0];
	f[1] = center[1]-eye[1];
	f[2] = center[2]-eye[2];
	normalize(f);
	float s[3];
	float u[3];
	crossProduct(f,up,s);
	normalize(s);
	crossProduct(s,f,u);
	camera_matrix[0] = s[0];
	camera_matrix[4] = s[1];
	camera_matrix[8] = s[2];
	camera_matrix[12] = s[0]*-eye[0]+s[1]*-eye[1]+s[2]*-eye[2];
	camera_matrix[1] = u[0];
	camera_matrix[5] = u[1];
	camera_matrix[9] = u[2];
	camera_matrix[13] = u[0]*-eye[0]+u[1]*-eye[1]+u[2]*-eye[2];
	camera_matrix[2] = -f[0];
	camera_matrix[6] = -f[1];
	camera_matrix[10] = -f[2];
	camera_matrix[14] = f[0]*eye[0]+f[1]*eye[1]+f[2]*eye[2];
	camera_matrix[3] = 0; //3
	camera_matrix[7] = 0; //7
	camera_matrix[11] = 0; //11
	camera_matrix[15] = 1.0f;
	return 0;
}

// https://learnopengl.com/Getting-started/Camera
// http://paulbourke.net/geometry/circlesphere/
int init_camera_matrix()
{
	int i,d;
	float dir[3] = {0.0, 0.0, 1.0};
	float up[3] = {0.0, 1.0, 0.0};
	dir[0] = cos(toRad(pitch))*cos(toRad(yaw));
	dir[2] = cos(toRad(pitch))*sin(toRad(yaw));
	dir[1] = sin(toRad(pitch));
	dir[0] = pos[0]-dir[0];
	dir[1] = pos[1]-dir[1];
	dir[2] = pos[2]-dir[2];
	lookAt(pos,dir,up);
	return 0;
}

int multiply_matrix(float m1[16], float m2[16], float m3[16])
{
	int i,d,f;
	float acum;
	for (i=0; i<4; i++) {
		for (d=0; d<4; d++) {
			acum = 0.0;
			for (f=0; f<4; f++) {
				acum += m1[i*4+f] * m2[f*4+d];
			}
			m3[i*4+d] = acum;
		}
	}
}

int init_camera()
{
	return 0;
}

int world_display()
{
	int i,d;
	float aspect = (float) WIDTH/(float) HEIGHT;
	float cam[3] = {pos[0],pos[1],pos[2]-0.2};
	init_pers_matrix(aspect);
	init_camera_matrix();
	
	multiply_matrix(camera_matrix, pers_matrix, view_matrix);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glcheck("Depth test");
	glDepthFunc(GL_LESS);
	glcheck("GL_LESS");
	glEnable(GL_MULTISAMPLE);
	glcheck("Multi");
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glcheck("Depth test");

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glcheck("Bind FrameBuffer");
	glUseProgram(prog_simple);
	glcheck("Using Program");
	glUniformMatrix4fv(view_loc_frag,1,GL_FALSE,view_matrix);
	glUniform3fv(light_pos_frag, 1, pos);
	glUniform1iv(simple_texture_loc, 1, samplers);
	glcheck("View_matrix");
	glBindVertexArray(vao);
	glcheck("View_matrix");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glcheck("View_matrix");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glcheck("mano");
	glDrawArrays(GL_TRIANGLES,0,108*size_of_world*size_of_world);
	return 0;
}
