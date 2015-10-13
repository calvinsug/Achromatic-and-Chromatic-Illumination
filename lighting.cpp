#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>

enum {
  LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
} LightValues;

#define TORUS 0
#define TEAPOT 1
#define DOD 2
#define TET 3
#define ISO 4
#define QUIT 5

static int spin = 0;
static int obj = TORUS;
static int begin;
char *left_light, *right_light;
GLfloat red_light[] =
{1.0, 0.0, 0.0, 1.0}, green_light[] =
{0.0, 1.0, 0.0, 1.0}, white_light[] =
{1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] =
{-1.0, 0.0, 1.0, 0.0}, right_light_position[] =
{0.0, 0.0, 1.0, 0.0};
GLfloat brass_ambient[] =
{0.33, 0.22, 0.03, 1.0}, brass_diffuse[] =
{0.78, 0.57, 0.11, 1.0}, brass_specular[] =
{0.99, 0.91, 0.81, 1.0}, brass_shininess = 27.8;
GLfloat red_plastic_ambient[] =
{0.0, 0.0, 0.0}, red_plastic_diffuse[] =
{0.5, 0.0, 0.0}, red_plastic_specular[] =
{0.7, 0.6, 0.6}, red_plastic_shininess = 32.0;
GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 76.8;
GLfloat slate_ambient[] =
{0.02, 0.02, 0.02}, slate_diffuse[] =
{0.02, 0.01, 0.01}, slate_specular[] =
{0.4, 0.4, 0.4}, slate_shininess = .78125;

void
output(GLfloat x, GLfloat y, char *format,...)
{
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

void
menu_select(int item)
{
  if (item == QUIT)
    exit(0);
  obj = item;
  glutPostRedisplay();
}

/* ARGSUSED2 */
void
movelight(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    begin = x;
  }
}

/* ARGSUSED1 */
void
motion(int x, int y)
{
  spin = (spin + (x - begin)) % 360;
  begin = x;
  glutPostRedisplay();
}

void
myinit(void)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the 
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void
display(void)
{
  GLfloat position[] =
  {0.0, 0.0, 1.5, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0.0, 0.0, -5.0);

  glPushMatrix();
  glRotated((GLdouble) spin, 0.0, 1.0, 0.0);
  glRotated(0.0, 1.0, 0.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glTranslated(0.0, 0.0, 1.5);
  glDisable(GL_LIGHTING);
  glColor3f(0.0, 1.0, 1.0);
  glutWireCube(0.1);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  switch (obj) {
  case TORUS:
    glutSolidTorus(0.275, 0.85, 20, 20);
    break;
  case TEAPOT:
    glutSolidTeapot(1.0);
    break;
  case DOD:
    glPushMatrix();
    glScalef(.5, .5, .5);
    glutSolidDodecahedron();
    glPopMatrix();
    break;
  case TET:
    glutSolidTetrahedron();
    break;
  case ISO:
    glutSolidIcosahedron();
    break;
  
  }

  glPopMatrix();
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 3000, 0, 3000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  output(80, 2800, "Program Kelompok 10 By:");
  output(80, 2650, "Antonius Surya, Agung Wijyakusuma");
   output(80, 2450, "Calvin Sugianto , Christian , Rama");
   output(80, 2250, "Click kanan mouse untuk memilih menu.");
  output(80, 400, "tahan mouse kiri");
  output(80, 250, "dan gerakan lampu secara horizontal");
  output(80, 100, "untuk mengatur lokasi pencahayaan.");
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
  glutSwapBuffers();
}

void
myReshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

void
tmotion(int x, int y)
{
  printf("Tablet motion x = %d, y = %d\n", x, y);
}

void
tbutton(int b, int s, int x, int y)
{
  printf("b = %d, s = %d, x = %d, y = %d\n", b, s, x, y);
}

void
smotion(int x, int y, int z)
{
  fprintf(stderr, "Spaceball motion %d %d %d\n", x, y, z);
}

void
rmotion(int x, int y, int z)
{
  fprintf(stderr, "Spaceball rotate %d %d %d\n", x, y, z);
}

void
sbutton(int button, int state)
{
  fprintf(stderr, "Spaceball button %d is %s\n",
    button, state == GLUT_UP ? "up" : "down");
}

void
dials(int dial, int value)
{
  fprintf(stderr, "Dial %d is %d\n", dial, value);
  spin = value % 360;
  glutPostRedisplay();
}

void
buttons(int button, int state)
{
  fprintf(stderr, "Button %d is %s\n", button,
    state == GLUT_UP ? "up" : "down");
}

void 
light_select(GLenum which, int value, char **label)
{
  glEnable(which);
  switch (value) {
  case LIGHT_OFF:
    *label = "off";
    glDisable(which);
    break;
  case LIGHT_RED:
    *label = "red";
    glLightfv(which, GL_DIFFUSE, red_light);
    break;
  case LIGHT_WHITE:
    *label = "white";
    glLightfv(which, GL_DIFFUSE, white_light);
    break;
  case LIGHT_GREEN:
    *label = "green";
    glLightfv(which, GL_DIFFUSE, green_light);
    break;
  }
  glutPostRedisplay();
}

void 
left_light_select(int value)
{
  light_select(GL_LIGHT0, value, &left_light);
}

void 
right_light_select(int value)
{
  light_select(GL_LIGHT1, value, &right_light);
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int
main(int argc, char **argv)
{
	int left_light_m, right_light_m;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow(argv[0]);
  myinit();
  glutMouseFunc(movelight);
  glutMotionFunc(motion);
  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);

  #define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disabled", LIGHT_OFF); \
    glutAddMenuEntry("Red", LIGHT_RED); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Green", LIGHT_GREEN);
   left_light_m = glutCreateMenu(left_light_select);
  LIGHT_MENU_ENTRIES();
  right_light_m = glutCreateMenu(right_light_select);
  LIGHT_MENU_ENTRIES();
  glutTabletMotionFunc(tmotion);
  glutTabletButtonFunc(tbutton);
  glutSpaceballMotionFunc(smotion);
  glutSpaceballRotateFunc(rmotion);
  glutSpaceballButtonFunc(sbutton);
  glutDialsFunc(dials);
  glutButtonBoxFunc(buttons);
  glutCreateMenu(menu_select);
  glutAddMenuEntry("Donut", TORUS);
  glutAddMenuEntry("Teko", TEAPOT);
  glutAddMenuEntry("Dodecahedron", DOD);
  glutAddMenuEntry("Tetrahedron", TET);
  glutAddMenuEntry("Icosahedron", ISO);
  glutAddSubMenu("cahaya senter", left_light_m);
  glutAddSubMenu("warna objek", right_light_m);	
  glutAddMenuEntry("Exit Program", QUIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
  left_light_select(LIGHT_RED);
  right_light_select(LIGHT_GREEN);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glLineWidth(1.0);
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* degrees field of view */ 50.0,
    /* aspect ratio */ 1.0, /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
  glTranslatef(0.0, 0.0, -1.0);

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}