#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static char* vsSource = "#version 120 \n\
attribute vec4 vertex; \n\
void main(void) { \n\
  gl_Position = vertex; \n\
}";

static char* fsSource = "#version 120 \n\
void main(void) { \n\
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

void myinit(void) {
	// vs: vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs); // compile to get .OBJ
	// fs: fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs); // compile to get .OBJ
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog); // link to get .EXE
	glUseProgram(prog); // execute it !
}

void mykeyboard(unsigned char key, int x, int y) {
	printf("key : %c      key :%d     x:%d   y:%d\n", key, key, x, y);
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

GLfloat vertices[] = {
	-0.9, -0.9, 0.0, 1.0,
	-0.1, -0.9, 0.0, 1.0,
	-0.9, -0.1, 0.0, 1.0,
	+0.7, +0.7, 0.0, 1.0,
	+0.7, -0.5, 0.0, 1.0,
	-0.5, +0.7, 0.0, 1.0,
};

void mydisplay(void) {
	printf("mydisplay\n");
	GLuint loc;
	// clear in black color
	glClearColor(0.1, 0.5, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "vertex");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	
	// flush all
	glFlush();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);//size ¿‚±‚
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
