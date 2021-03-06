#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WINDSIZEX 500
#define WINDSIZEY 500

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aNormal; \n\
varying vec4 vColor; \n\
uniform mat4 uModel; \n\
uniform mat4 uView; \n\
uniform mat4 uProj; \n\
uniform vec4 light_position; // in world frame \n\
uniform vec4 light_ambient; \n\
uniform vec4 light_diffuse; \n\
uniform vec4 light_att; \n\
uniform vec4 material_ambient; \n\
uniform vec4 material_diffuse; \n\
void main(void) { \n\
	vec3 N = aNormal.xyz; \n\
	vec3 L = normalize(light_position.xyz - (uModel * aPosition).xyz); \n\
	vec4 ambient = light_ambient * material_ambient; \n\
	float d = length(light_position.xyz - (uModel * aPosition).xyz); \n\
	float denom = light_att.x + light_att.y * d + light_att.z * d * d; \n\
	vec4 diffuse = max(dot(L, N), 0.0) * light_diffuse * material_diffuse / denom; \n\
  	vColor = ambient + diffuse; \n\
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

int num = 0;
GLfloat vertex[10240 * 3][4];
GLfloat normal[10240 * 3][4];

void setLightAndMaterial(void) {
	GLfloat light_pos[4] = { 1.5, 1.5, -2.0, 1.0 };
	GLfloat light_amb[4] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_dif[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_att[4] = { 0.7, 0.2, 0.1, 1.0 }; // a, b, c, shineness
	GLfloat mat_amb[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_dif[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLuint loc;
	// light
	loc = glGetUniformLocation(prog, "light_position");
	glUniform4fv(loc, 1, light_pos);
	loc = glGetUniformLocation(prog, "light_ambient");
	glUniform4fv(loc, 1, light_amb);
	loc = glGetUniformLocation(prog, "light_diffuse");
	glUniform4fv(loc, 1, light_dif);
	loc = glGetUniformLocation(prog, "light_att");
	glUniform4fv(loc, 1, light_att);
	// material
	loc = glGetUniformLocation(prog, "material_ambient");
	glUniform4fv(loc, 1, mat_amb);
	loc = glGetUniformLocation(prog, "material_diffuse");
	glUniform4fv(loc, 1, mat_dif);
}

void triangles(int level, GLfloat a[3], GLfloat b[3], GLfloat c[3]) {
	if (num >= sizeof(vertex) / sizeof(vertex[0])) {
		printf("vertices buffer overflow...\n");
		return;
	}
	if (level <= 0) {
		vertex[num][0] = a[0]; vertex[num][1] = a[1]; vertex[num][2] = a[2]; vertex[num][3] = 1;
		normal[num][0] = a[0]; normal[num][1] = a[1]; normal[num][2] = a[2]; normal[num][3] = 1;
		num++;
		vertex[num][0] = b[0]; vertex[num][1] = b[1]; vertex[num][2] = b[2]; vertex[num][3] = 1;
		normal[num][0] = b[0]; normal[num][1] = b[1]; normal[num][2] = b[2]; normal[num][3] = 1;
		num++;
		vertex[num][0] = c[0]; vertex[num][1] = c[1]; vertex[num][2] = c[2]; vertex[num][3] = 1;
		normal[num][0] = c[0]; normal[num][1] = c[1]; normal[num][2] = c[2]; normal[num][3] = 1;
		num++;
	} else {
		GLfloat ab[3];
		GLfloat bc[3];
		GLfloat ca[3];
		GLfloat l;
		// ab = (a + b) /2
		ab[0] = (a[0] + b[0]) / 2.0; ab[1] = (a[1] + b[1]) / 2.0; ab[2] = (a[2] + b[2]) / 2.0;
		l = sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
		ab[0] /= l; ab[1] /= l; ab[2] /= l;
		// bc = (b + c) /2
		bc[0] = (b[0] + c[0]) / 2.0; bc[1] = (b[1] + c[1]) / 2.0; bc[2] = (b[2] + c[2]) / 2.0;
		l = sqrt(bc[0] * bc[0] + bc[1] * bc[1] + bc[2] * bc[2]);
		bc[0] /= l; bc[1] /= l; bc[2] /= l;
		// ca = (c + a) /2
		ca[0] = (c[0] + a[0]) / 2.0; ca[1] = (c[1] + a[1]) / 2.0; ca[2] = (c[2] + a[2]) / 2.0;
		l = sqrt(ca[0] * ca[0] + ca[1] * ca[1] + ca[2] * ca[2]);
		ca[0] /= l; ca[1] /= l; ca[2] /= l;
		// call sub-triangles
		triangles(level - 1, a, ab, ca);
		triangles(level - 1, b, bc, ab);
		triangles(level - 1, c, ca, bc);
		triangles(level - 1, ab, bc, ca);
	}
}

void generateSphere(int level) {
	static GLfloat v[4][3] = {
		0.0, 0.0, 1.0,
		0.943, 0.0, -0.333,
		-0.471, 0.816, -0.333,
		-0.471, -0.816, -0.333,
	};
	num = 0;
	triangles(level, v[0], v[1], v[2]);
	triangles(level, v[0], v[2], v[3]);
	triangles(level, v[0], v[3], v[1]);
	triangles(level, v[3], v[2], v[1]);
	printf("totally %d vertices\n", num);
}

GLuint vbo[1];

char buf[1024];

GLfloat motion_start[3]; // start vector, (x, y, z)
GLfloat motion_end[3]; // end vector, (x, y, z)

GLfloat matModel[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
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

void matCopy(GLfloat a[16], GLfloat b[16]) { // a = b;
	a[0] = b[0]; a[4] = b[4]; a[8] = b[8]; a[12] = b[12];
	a[1] = b[1]; a[5] = b[5]; a[9] = b[9]; a[13] = b[13];
	a[2] = b[2]; a[6] = b[6]; a[10] = b[10]; a[14] = b[14];
	a[3] = b[3]; a[7] = b[7]; a[11] = b[11]; a[15] = b[15];
}

void matIden(GLfloat a[16]) { // a = I
	a[0] = 1.0; a[4] = 0.0; a[8] = 0.0; a[12] = 0.0;
	a[1] = 0.0; a[5] = 1.0; a[9] = 0.0; a[13] = 0.0;
	a[2] = 0.0; a[6] = 0.0; a[10] = 1.0; a[14] = 0.0;
	a[3] = 0.0; a[7] = 0.0; a[11] = 0.0; a[15] = 1.0;
}

void matMult(GLfloat c[16], GLfloat a[16], GLfloat b[16]) { // C = A * B
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

void vecMult(GLfloat v[4], GLfloat m[16], GLfloat u[4]) { // v = M * u
	v[0] = m[0] * u[0] + m[4] * u[1] + m[8] * u[2] + m[12] * u[3];
	v[1] = m[1] * u[0] + m[5] * u[1] + m[9] * u[2] + m[13] * u[3];
	v[2] = m[2] * u[0] + m[6] * u[1] + m[10] * u[2] + m[14] * u[3];
	v[3] = m[3] * u[0] + m[7] * u[1] + m[11] * u[2] + m[15] * u[3];
}

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
	glBufferData(GL_ARRAY_BUFFER, 2 * num * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * 4 * sizeof(GLfloat), vertex);
	glBufferSubData(GL_ARRAY_BUFFER, num * 4 * sizeof(GLfloat), num * 4 * sizeof(GLfloat), normal);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aNormal");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(num * 4 * sizeof(GLfloat)));
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

void myidle(void) {
	// nothing
}

void myreshape(int width, int height) {
	int shift;
	if (width >= height) {
		shift = (width - height) / 2;
		glViewport(shift, 0, height, height);
	} else {
		shift = (height - width) / 2;
		glViewport(0, shift, width, width);
	}
}

void mydisplay(void) {
	GLuint loc;
	// clear 
	glClearColor(0.0, 0.0, 0.0, 1.0); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// provide the matrices
	loc = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matModel);
	loc = glGetUniformLocation(prog, "uView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matView);
	loc = glGetUniformLocation(prog, "uProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matProj);
	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, num);
	// flush all
	glFlush();
	glutSwapBuffers();
}

void setViewMat(void) {
	GLfloat eye[3], at[3], up[3];
	GLfloat p[3], n[3], v[3], u[3];
	GLfloat l;
	// set defaults
	eye[0] = 0; eye[1] = 1; eye[2] = -2;
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
	n[0] /= l; n[1] /= l; n[2] /= l;
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
	// view matrix
	matA[0] = u[0]; matA[4] = u[1]; matA[8] = u[2]; matA[12] = 0;
	matA[1] = v[0]; matA[5] = v[1]; matA[9] = v[2]; matA[13] = 0;
	matA[2] = n[0]; matA[6] = n[1]; matA[10] = n[2]; matA[14] = 0;
	matA[3] = 0.0; matA[7] = 0.0; matA[11] = 0.0; matA[15] = 1.0;
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
	glutInitWindowSize(WINDSIZEX, WINDSIZEY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutReshapeFunc(myreshape);
	glutIdleFunc(myidle);
	glewInit();
	generateSphere(4);
	myinit();
	setLightAndMaterial();
	setViewMat();
	setProjMat();
	glutMainLoop();
	return 0;
}
