#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// Raw RGBA data for later use as a texture:
static GLubyte  logo[128][96][4];     

static char* vsSource = "#version 150 \n\
in vec4 aVertex; \n\
in vec2 aTexCoord; \n\
out vec2 vTexCoord; \n\
void main(void) { \n\
	gl_Position = aVertex; \n\
	vTexCoord = aTexCoord; \n\
}";

static char* fsSource = "#version 120 \n\
varying vec2 vTexCoord; \n\
uniform sampler2D texSampler; \n\
void main(void) { \n\
	gl_FragColor = texture2D(texSampler, vTexCoord); \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

char buf[1024];

GLuint texObject = 0;

GLubyte texImage[] = {
	255, 0, 0, 255, // red
	0, 255, 0, 255, // green
	0, 0, 255, 255, // blue
	255, 255, 0, 255, // yellow
};

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
	+0.5, +0.5, 0.0, 1.0,
};

GLfloat texCoord[] = {
	0.0, 1.0,
	1.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
};

void myinit(void) {
	GLuint status;
	GLuint loc;
	int i, j;

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
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aVertex");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	// provide the tex coord attributes
	loc = glGetAttribLocation(prog, "aTexCoord");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, texCoord);
	// select tex unit #0
	glActiveTexture(GL_TEXTURE0);
	// make a tex object
	glGenTextures(1, &texObject);
printf("tex object = %d\n", texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);


  for (i = 0; i < 128; i++ ) {
        for (j = 0; j < 96; j++ ) {
            GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
            logo[i][j][0]  = c;
            logo[i][j][1]  = c;
			logo[i][j][2] = c;
			logo[i][j][3] = c;
            
        }
    }
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 96, 0, GL_RGBA, GL_UNSIGNED_BYTE, logo);
glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// tex binding to tex unit #0
	loc = glGetUniformLocation(prog, "texSampler");
printf("loc = %d\n", loc);
	glUniform1i(loc, 0);
printf("error = %d\n", glGetError());
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

void mydisplay(void) {
	// clear in black color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// draw a triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// flush all
	glFlush();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
