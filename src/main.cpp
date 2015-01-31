

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "Physics.h"
#include <stdio.h>
#include <string.h>


//displayfunktionen ritar saker till skärmen.
//här kommer våra koordinater ritas ut och antagligen kalla på
//funktionerna som ger nya koordinater att måla hår på!

/**Variabeldeklaration*/
MassSpringSystem* mss0 = new MassSpringSystem(2.0f, -2.0f, 2.5f);
MassSpringSystem* mss1 = new MassSpringSystem(2.0f, -1.0f, 3.0f);
MassSpringSystem* mss2 = new MassSpringSystem(2.0f,  0.0f, 3.5f);
MassSpringSystem* mss3 = new MassSpringSystem(2.0f,  1.0f, 4.0f);
MassSpringSystem* mss4 = new MassSpringSystem(2.0f,  2.0f, 4.5f);

float slowMotionRatio = 10.0f, timeElapsed = 0;
float moveZ = 55.0f,  moveX = 0.0f,  moveY = 0.0f;
float time2 = 0.0;
int counter = 0;
int lastTime, milliseconds = 0;
/******************************/


/** funktionsdeklaration */
void drawSimulation(MassSpringSystem* mss);
void drawBackground();
void printTimeToScreen();
static void key(unsigned char key, int x, int y);
static void functionKeys(int key, int x, int y);
/*************************/


/** Här kommer den viktigaste funktionen **/
static void display(void)
{
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	gluLookAt(moveX, moveY, moveZ, 0, 0, 0, 0, 1, 0);
    // Clear Screen And Depth Buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Rita bakgrunden
    drawBackground();

    drawSimulation(mss0);
    drawSimulation(mss1);
    drawSimulation(mss2);
    drawSimulation(mss3);
    drawSimulation(mss4);

    //skriv ut gången tid till skärmen
    printTimeToScreen();

    glFlush();
}


static void update(){

    int timeNow;
    timeNow = glutGet(GLUT_ELAPSED_TIME);
    int milliseconds = timeNow - lastTime;
    lastTime = timeNow;

    float dt = milliseconds / 1000.0f;  // konvertera till millisekunder
	float maxPossible_dt = 0.001f;		// sätt max tidssteg till 0.06 för stabilitet
										// överstiger dt vissa värden flippar allt ur...
    dt /= slowMotionRatio;

  	int numOfIterations = (int)(dt / maxPossible_dt) + 1;		// räkna ut hur många gånger vi måste iterera
	if (numOfIterations != 0)									// undvik noll-division
		dt = dt / numOfIterations;								// dt måste skalas inför uträkningarna

	for (int a = 0; a < numOfIterations; ++a)					// iterera simulationen numOfIterations gånger
	{
	    counter +=1;
		mss0->operate(dt);					// Itererar simulationen med dt
		mss1->operate(dt);
		mss2->operate(dt);
		mss3->operate(dt);
		mss4->operate(dt);
	}

    glutPostRedisplay();
}

/** Program entry point **/
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);


    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("GLUT & openGL");
    glutDisplayFunc(display);

    counter = 0;
    glClearColor(0.0, 0.0, 0.0, 0.5);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	glViewport (0, 0, 640, 480);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0f, 640/480,	1.0f, 100.0f);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

    //keyboard strokes
    glutKeyboardFunc(key);
    //handles up, down, F1-F12 osv
    glutSpecialFunc(functionKeys);
    //ser till att det händer något hela tiden!
    glutIdleFunc(update);

    glutMainLoop();

    return EXIT_SUCCESS;
}



/** Här kommer lite funktioner **/

