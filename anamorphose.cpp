#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <jpeglib.h>
#include <jerror.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifdef __WIN32
#pragma comment(lib, "jpeg.lib")
#endif

const int largim = 512;
const int hautimg = 170;

unsigned char image[largim * hautimg * 3];
char presse;
int anglex = 30, angley = 20, xold, yold;


int n=10;            // Nombre de coté du polygone (base deu triangle)


GLuint textureID;
glm::mat4 model, view, projection;

void affichage();
void clavier(unsigned char touche, int x, int y);
void souris(int boutton, int etat, int x, int y);
void sourismouv(int x, int y);
void redim(int l, int h);
void loadJpegImage(const char* fichier);
void setTexture();
void drawCone();
void drawCube();
void drawPyramid();
void drawPyramid4();
void drawPolygonalBasePyramid(int sides);
enum TypeBouton
{
action1 = 0,
action2,
action3,
action4,
action5,
action6,
action7
}bouton_action = action1;

static void menu (int item)
{
bouton_action = static_cast<TypeBouton> (item);
glutPostRedisplay();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ATTENTION !  Les instructions tourne en boucle dans les fonctions d'appelle
static void appelaction1()
{
   drawCone();
}
static void appelaction2()
{
   drawPyramid();
}
static void appelaction3()
{
   drawPyramid4();
}
static void appelaction4()
{
    drawPolygonalBasePyramid(n);
}
static void appelaction5()
{
   drawCube();
}
static void appelaction6()
{
   drawPolygonalBasePyramid(n+1);

}
static void appelaction7()
{
    if(n>3){
        drawPolygonalBasePyramid(n-1);
    }
    else{
        drawPolygonalBasePyramid(3);
    }
}
int main(int argc, char** argv)
{
    /* Chargement de la texture */
    loadJpegImage("./bat.jpg");

    /* Creation de la fenetre OpenGL */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Texture Cone");

    /* Initialisation de l'etat d'OpenGL */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    /* Mise en place de la projection perspective */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1, 1.0, 20);
    glMatrixMode(GL_MODELVIEW);

    /* Parametrage du placage de textures */
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largim, hautimg, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glEnable(GL_TEXTURE_2D);

    /* Mise en place des fonctions de rappel */
    glutDisplayFunc(affichage);
    glutKeyboardFunc(clavier);
    glutMouseFunc(souris);
    glutMotionFunc(sourismouv);
    glutReshapeFunc(redim);

    glutCreateMenu (menu);
    glutAddMenuEntry ("Cone", action1);
    glutAddMenuEntry ("Pyramide base 3 ", action2);
    glutAddMenuEntry ("Pyramide base 4 ", action3);
    glutAddMenuEntry ("Pyramide base n ", action4);
    glutAddMenuEntry ("Pyramide base n+1 ", action6);
    glutAddMenuEntry ("Pyramide base n-1", action7);
    glutAddMenuEntry ("Cube", action5);

  // par exemple si on veut faire apparaître le menu avec un clic sur milieu de la souris
  glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutMainLoop();
    return 0;
}

void drawCone()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

    const float radius = 0.5f;
    const float height = 1.0f;
    const int segments = 50;

    glPushMatrix();
    glTranslatef(0.0f, -height / 2.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, height / 2.0f, 0.0f);

    for (int i = 0; i <= segments; ++i)
    {
        float theta = (2.0f * 3.1415926f * float(i)) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        float texCoordX = (x / radius + 1.0f) * 0.5f;
        float texCoordY = (y / radius + 1.0f) * 0.5f;

        glTexCoord2f(texCoordX, texCoordY);
        glVertex3f(x, -height / 2.0f, y);
    }

    glEnd();

    // Base du cone avec texture
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i)
    {
        float theta = (2.0f * 3.1415926f * float(i)) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        //float texCoordX = (x / radius + 1.0f) * 0.5f;
        //float texCoordY = (y / radius + 1.0f) * 0.5f;

        //glTexCoord2f(texCoordX, texCoordY);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(x, -height / 2.0f, y);
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glutSwapBuffers();
}

void drawCube()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

glPushMatrix();
    glColor3f(255.0 / 255.0, 202.8 / 255.0, 72.3/255.0);
glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.5); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.5, 0.5); glVertex3f(0.5, 0.5, 0.5);
    glEnd();


  glBegin(GL_POLYGON);
  glTexCoord2f(0.5, 0.5); glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(0.5, 1.0); glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(1.0, 1.0); glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(1.0, 0.5); glVertex3f( 0.5, 0.5,-0.5);
  glEnd();



  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, 0.5,-0.5);
  glTexCoord2f(0.0, 0.5); glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(0.5, 0.5); glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5, 1.0); glVertex3f(-0.5, 0.5,-0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.5, 1.0); glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.5, 0.5); glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(1.0, 0.5); glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glTexCoord2f(0.5, 0.0); glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.5, 0.5); glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, 0.5,-0.5);
  glEnd();


  glBegin(GL_POLYGON);
  glTexCoord2f(0.5, 0.5); glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5, 0.0); glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,-0.5,-0.5);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  glutSwapBuffers();
}

