#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

// scaling

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
attribute vec4 aRef; \n\
varying vec4 vColor; \n\
uniform vec4 uP; \n\
uniform float uTheta; \n\
void main(void) { \n\
  gl_Position.x = aRef.x * cos(uTheta) - aRef.y * sin(uTheta)+uP.x; \n\
  gl_Position.y = aRef.x * sin(uTheta) + aRef.y * cos(uTheta)+uP.y; \n\
  gl_Position.z = 0.0; \n\
  gl_Position.w = 1.0; \n\
  vColor = aColor; \n\
}";
/*
// scaling based on uP

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform vec4 uP; \n\
uniform float uScale; \n\
float sPoint;\n\
void main(void) { \n\
  gl_Position.x = uScale*aPosition.x - uScale*uP.x + uP.x; \n\
  gl_Position.y = uScale*aPosition.y - uScale*uP.y + uP.y; \n\
  gl_Position.z = uScale*aPosition.z - uScale*uP.z + uP.z; \n\
  gl_Position.w = 1.0; \n\
  vColor = aColor; \n\
}";


static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform vec4 uP; \n\
uniform float uScale; \n\
void main(void) { \n\
  gl_Position.xyz = uScale*aPosition.xyz - uScale*uP.xyz + uP.xyz; \n\
  gl_Position.w = 1.0; \n\
  vColor = aColor; \n\
}";
*/

static char* fsSource = "#version 120 \n\
varying vec4 vColor; \n\
void main(void) { \n\
  gl_FragColor = vColor; \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

GLfloat vertices[] = {
	0.3, 0.7, 0.0, 1.0, // 0
	0.7, 0.3, 0.0, 1.0, // 1
	0.3, 0.3, 0.0, 1.0, // 2
	0.5, 0.7, 0.0, 1.0  // 3

};
GLfloat refs[] = {
	0.3, 0.7, 0.0, 1.0, // 0
	0.7, 0.3, 0.0, 1.0, // 1
	0.3, 0.3, 0.0, 1.0, // 2
	0.5, 0.7, 0.0, 1.0  // 3

};
GLfloat colors[] = {
	1.0, 0.0, 0.0, 1.0, // black
	0.0, 1.0, 0.0, 1.0, // blue
	0.0, 0.0, 1.0, 1.0, // green
	1.0, 1.0, 1.0, 1.0, // cyan
};

GLushort indices[] = {

	0, 2, 3,
	3, 1, 0,
};

GLuint vbo[1];

char buf[1024];
float theta = 0.0;
GLfloat scale = 1.0, P[4];
GLfloat mat[16];

void myinit(void) {
	GLuint status;
	GLuint loc;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glValidateProgram(prog);
	glUseProgram(prog);
	for (int i = 0; i < 4; i++) {
		refs[4 * i + 0] = vertices[4 * i + 0]-vertices[0];
		refs[4 * i + 1] = vertices[4 * i + 1]-vertices[1];
	}
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), 4 * 4 * sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, 4 * 8 * sizeof(GLfloat), 4 * 4 * sizeof(GLfloat), refs);

	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(4 * 4 * sizeof(GLfloat)));
	loc = glGetAttribLocation(prog, "aRef");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(4 * 8 * sizeof(GLfloat)));

	glEnable(GL_DEPTH_TEST);
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+': theta += M_PI/6;
		break;
	case '-': theta -= M_PI/6;
		break;
	case 27: // ESCAPE
		exit(0);
		break;
	}
	glutPostRedisplay();
}


void mydisplay(void) {
	GLuint loc;

	glClearColor(0.3, 0.3, 0.3, 1.0); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	P[0] = vertices[0];
	P[1] = vertices[1];
	P[2] = vertices[2];
	P[3] = vertices[3];
	printf("%f %f %f %f\n", P[0], P[1], P[2], P[3]);
	/*loc = glGetUniformLocation(prog, "uScale");
	glUniform1f(loc, scale);*/
	loc = glGetUniformLocation(prog, "uTheta");
	glUniform1f(loc, theta);
	loc = glGetUniformLocation(prog, "uP");
	glUniform4fv(loc, 1, P);

	glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_SHORT, indices);

	glFlush();

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("��ǻ���к� 2014105011 �赿��");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);

	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
