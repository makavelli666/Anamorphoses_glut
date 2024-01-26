#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>


using namespace std ;

void affichage(void);
void clavier(unsigned char touche,int x,int y);
void affiche_repere(void);
void mouse(int, int, int, int);
void mouseMotion(int, int);
//void reshape(int,int);

// variables globales pour OpenGL
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance=0.;
float zoomFactor = 1.0;
float zoomIncrement = 0.1;



enum TypeBouton
{
action1 = 0,
action2,
action3,
action4
}bouton_action = action1;

static void menu (int item)
{
bouton_action = static_cast<TypeBouton> (item);
glutPostRedisplay ();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ATTENTION !  Les instructions tourne en boucle dans les fonctions d'appelle
static void appelaction1()
{
    // Instruction ... Fonction 1
}

static void appelaction2()
{
    // Fonction 2
}

static void appelaction3()
{
    // Fonction 3
}

static void appelaction4()
{
    // Fonction 4
}

//----------------------------------------------------------------------------------
void initOpenGl()
//----------------------------------------------------------------------------------
{
    glClearColor( .5, .5, 0.5, 0.0 );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat l_pos[] = { 3.,3.5,3.0,1.0 };
    glLightfv(GL_LIGHT0,GL_POSITION,l_pos);

    glLightfv(GL_LIGHT0,GL_DIFFUSE,l_pos);
    glLightfv(GL_LIGHT0,GL_SPECULAR,l_pos);

    // glDepthFunc(GL_LESS);
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)200/(GLfloat)200,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// glScalef(.7,.7,.7);
    gluLookAt(0.,0.,10., 0.,0.,0., 0.,1.,0.);
    // glTranslatef(0.0,0.0,-5.0);
}



int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(300,300);
  glutInitWindowSize(800,800);
  glutCreateWindow("ifs");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(1.0);

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);
 //glutDisplayFunc(main_display);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Cone", action1);
  glutAddMenuEntry ("Cube", action2);
  glutAddMenuEntry ("Pyramide", action3);
  glutAddMenuEntry ("Miroir cylindrique", action4);
  // par exemple si on veut faire apparaître le menu avec clic droit

  glutAttachMenu(GLUT_MIDDLE_BUTTON);
  glutPostRedisplay();
  initOpenGl();
/* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}
//------------------------------------------------------
void affiche_repere(void)
{
  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0);
  glVertex2f(0.,0.);
  glVertex2f(1.,0.);
  glEnd();

	 glBegin(GL_LINES);
  glColor3f(0.0,1.0,0.0);
  glVertex2f(0.,0.);
  glVertex2f(0.,1.);
  glEnd();
   glBegin(GL_LINES);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.,0.,0.);
  glVertex3f(0.,0.,1.);
  glEnd();

}

//-----------------------------------------------------



//------------------------------------------------------
void affichage(void)
{
	glMatrixMode(GL_MODELVIEW);
  /* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0,0,cameraDistance);
    glRotatef(cameraAngleX,1.,0.,0.)	;
    glRotatef(cameraAngleY,0.,1.,0.);
    //--------------------------------
    affiche_repere();

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
    }

    glPopMatrix();
    /* on force l'affichage du resultat */
    glFlush();
}

//------------------------------------------------------


//------------------------------------------------------
void clavier(unsigned char touche,int x,int y)
{

  switch (touche)
    {
    case '+':
      glutPostRedisplay();
      break;
    case '-':
      glutPostRedisplay();
      break;
    case 'f': //* affichage en mode fil de fer
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
      case 'p': //* affichage du carre plein
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
  case 's' : //* Affichage en mode sommets seuls
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'q' : //*la touche 'q' permet de quitter le programme
      exit(0);
    }

}
void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
    glutPostRedisplay();
}


void mouseMotion(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}