void drawSimulation(MassSpringSystem* mss){
    MassSpringSystem* massSpringSystem = mss;

    int a;
	for (a = 0; a < massSpringSystem->numOfMasses; ++a)
	{
		/** Hämta massan **/
		Mass* mass = massSpringSystem->getMass(a);
		Vector3D* pos = &mass->pos;

		/** Draw A Quad Between The Masses To Represent The Spring **/
		glColor3ub(0, 255, 0); // rita i grönt
		glBegin(GL_QUADS);
            //skapar ny vector att lagra postion i. pos = massan vi behandlar
            Vector3D* pos2;

            //om a>0 sätts pos2 till förra massan, annars fästpunkten
            if (a>0) {
                Mass* mass2 = massSpringSystem->getMass(a-1);
                pos2 = &mass2->pos;
            }
            else{
                pos2 = &massSpringSystem->connectionPos;
            }
            //skapa vektor mellan de 2 punkterna, motsvarande springVector
            Vector3D vec = *pos - *pos2;

            Vector3D* z = new Vector3D(0.0f, 0.0f, 1.0f);
            Vector3D n = vec.cross(*z);
            n =  n*0.04;

            //rita ut quads mellan punkter som erhålls
            //genom att addera resp. subtrahera normalen
			glVertex3f(pos->x-n.x, pos->y-n.y, pos->z);
			glVertex3f(pos->x+n.x, pos->y+n.y, pos->z);

			glVertex3f(pos2->x+n.x, pos2->y+n.y, pos2->z);
			glVertex3f(pos2->x-n.x, pos2->y-n.y, pos2->z);

		glEnd();

		/** rita en punkt där massan befinner sig **/
		glColor3ub(255, 0, 0); // rita med annan färg
		glPointSize(200/moveZ);
		glBegin(GL_POINTS);
			glVertex3f(pos->x, pos->y, pos->z);
		glEnd();

		/** Rita ut en rektangel vid fästpunkten*/
		glColor3ub(150, 100, 150); // rita med annan färg
		glBegin(GL_QUADS);
			glVertex3f(-2.2f, 16.2f, 0);
			glVertex3f(2.2f,  16.2f, 0);
			glVertex3f(2.2f,  15.0f, 0);
			glVertex3f(-2.2f, 15.0f, 0);
		glEnd();

		glColor3ub(0, 0, 0); // rita med annan färg
		glBegin(GL_LINE_LOOP);
			glVertex3f(-2.2f, 16.2f, 0);
			glVertex3f(2.2f,  16.2f, 0);
			glVertex3f(2.2f,  15.0f, 0);
			glVertex3f(-2.2f, 15.0f, 0);
		glEnd();

	}
}

void printTimeToScreen()
{
    glColor3ub(250, 150, 50);
    int time;
    time = glutGet(GLUT_ELAPSED_TIME);
    time2 = time/1000.0;
    char buffer[15];
    sprintf(buffer,"TIME:%4.5f",time2);

    glRasterPos2d(-20,-22);
    for (char* c=buffer; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    char info1[50];
    sprintf(info1,"control camera with arrow keys, add ctrl for height");
    glRasterPos3d(-20,21, 0);
    for (char* c=info1; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    char info2[50];
    sprintf(info2,"Reset by pressing spacebar");
    glRasterPos3d(5,-22, 0);
    for (char* c=info2; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    char info3[50];
    sprintf(info3,"Calculations: %i", counter);
    glRasterPos3d(-9,-22, 0);
    for (char* c=info3; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

/** Rita bakgrunden */
void drawBackground(){
    glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
        // Draw The Vertical Lines
        for (float x = -20; x <= 20; x += 1.0f)		// x += 1.0f Stands For 1 Meter Of Space
        {
            glVertex3f(x, 20, 0);
            glVertex3f(x,-20, 0);
        }

        // Draw The Horizontal Lines
        for (float y = -20; y <= 20; y += 1.0f)	    // y += 1.0f Stands For 1 Meter Of Space
        {
            glVertex3f( 20, y, 0);
            glVertex3f(-20, y, 0);
        }
	glEnd();
}

/* Tar hand om mus/tangentbords-callbacks och skickar även med musens position */
static void key(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case ' ':{
            mss0->initSystem();
            mss1->initSystem();
            mss2->initSystem();
            mss3->initSystem();
            mss4->initSystem();

            break;
        }

    }

    glutPostRedisplay();
}

static void functionKeys(int key, int x, int y)
{

    switch (key)
    {
        case GLUT_KEY_UP:{
            int mod = glutGetModifiers();
            if (mod == GLUT_ACTIVE_SHIFT || mod == GLUT_ACTIVE_CTRL)
                moveZ -= 0.5f;
            else
                moveY += 0.5f;
            break;
        }

        case GLUT_KEY_DOWN:{
            int mod = glutGetModifiers();
            if (mod == GLUT_ACTIVE_SHIFT || mod == GLUT_ACTIVE_CTRL)
                moveZ += 0.5f;
            else
                moveY -= 0.5f;
            break;
        }

        case GLUT_KEY_LEFT:{
            moveX -= 0.5f;
            break;
        }

        case GLUT_KEY_RIGHT:{
            moveX += 0.5f;
            break;
        }

        case GLUT_KEY_F1:{
            slowMotionRatio = 1.0f;
            break;
        }

        case GLUT_KEY_F2:{
            slowMotionRatio = 10.0f;
            break;
        }

    }

    glutPostRedisplay();
}
