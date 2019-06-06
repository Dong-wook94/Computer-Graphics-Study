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
  if (aPosition.z < -0.2) \n\
	gl_Position.x = aPosition.x + udist; \n\
  else \n\
    gl_Position.x = aPosition.x; \n\
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
GLuint vbo[1];

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
	-0.9, -0.9, -0.5, 1.0,
	+0.1, -0.1, -0.5, 1.0,
	-0.9, +0.1, -0.5, 1.0,
};



GLfloat colors[] = {
	1.0, 0.0, 0.0, 0.5, // red
	1.0, 0.0, 0.0, 0.5, // red
	1.0, 0.0, 0.0, 0.5, // red
	0.0, 0.0, 1.0, 0.5, // blue
	0.0, 0.0, 1.0, 0.5, // blue
	0.0, 0.0, 1.0, 0.5, // blue
};

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


	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), 6 * 4 * sizeof(GLfloat), 
		colors);
	GLuint loc;

	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(6 * 4 * sizeof(GLfloat)));
	
	glLineWidth(10);
	glPointSize(20);
	   
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}


int tt = 0;
float dist = 0;
void mydisplay(void) {
	
	// clear 
	glClearColor(1.0, 1.0, 1.0, 1.0); // gray
	glClear(GL_COLOR_BUFFER_BIT);
	GLuint loc;

	loc = glGetUniformLocation(prog, "udist");
	glUniform1f(loc, dist);

	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glutSwapBuffers();
	// flush all
	glFlush();
}
void myreshape(int x, int y)
{
	glViewport(0, 0, x, y);
 }


void myidle()
{
	dist += 0.00;
	if (dist > 1.0)
		dist = 0;
//	printf("dist  %f\n", dist);

/*	for (int i = 0; i < 12; i += 4)
		vertices[i] = ref_vertices[i] + dist;
		*/
	glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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
