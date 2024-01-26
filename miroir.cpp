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

void drawCircularSurface()
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    gluDisk(quad, 0.0, 1.5, 32, 1);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(quad);
}

/* void drawCylinder()
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -1.6f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    // Le cylindre est orienté verticalement par défaut.
    // Appliquer la texture manuellement en mappant les coordonnées UV
    gluQuadricTexture(quad, true); // Dites à GLU de générer des coordonnées de texture

    // Dessiner le cylindre
    gluCylinder(quad, 0.5, 0.5, 2.5, 32, 32);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(quad);
} */

void drawCylinder()
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -1.6f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    // Activation du mappage de l'environnement

    glEnable(GL_TEXTURE_GEN_S);
    // glEnable(GL_TEXTURE_GEN_T);

    // Configuration du plan de réflexion (ici, nous utilisons GL_SPHERE_MAP)
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    // glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    gluCylinder(quad, 0.5, 0.5, 2.5, 32, 32);

    glPopMatrix();

    // Désactivation du mappage de l'environnement après le rendu
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

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

    // Chargez la texture avant le rendu du disque et du cylindre
    loadTexture("Img.jpeg");

    // SUR PLAN DISQUE
    drawCircularSurface();

    // loadTexture("ImgInv.jpeg");
    //  SUR CYLINDRE
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
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
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