void drawPyramid() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

    const float baseSize = 1.5f;
    const float height = 2.0f;
    const int segments = 3; // Nombre de cotes du polygone de base (triangle)

    glPushMatrix();
    glTranslatef(0.0f, -height / 2.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, height / 2.0f, 0.0f);

    for (int i = 0; i <= segments; ++i) {
        float theta = (2.0f * PI * float(i)) / float(segments);
        float x = baseSize * cosf(theta);
        float y = baseSize * sinf(theta);

        float texCoordX = (x / baseSize + 1.0f) * 0.5f;
        float texCoordY = (y / baseSize + 1.0f) * 0.5f;

        glTexCoord2f(texCoordX, texCoordY);
        glVertex3f(x, -height / 2.0f, y);
    }

    glEnd();

    // Base de la pyramide avec texture
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < segments; ++i)  {
        float theta = (2.0f * PI * float(i)) / float(segments);
        float x = baseSize * cosf(theta);
        float y = baseSize * sinf(theta);

        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(x, -height / 2.0f, y);
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glutSwapBuffers();
}

void drawPyramid4()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 6, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

    const float baseSize = 1.5f;
    const float height = 1.75f;
    const int segments = 4; // Nombre de côtés du polygone de base (carré)

    glPushMatrix();
    glTranslatef(0.0f, -height / 2.0f, 0.0f);
    glTranslatef(0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Dessine les côtés de la pyramide
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, height / 2.0f, 0.0f);

    for (int i = 0; i <= segments; ++i)
    {
        float theta = (2.0f * PI * float(i)) / float(segments);
        float x = baseSize * cosf(theta);
        float y = baseSize * sinf(theta);

        float texCoordX = (cosf(theta) + 1.0f) * 0.5f;
        float texCoordY = (sinf(theta) + 1.0f) * 0.5f;

        glTexCoord2f(texCoordX, texCoordY);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(x, -height / 2.0f, y);
    }
    glEnd();

    // Dessine la base de la pyramide sans texture
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    for (int i = segments; i >= 0; --i)
    {
        float theta = (2.0f * PI * float(i)) / float(segments);
        float x = baseSize * cosf(theta);
        float y = baseSize * sinf(theta);

        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(x, -height / 2.0f, y);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glutSwapBuffers();
}

void drawPolygonalBasePyramid(int sides) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 6, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

    const float baseRadius = 1.5f;
    const float height = 2.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTranslatef(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < sides; ++i) {
        float angle1 = 2.0f * PI * static_cast<float>(i) / static_cast<float>(sides);
        float angle2 = 2.0f * PI * static_cast<float>(i + 1) / static_cast<float>(sides);

        // Triangle adjacent to the base
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -height / 2.0f, 0.0f);

        glTexCoord2f(cos(angle1) * 0.5f + 0.5f, sin(angle1) * 0.5f + 0.5f);
        glVertex3f(baseRadius * cos(angle1), -height / 2.0f, baseRadius * sin(angle1));

        glTexCoord2f(cos(angle2) * 0.5f + 0.5f, sin(angle2) * 0.5f + 0.5f);
        glVertex3f(baseRadius * cos(angle2), -height / 2.0f, baseRadius * sin(angle2));

        // Triangle forming the sides
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, height / 2.0f, 0.0f);

        glTexCoord2f(cos(angle1) * 0.5f + 0.5f, sin(angle1) * 0.5f + 0.5f);
        glVertex3f(baseRadius * cos(angle1), -height / 2.0f, baseRadius * sin(angle1));

        glTexCoord2f(cos(angle2) * 0.5f + 0.5f, sin(angle2) * 0.5f + 0.5f);
        glVertex3f(baseRadius * cos(angle2), -height / 2.0f, baseRadius * sin(angle2));
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void affichage()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0);

    switch (bouton_action)
	{
	case action1:
        appelaction1();
        break;
    case action2:
        appelaction2();
        break;
    case action3:
        appelaction3();
        break;
    case action4:
        appelaction4();
        break;
    case action5:
        appelaction5();
        break;
    case action6:
        appelaction6();
        break;
    case action7:
        appelaction7();
        break;
    }

    glutSwapBuffers();
}

void clavier(unsigned char touche, int x, int y)
{
    switch (touche)
    {
    case 'l':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glutPostRedisplay();
        break;
    case 'n':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glutPostRedisplay();
        break;
    case 27: /* touche ESC */
        exit(0);
    default:
        break;
    }
}

void souris(int bouton, int etat, int x, int y)
{
    if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
    {
        presse = 1;
        xold = x;
        yold = y;
    }
    if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
        presse = 0;
}

void sourismouv(int x, int y)
{
    if (presse)
    {
        anglex = anglex + (x - xold);
        angley = angley + (y - yold);
        glutPostRedisplay();
    }

    xold = x;
    yold = y;
}

void redim(int l, int h)
{
    if (l < h)
        glViewport(0, (h - l) / 2, l, l);
    else
        glViewport((l - h) / 2, 0, h, h);
}

void loadJpegImage(const char* fichier)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* file;
    unsigned char* ligne;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
#ifdef __WIN32
    if (fopen_s(&file, fichier, "rb") != 0)
    {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier texture.jpg\n");
        exit(1);
    }
#elif __GNUC__
    if ((file = fopen(fichier, "rb")) == 0)
    {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier texture.jpg\n");
        exit(1);
    }
#endif
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
    {
        fprintf(stdout, "Erreur : l'image doit etre de type RGB\n");
        exit(1);
    }

    jpeg_start_decompress(&cinfo);
    ligne = image;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        ligne = image + 3 * largim * cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo, &ligne, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}
