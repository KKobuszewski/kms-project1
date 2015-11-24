//extern "C" {
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//}

#include "molecdyn.h"


class punkt{
public:
  double x,y;
  punkt()
  {
    x = 0;
    y = 0;
  }
};
punkt A;
double fio=90, fi=fio, omega=0, l=6 ,g=9.82, krok=0.004, r=1;

// wskaźnik naciśnięcia lewego przycisku myszki

GLint PARTICLES;

int button_state = GLUT_UP;

// położenie kursora myszki

int button_x, button_y;


// rozmiary bryły obcinania

const GLdouble left = - 20.0;
const GLdouble right = 20.0;
const GLdouble bottom = - 20.0;
const GLdouble top = 20.0;
const GLdouble near = 40.0;
const GLdouble far = 80.0;

// współczynnik skalowania

GLfloat scale = 1.0;

// kąty obrotu

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

void GenerateDisplayList()
{
    PARTICLES = glGenLists(1);
    glNewList(PARTICLES,GL_COMPILE);
    glutSolidSphere(0.07,20,20);
    glEndList();
    
    // TODO: Druga lista na wire sphere
}


// funkcja generująca scenę 3D

void Display()
{
    glEnable(GL_DEPTH); 
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    
    glLoadIdentity();
    
    // =============== Initial settings =================================================
    
    // przesunięcie układu współrzędnych obiektu do środka bryły odcinania
    glTranslatef( 0, 0, -( near + far ) / 2 );
    
    // skalowanie obiektu - klawisze "+" i "-"
    glScalef( scale, scale, scale );
    
    // obroty obiektu - klawisze kursora
    glRotatef( rotatex, 1.0, 0, 0 );
    glRotatef( rotatey, 0, 1.0, 0 );
    
    
    // =============== Load particles =====================================================
    
    //glLoadIdentity();
        gluLookAt(0., 0., 0., 0, 0, 0, 0.0, 1.0, 0.0);
        glPushMatrix();
        glRotatef(0,0,1,0);
        glTranslatef(1., 1., 1.);
        glColor3f(0.0f, 0.0f, 1.0f);
        //glutSolidSphere(0.07f,20,20);
        glCallList(PARTICLES);
        glPopMatrix();
    
        glPushMatrix();
        glTranslatef(-1.,- 1., -1.);
        glColor3f(0.0f, 0.0f, 1.0f);
        glutSolidSphere(0.07f,20,20);
        glCallList(PARTICLES);
        glPopMatrix();
        
//     for (uint32_t ii = 0; ii < global.N; ii++)
//     {
//         //glLoadIdentity();
//         glPushMatrix();
//         gluLookAt(0., 0., 0., 0, 0, 0, 0.0, 1.0, 0.0);
//         glRotatef(0,0,1,0);
//         glTranslatef(global.x_arr[ii], global.y_arr[ii], global.z_arr[ii]);
//         glColor3f(0.0f, 0.0f, 1.0f);
//         //glutSolidSphere(0.07f,20,20);
//         glCallList(PARTICLES);
//         glPopMatrix();
//     }
    
    // =============== Set bonduary frame =================================================
    
    //glPushMatrix();
    //glColor3f( 0.0, 0.0, 0.0 );
    //glLineWidth(3);
    //glutWireCube( 12.0 );
    //glPopMatrix();
    /*
    glPushMatrix();
    //glLoadIdentity();
    //gluLookAt(0., 0., 0., 0, 0, 0, 0.0, 1.0, 0.0);
    //glRotatef(0.,0,1,0);
    //glTranslatef(0, 0, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glutWireSphere(global.L,10,10);
    glPopMatrix();
    */
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    
    // ========================  =========================================
    
    glFlush();
    glutSwapBuffers();
}

// zmiana wielkości okna
void Reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
        if( width < height && width > 0 )
             glFrustum( left, right, bottom * height / width, top * height / width, near, far );
        else
       
        if( width >= height && height > 0 )
             glFrustum( left * width / height, right * width / height, bottom, top, near, far );
    
    Display();
}

void Keyboard( unsigned char key, int x, int y )
{
    // klawisz +
    if( key == '+' )
         scale += 0.2;
    else
   
    // klawisz -
    if( key == '-' && scale > 0.1 )
         scale -= 0.2;
   
    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}


void MouseButton( int button, int state, int xm, int ym )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // zapamiętanie stanu lewego przycisku myszki
        button_state = state;
       
        // zapamiętanie położenia kursora myszki
        if( state == GLUT_DOWN )
        {
            button_x = xm;
            button_y = ym;
        }
    }
}

// obsługa ruchu kursora myszki

void MouseMotion( int xm, int ym )
{
    if( button_state == GLUT_DOWN )
    {
        rotatey += 0.25 * ( xm - button_x );
        button_x = xm;
        rotatex += 0.25 * ( ym - button_y );
        button_y = ym;
        glutPostRedisplay();
    }
}


void init_gl( int argc, char * argv[] )
{
    glutInit( & argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Animacja dynamiki molekularnej" ); 
    
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );

    glutKeyboardFunc( Keyboard );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    
    GenerateDisplayList();
    //glutMainLoop();
}