#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture;
GLfloat cameraAngleX = 0.5f;
GLfloat cameraAngleY = 0.0f;
GLfloat cameraDistance = 3.0f;
int lastMouseX, lastMouseY;

void loadTexture(const char *filename)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (nrChannels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else if (nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            std::cout << "Not supported format." << std::endl;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
    }
}

void drawPlane()
{
    float angle = 90.0f;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawCylinder()
{
    float cylinderRadius = 0.5f;
    float cylinderHeight = 2.5f;

    float verticalAngle = 90.0f;
    float rotateAgnle = 120.0f;

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -1.5f);
    glRotatef(verticalAngle, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateAgnle, 0.0f, 0.0f, 1.0f);

    gluCylinder(quad, cylinderRadius, cylinderRadius, cylinderHeight, 32, 32);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(quad);
}

void updateCameraPosition()
{
    glLoadIdentity();
    gluLookAt(
        cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
        cameraDistance * sin(cameraAngleX),
        cameraDistance * cos(cameraAngleY) * cos(cameraAngleX),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    updateCameraPosition();

    drawPlane();
    drawCylinder();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        lastMouseX = x;
        lastMouseY = y;
    }
}

void motion(int x, int y)
{
    cameraAngleY += (x - lastMouseX) * 0.01f;
    cameraAngleX += (y - lastMouseY) * 0.01f;

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
    loadTexture("Img.jpeg");
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Anamorphose avec miroir cylindrique");
    initRendering();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
