#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//static char* vsSource = "#version 120 \n\
//attribute vec4 aPosition; \n\
//attribute vec4 aColor; \n\
//varying vec4 vColor; \n\
//uniform mat4 uModelview; \n\
//void main(void) { \n\
//  gl_Position = uModelview * aPosition; \n\
//  vColor = aColor; \n\
//}";
static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aNormal; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform mat4 uModel; \n\
uniform mat4 uView; \n\
uniform mat4 uProj; \n\
void main(void) { \n\
  vColor = aColor; \n\
  gl_Position = uProj * uView * uModel * aPosition; \n\
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

GLfloat matView[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};
GLfloat matProj[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};
GLfloat matA[16];
GLfloat matB[16];
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
GLfloat eye[3], at[3], up[3];
void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	case 'q': dirX = -1.0; break;
	case 'w': dirX = 0.0; break;
	case 'e': dirX = +1.0; break;
	case 'a': dirY = -1.0; break;
	case 's': dirY = 0.0; break;
	case 'd': dirY = +1.0; break;
	case 'z': dirZ = -1.0; break;
	case 'x': dirZ = 0.0; break;
	case 'c': dirZ = +1.0; break;
	case 'r':eye[0] += 0.01; break;
	case 't':eye[0] -= 0.01; break;
	case 'f':eye[1] += 0.01; break;
	case 'g':eye[1] -= 0.01; break;
	case 'v':eye[2]+= 0.01; break;
	case 'b':eye[2] -= 0.01; break;
	}
}

GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat angleZ = 0.0;

void myidle(void) {
	const GLfloat delta = 0.001;
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

	loc = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
	loc = glGetUniformLocation(prog, "uView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matView);
	loc = glGetUniformLocation(prog, "uProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matProj);
	// draw a triangle
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, indices);
	// flush all
	glFlush();
	glutSwapBuffers();
}
void setViewMat(void) {
	
	GLfloat p[3], n[3], v[3], u[3];
	GLfloat l;
	// set defaults
	eye[0] = 2; eye[1] = -1.5; eye[2] = 1;
	at[0] = 0; at[1] = 0; at[2] = 0;
	up[0] = 0; up[1] = 1; up[2] = 0;
	/*
		printf("input eye: ");
		scanf("%f%f%f", &eye[0], &eye[1], &eye[2]);
		printf("input at: ");
		scanf("%f%f%f", &at[0], &at[1], &at[2]);
		printf("input up: ");
		scanf("%f%f%f", &up[0], &up[1], &up[2]);
	*/
	// p = eye
	p[0] = eye[0];
	p[1] = eye[1];
	p[2] = eye[2];
	// n = (at - eye)
	n[0] = at[0] - eye[0];
	n[1] = at[1] - eye[1];
	n[2] = at[2] - eye[2];
	l = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0] /= l; n[1] /= l; n[2] /= l;//물체방향 단위벡터
	// u = up * n
	u[0] = up[1] * n[2] - up[2] * n[1];
	u[1] = up[2] * n[0] - up[0] * n[2];
	u[2] = up[0] * n[1] - up[1] * n[0];
	l = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
	u[0] /= l; u[1] /= l; u[2] /= l;
	// v = n * u;
	v[0] = n[1] * u[2] - n[2] * u[1];
	v[1] = n[2] * u[0] - n[0] * u[2];
	v[2] = n[0] * u[1] - n[1] * u[0];
	l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= l; v[1] /= l; v[2] /= l;
	//
	// view matrix(
	matA[0] = u[0]; matA[4] = u[1]; matA[8] = u[2]; matA[12] = 0;
	matA[1] = v[0]; matA[5] = v[1]; matA[9] = v[2]; matA[13] = 0;
	matA[2] = n[0]; matA[6] = n[1]; matA[10] = n[2]; matA[14] = 0;
	matA[3] = 0.0; matA[7] = 0.0; matA[11] = 0.0; matA[15] = 1.0;
	//matB는 평행이동(eye만큼)
	matB[0] = 1; matB[4] = 0; matB[8] = 0; matB[12] = -p[0];
	matB[1] = 0; matB[5] = 1; matB[9] = 0; matB[13] = -p[1];
	matB[2] = 0; matB[6] = 0; matB[10] = 1; matB[14] = -p[2];
	matB[3] = 0.0; matB[7] = 0.0; matB[11] = 0.0; matB[15] = 1.0;
	matMult(matView, matA, matB);
}

void setProjMat(void) {
	GLfloat xmin, xmax;
	GLfloat ymin, ymax;
	GLfloat zmin, zmax;
	GLfloat a, b, c, d, e, f;
	// input
	xmin = -1.0; xmax = +1.0;
	ymin = -1.0; ymax = +1.0;
	zmin = 1; zmax = 4;
	/*
		printf("input xmin, xmax: ");
		scanf("%f%f", &xmin, &xmax);
		printf("input ymin, ymax: ");
		scanf("%f%f", &ymin, &ymax);
		printf("input zmin, zmax: ");
		scanf("%f%f", &zmin, &zmax);
	*/
	// projection matrix
	a = (xmax + xmin) / (xmax - xmin);
	b = (ymax + ymin) / (ymax - ymin);
	c = (zmax + zmin) / (zmax - zmin);
	d = -2 * zmax * zmin / (zmax - zmin);
	e = 2 * zmin / (xmax - xmin);
	f = 2 * zmin / (ymax - ymin);
	matProj[0] = e; matProj[4] = 0; matProj[8] = a; matProj[12] = 0;
	matProj[1] = 0; matProj[5] = f; matProj[9] = b; matProj[13] = 0;
	matProj[2] = 0; matProj[6] = 0; matProj[10] = c; matProj[14] = d;
	matProj[3] = 0; matProj[7] = 0; matProj[11] = 1; matProj[15] = 0;
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2014105011 컴퓨터학부 김동욱");
	setViewMat();
	setProjMat();
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutIdleFunc(myidle);
	glewInit();
	myinit();
	
	glutMainLoop();
	return 0;
}
