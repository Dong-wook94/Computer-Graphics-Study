#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
void main(void) { \n\
  gl_Position = aPosition; \n\
  vColor = aColor; \n\
}";

static char* fsSource = "#version 120 \n\
varying vec4 vColor; \n\
void main(void) { \n\
  gl_FragColor = vColor; \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

char buf[1024];

void myinit(void) {
	GLuint status;
	// vs: vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs); // compile to get .OBJ
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	printf("vs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vs, sizeof(buf), NULL, buf);
	printf("vs log = [%s]\n", buf);
	// fs: fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs); // compile to get .OBJ
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	printf("fs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(fs, sizeof(buf), NULL, buf);
	printf("fs log = [%s]\n", buf);
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog); // link to get .EXE
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("program link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("program validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);
	glUseProgram(prog); // execute it !
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1': printf("key 1\n");
		break;
	case '2': printf("key 2 \n");
		break;
	case 27: // ESCAPE
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
};

GLfloat colors[] = {
	1.0, 0.0, 0.0, 1.0, // red
	0.0, 1.0, 0.0, 1.0, // green
	0.0, 0.0, 1.0, 1.0, // blue
};

void mydisplay(void) {
	GLuint loc;
	// clear 
	glClearColor(0.3, 0.3, 0.3, 1.0); // gray
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, colors);
	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// flush all
	glFlush();
}

void mymouse (int button, int state, int x, int y)
{
	printf("(%d, %d)\n", x, y);
}

void mymenu(int id)
{
	switch (id) {
		case 1: printf("menu 1\n");
			break;
		case 2: printf("menu 2\n");
			break;
	}
}
void myreshape(int x, int y)
{
	printf("reshaped (%d, %d)\n", x, y);
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutMouseFunc(mymouse);
	glutReshapeFunc(myreshape);
	glutCreateMenu(mymenu);
	glutAddMenuEntry("select A", 1);
	glutAddMenuEntry("select B", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
