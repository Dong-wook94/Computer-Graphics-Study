#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform float udist; \n\
void main(void) { \n\
  gl_Position = aPosition.x + udist; \n\
  gl_Position.yzw = aPosition.yzw; \n\
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

	glGenBuffer(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 + 3 + 4 * sizeof(GLfloat), NULL, , GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), 3 * 4 * sizeof(GLfloat), vertices);

}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
};
GLfloat refvertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
};

GLfloat colors[] = {
	1.0, 0.0, 0.0, 1.0, // red
	0.0, 1.0, 0.0, 1.0, // green
	0.0, 0.0, 1.0, 1.0, // blue
};
float dist = 0;
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

	loc = glGetUniformLocation(prog, "udist");
	glUniform1f(loc, dist);
	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// flush all
	glFlush();
}

void myreshape(int x, int y) {
	printf("reshape\n");
	glViewport(0, 0, x, y);//ÀÌ°Ô ¹¹ÇÏ´Â°ÇÁö ³ìÀ½ ´Ù½Ãµè±â
}
int tt = 0;

void myidle() {

	
	dist += 0.001;
	if (dist > 0.5)
		dist =-0.5;
	printf("dist %f\n", dist );
	/*for (int i = 0; i < 12; i += 4) {
		vertices[i]= refvertices[i]+dist;
		glutPostRedisplay();
	}*/
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutReshapeFunc(myreshape);
	glutIdleFunc(myidle);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
