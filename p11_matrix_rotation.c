#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform mat4 uModelview; \n\
void main(void) { \n\
  gl_Position = uModelview * aPosition; \n\
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
	// depth buffer enabled
	glEnable(GL_DEPTH_TEST);
}

GLfloat dirX = 0.0;
GLfloat dirY = 0.0;
GLfloat dirZ = 0.0;

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	case 'q': dirX = -1.0; break;
	case 'w': dirX =  0.0; break;
	case 'e': dirX = +1.0; break;
	case 'a': dirY = -1.0; break;
	case 's': dirY =  0.0; break;
	case 'd': dirY = +1.0; break;
	case 'z': dirZ = -1.0; break;
	case 'x': dirZ =  0.0; break;
	case 'c': dirZ = +1.0; break;
	}
}

GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat angleZ = 0.0;

void myidle(void) {
	const GLfloat delta = 0.01;
	angleX += dirX * delta;
	angleY += dirY * delta;
	angleZ += dirZ * delta;
	glutPostRedisplay();
}

GLfloat mat[16]; // mat = reg * rotZ = rotX * rotY * rotZ
GLfloat reg[16]; // reg = rotX * rotY
GLfloat rotX[16];
GLfloat rotY[16];
GLfloat rotZ[16];

void matMult(GLfloat c[16], GLfloat a[16], GLfloat b[16]) {
	c[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	c[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	c[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	c[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	//
	c[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	c[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	c[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	c[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	//
	c[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	c[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	c[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	c[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	//
	c[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
	c[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
	c[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	c[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

void mydisplay(void) {
	GLuint loc;
	// clear 
	glClearColor(0.3, 0.3, 0.3, 1.0); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// rotX : rotation about X axis
	rotX[0] = 1.0; rotX[4] = 0.0; rotX[8] = 0.0; rotX[12] = 0.0; 
	rotX[1] = 0.0; rotX[5] = cos(angleX); rotX[9] = -sin(angleX); rotX[13] = 0.0; 
	rotX[2] = 0.0; rotX[6] = sin(angleX); rotX[10] = cos(angleX); rotX[14] = 0.0; 
	rotX[3] = 0.0; rotX[7] = 0.0; rotX[11] = 0.0; rotX[15] = 1.0; 
	// rotY : rotation about Y axis
	rotY[0] = cos(angleY); rotY[4] = 0.0; rotY[8] = sin(angleY); rotY[12] = 0.0; 
	rotY[1] = 0.0; rotY[5] = 1.0; rotY[9] = 0.0; rotY[13] = 0.0; 
	rotY[2] = -sin(angleY); rotY[6] = 0.0; rotY[10] = cos(angleY); rotY[14] = 0.0; 
	rotY[3] = 0.0; rotY[7] = 0.0; rotY[11] = 0.0; rotY[15] = 1.0; 
	//
	rotZ[0] = cos(angleZ); rotZ[4] = -sin(angleZ); rotZ[8] = 0.0; rotZ[12] = 0.0; 
	rotZ[1] = sin(angleZ); rotZ[5] = cos(angleZ); rotZ[9] = 0.0; rotZ[13] = 0.0; 
	rotZ[2] = 0.0; rotZ[6] = 0.0; rotZ[10] = 1.0; rotZ[14] = 0.0; 
	rotZ[3] = 0.0; rotZ[7] = 0.0; rotZ[11] = 0.0; rotZ[15] = 1.0; 
	//
	matMult(reg, rotX, rotY);
	matMult(mat, reg, rotZ);
	// provide the theta
	loc = glGetUniformLocation(prog, "uModelview");
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
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
