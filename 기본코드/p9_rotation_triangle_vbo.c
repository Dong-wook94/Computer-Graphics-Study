#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform float uTheta; \n\
void main(void) { \n\
  gl_Position.z = aPosition.z * cos(uTheta) - aPosition.x * sin(uTheta); \n\
  gl_Position.x = aPosition.z * sin(uTheta) + aPosition.x * cos(uTheta); \n\
  gl_Position.yw = aPosition.yw; \n\
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

GLfloat vertices[] = {
	-0.5, -0.5, -0.5, 1.0, // 0
	-0.5, -0.5, +0.5, 1.0, // 1
	-0.5, +0.5, -0.5, 1.0, // 2
	-0.5, +0.5, +0.5, 1.0, // 3
	+0.5, -0.5, -0.5, 1.0, // 4
	+0.5, -0.5, +0.5, 1.0, // 5
	+0.5, +0.5, -0.5, 1.0, // 6
	+0.5, +0.5, +0.5, 1.0, // 7
};

GLfloat colors[] = {
	0.5, 0.5, 0.5, 1.0, // black
	0.5, 0.5, 1.0, 1.0, // blue
	0.5, 1.0, 0.5, 1.0, // green
	0.5, 1.0, 1.0, 1.0, // cyan
	1.0, 0.5, 0.5, 1.0, // red
	1.0, 0.5, 1.0, 1.0, // magenta
	1.0, 1.0, 0.5, 1.0, // yellow
	1.0, 1.0, 1.0, 1.0, // white
};

GLushort indices[] = { // 36 points, 12 triangles
	0, 4, 6,
	6, 2, 0,
	4, 5, 7,
	7, 6, 4,
	1, 3, 7,
	7, 5, 1,
	0, 2, 3,
	3, 1, 0,
	2, 6, 7,
	7, 3, 2,
	0, 1, 5,
	5, 4, 0,
};

GLuint vbo[1];

char buf[1024];

void myinit(void) {
	GLuint status;
	GLuint loc;
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

	
	// VBO setting
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 8 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(GLfloat), 8 * 4 * sizeof(GLfloat), colors);

	
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * 4 * sizeof(GLfloat)));
	
	/*
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, colors);
	*/


	// depth buffer enabled
	glEnable(GL_DEPTH_TEST);
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

float theta = 0.0;

void myidle(void) {
	theta += 0.0005; // radian
	glutPostRedisplay();
}

void mydisplay(void) {
	GLuint loc;
	// clear 
	glClearColor(0.3, 0.3, 0.3, 1.0); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// provide the theta
	loc = glGetUniformLocation(prog, "uTheta");
	glUniform1f(loc, theta);
	// draw a triangle
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, indices);
	// flush all
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutIdleFunc(myidle);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
