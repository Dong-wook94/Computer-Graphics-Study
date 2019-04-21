#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform mat4  uModel; \n\
void main(void) { \n\
  gl_Position = uModel * aPosition; \n\
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
	
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), 4 * 4 * sizeof(GLfloat), colors);

	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(4 * 4 * sizeof(GLfloat)));
	
	glEnable(GL_DEPTH_TEST);
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+': scale = 2.0;
		break;
	case '-': scale = 0.5;
		break;
	case '0': scale = 1.0;
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

	// original model
	P[0] = vertices[0];
	P[1] = vertices[1];
	P[2] = vertices[2];
	P[3] = vertices[3];

	// Identity matrix
	mat[0] = 1;       mat[4] = 0;      mat[8] = 0;         mat[12] = 0;
	mat[1] = 0;       mat[5] = 1;      mat[9] = 0;         mat[13] = 0;
	mat[2] = 0;       mat[6] = 0;      mat[10] = 1;        mat[14] = 0;
	mat[3] = 0;       mat[7] = 0;      mat[11] = 0;        mat[15] = 1;

	loc = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
	glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_SHORT, indices);

/*
	//  scaling 
	mat[0] = scale;   mat[4] = 0;      mat[8] = 0;         mat[12] = 0;
	mat[1] = 0;       mat[5] = scale;  mat[9] = 0;         mat[13] = 0;
	mat[2] = 0;       mat[6] = 0;      mat[10] = scale;    mat[14] = 0;
	mat[3] = 0;       mat[7] = 0;      mat[11] = 0;        mat[15] = 1;


	//  reflection 
	mat[0] = -scale;   mat[4] = 0;      mat[8] = 0;         mat[12] = 0;
	mat[1] = 0;       mat[5] = -scale;  mat[9] = 0;         mat[13] = 0;
	mat[2] = 0;       mat[6] = 0;      mat[10] = scale;    mat[14] = 0;
	mat[3] = 0;       mat[7] = 0;      mat[11] = 0;        mat[15] = 1;


	// Scaling based on the point P
	mat[0] = scale;   mat[4] = 0;      mat[8] = 0;         mat[12] = -scale * P[0] + P[0];
	mat[1] = 0;       mat[5] = scale;  mat[9] = 0;         mat[13] = -scale * P[1] + P[1];
	mat[2] = 0;       mat[6] = 0;      mat[10] = scale;    mat[14] = -scale * P[2] + P[2];
	mat[3] = 0;       mat[7] = 0;      mat[11] = 0;        mat[15] = 1;

	// Reflection based on the point P
	mat[0] = -scale;   mat[4] = 0;      mat[8] = 0;         mat[12] = scale * P[0] + P[0];
	mat[1] = 0;       mat[5] = -scale;  mat[9] = 0;         mat[13] = scale * P[1] + P[1];
	mat[2] = 0;       mat[6] = 0;      mat[10] = scale;    mat[14] = -scale * P[2] + P[2];
	mat[3] = 0;       mat[7] = 0;      mat[11] = 0;        mat[15] = 1;
*/


	loc = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(loc, 1,GL_FALSE, mat);
	
	
	glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_SHORT, indices);
	
	glFlush();
	
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("컴퓨터학부 20190920 홍길동");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);

	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
