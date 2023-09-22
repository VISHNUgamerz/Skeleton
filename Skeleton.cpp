#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cctype>

#define SLICES  100
#define STACKS  100

#define PI      3.141592

#define WINDOW_WIDTH    700
#define WINDOW_HEIGHT   500

#define CAMERA_NEAR     1
#define CAMERA_FAR      1000

using namespace std;
bool yogaPose = false; // Flag to indicate whether to perform yoga pose
float yogaPoseAngle = 0.0f; // Angle for yoga pose

typedef struct {
    float x, y, z;
} Coordinate;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////// PAR�METROS DE ILUMINA��O E DA C�MERA  ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////

float fieldOfVision, aspect;

float cameraX, cameraY, cameraZ;


float lightX, lightY, lightZ;


float cameraRadius = 600.0f;


float theta = 0.0f;


float alpha = 0.0f;

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////  PAR�METROS DE CONTROLE DO PERSONAGEM  //////////////////////
/////////////////////////////////////////////////////////////////////////////////////


float neckJointXAxis = 0.0;
float neckJointYAxis = 0.0;
float neckJointZAxis = 0.0;


float leftShoulderJointXAxis = 0.0;
float leftShoulderJointZAxis = 0.0;

float leftElbowJointZAxis = 0.0;


float rightShoulderJointXAxis = 0.0;
float rightShoulderJointZAxis = 0.0;

float rightElbowJointZAxis = 0.0;


float leftHipJointXAxis = 0.0;
float leftHipJointZAxis = 0.0;

float leftKneeJointXAxis = 0.0;


float rightHipJointXAxis = 0.0;
float rightHipJointZAxis = 0.0;

float rightKneeJointXAxis = 0.0;

float* xRotation, * zRotation, * yRotation;

int previousX;
int previousY;

bool firstClick = true;


void drawTruncatedCone(float centerX, float centerY, float centerZ, float upperRadius, float lowerRadius, float length) {

    const int NUM_POINTS = 100;

    float pass = 2 * PI / NUM_POINTS;

    vector<Coordinate> firstCircle;
    vector<Coordinate> secondCircle;

    float dx, dy, dz;

    Coordinate coord;

    for (float angle = 0.0; angle <= 2 * PI; angle += pass) {

        dx = cos(angle) * upperRadius;
        dy = length / 2.0;
        dz = sin(angle) * upperRadius;

        coord.x = centerX + dx;
        coord.y = centerY + dy;
        coord.z = centerZ + dz;

        firstCircle.push_back(coord);

        dx = cos(angle) * lowerRadius;
        dy = -length / 2.0;
        dz = sin(angle) * lowerRadius;

        coord.x = centerX + dx;
        coord.y = centerY + dy;
        coord.z = centerZ + dz;

        secondCircle.push_back(coord);
    }

    int size;

    glBegin(GL_POLYGON);

    size = firstCircle.size();


    for (int i = 0; i < size; i++) {
        glVertex3f(firstCircle[i].x, firstCircle[i].y, firstCircle[i].z);
    }

    glEnd();

    glBegin(GL_POLYGON);

    size = secondCircle.size();


    for (int i = 0; i < size; i++) {
        glVertex3f(secondCircle[i].x, secondCircle[i].y, secondCircle[i].z);
    }

    glEnd();


    glBegin(GL_QUAD_STRIP);

    size = firstCircle.size();

    for (int i = 0; i < size; i++) {

        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(firstCircle[i].x, firstCircle[i].y, firstCircle[i].z);

        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(secondCircle[i].x, secondCircle[i].y, secondCircle[i].z);
    }

    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(firstCircle[0].x, firstCircle[0].y, firstCircle[0].z);

    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(secondCircle[0].x, secondCircle[0].y, secondCircle[0].z);

    glEnd();

}
void performYogaPose() {
    if (yogaPose) {
        // Set joint angles for the yoga pose
        neckJointXAxis = 40.0f;
        rightShoulderJointXAxis = -180.0f;
        
        leftShoulderJointXAxis = -180.0f;
         rightHipJointXAxis = 60.0f;
         leftHipJointXAxis = 0.0;
     
        // ... Set other joint angles as needed

        // Increment the angle for animationp
        yogaPoseAngle += 1.0f;

        // If the animation is complete, reset the flag and angles
        if (yogaPoseAngle >= 90.0f) {
            yogaPose = false;
            yogaPoseAngle = 0.0f;
        }
    }
}


void update() {
    performYogaPose();
    glutPostRedisplay();
}

void drawCylinder(float centerX, float centerY, float centerZ, float radius, float length) {

    drawTruncatedCone(centerX, centerY, centerZ, radius, radius, length);
}


void drawArm(float upperRadius, float lowerRadius, float length) {

    glutSolidSphere(upperRadius * 0.8, SLICES, STACKS);
    drawTruncatedCone(0.0, -length / 2.0, 0.0, upperRadius, lowerRadius, length);

}


void drawThorax(float shouldersWidth, float thoraxWidth, float thoraxHeight) {

    glPushMatrix();
    glScalef(1.0, 1.0, 0.5);
    drawTruncatedCone(0.0, thoraxHeight / 2.0, 0.0, shouldersWidth / 2.0, thoraxWidth / 2.0, thoraxHeight);
    glPopMatrix();
}


void drawWaist(float thoraxWidth, float waistWidth, float waistHeight) {

    glPushMatrix();
    glScalef(1.0, 1.0, 0.7);
    glutSolidSphere(thoraxWidth * 0.5, SLICES, STACKS);
    drawTruncatedCone(0, -waistHeight * 0.3, 0, thoraxWidth / 2.0, waistWidth / 2.0, waistHeight);
    glPopMatrix();
}


void drawLeg(float upperRadius, float lowerRadius, float length) {


    glutSolidSphere(upperRadius * 0.8, SLICES, STACKS);


    drawTruncatedCone(0, -length / 2.0, 0, upperRadius, lowerRadius, length);

}


void drawHead(float headRadius) {

    float neckLength = headRadius * 0.6;
    float neckRadius = headRadius * 0.5;

    float eyeRadius = 3.0;


    glutSolidSphere(neckRadius * 0.8, SLICES, STACKS);


    drawCylinder(0.0, neckLength / 2.0, 0.0, neckRadius, neckLength);

    glPushMatrix();

    glTranslatef(0.0, headRadius + neckLength, 0.0);


    glPushMatrix();

    glScalef(1.05, 1.2, 1.0);
    glutSolidSphere(headRadius, SLICES, STACKS);
    glPopMatrix();


    glColor3f(0.0, 0.0, 0.0);


    glPushMatrix();
    glTranslatef(-headRadius * 0.5, 0.0, headRadius - eyeRadius);
    glutSolidSphere(eyeRadius, SLICES, STACKS);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(headRadius * 0.5, 0.0, headRadius - eyeRadius);
    glutSolidSphere(eyeRadius, SLICES, STACKS);
    glPopMatrix();

    glPopMatrix();
}

void updateLightPosition() {


    float proportion = 1.0;

    lightX = cameraX * proportion;
    lightY = cameraY * proportion;
    lightZ = cameraZ * proportion;

    GLfloat lightPosition[4] = { lightX, lightY, lightZ, 1.0 };


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}


void updateCameraPosition() {

    float upX, upY, upZ;


    glMatrixMode(GL_PROJECTION);


    glLoadIdentity();


    gluPerspective(fieldOfVision, aspect, CAMERA_NEAR, CAMERA_FAR);


    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();


    cameraX = sin(alpha) * cos(theta) * cameraRadius;

    cameraY = sin(theta) * cameraRadius;

    cameraZ = cos(alpha) * cos(theta) * cameraRadius;

    upX = 0.0;
    upY = 1.0;
    upZ = 0.0;


    if (theta >= PI / 2.0 && theta < 3.0 * PI / 2.0)
        upY = -1.0;
    else
        upY = 1.0;


    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, upX, upY, upZ);

    updateLightPosition();
}


void draw() {

    float headRadius = 22.0;

    float upperArmUpperRadius = 15.0;
    float upperArmLowerRadius = 10.0;
    float upperArmLength = 70.0;

    float lowerArmUpperRadius = 13.0;
    float lowerArmLowerRadius = 9.0;
    float lowerArmLength = 60.0;

    float upperLegUpperRadius = 20.0;
    float upperLegLowerRadius = 16.0;
    float upperLegLength = 60.0;

    float lowerLegUpperRadius = 17.0;
    float lowerLegLowerRadius = 13.0;
    float lowerLegLength = 80.0;

    float shouldersWidth = 85.0;
    float thoraxWidth = 70.0;
    float thoraxHeight = 70.0;

    float waistWidth = thoraxWidth * 0.7;
    float waistHeight = thoraxHeight * 0.9;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCameraPosition();


    glColor3f(1.0, 0.0, 0.0);


    drawThorax(shouldersWidth, thoraxWidth, thoraxHeight);

    glPushMatrix();

    glTranslatef(0.0, thoraxWidth, 0.0);


    glColor3f(0.0, 0.0, 1.0);


    glPushMatrix();


    glRotatef(neckJointXAxis, 1.0, 0.0, 0.0);
    glRotatef(neckJointYAxis, 0.0, 1.0, 0.0);
    glRotatef(neckJointZAxis, 0.0, 0.0, 1.0);

    drawHead(headRadius);

    glPopMatrix();


    glColor3f(0.0f, 1.0, 0.0);


    glPushMatrix();

    glTranslatef(-shouldersWidth * 0.5, -upperArmUpperRadius, 0.0);


    glRotatef(rightShoulderJointXAxis, 1.0, 0.0, 0.0);
    glRotatef(rightShoulderJointZAxis, 0.0, 0.0, 1.0);

    drawArm(upperArmUpperRadius, upperArmLowerRadius, upperArmLength);

    glTranslatef(0.0, -(upperArmLength + lowerArmUpperRadius * 0.5), 0.0);


    glRotatef(rightElbowJointZAxis, 0.0, 0.0, 1.0);

    drawArm(lowerArmUpperRadius, lowerArmLowerRadius, lowerArmLength);

    glPopMatrix();


    glPushMatrix();

    glTranslatef(shouldersWidth * 0.5, -upperArmUpperRadius, 0.0);


    glRotatef(leftShoulderJointXAxis, 1.0, 0.0, 0.0);
    glRotatef(leftShoulderJointZAxis, 0.0, 0.0, 1.0);

    drawArm(upperArmUpperRadius, upperArmLowerRadius, upperArmLength);

    glTranslatef(0.0, -(upperArmLength + lowerArmUpperRadius * 0.5), 0.0);


    glRotatef(leftElbowJointZAxis, 0.0, 0.0, 1.0);

    drawArm(lowerArmUpperRadius, lowerArmLowerRadius, lowerArmLength);

    glPopMatrix();

    glPopMatrix();


    glColor3f(1.0f, 0.0, 1.0);


    drawWaist(waistWidth, thoraxWidth, waistHeight);


    glColor3f(0.3f, 0.5, 0.5);


    glPushMatrix();

    glTranslatef(-waistWidth * 0.5, -waistHeight * 0.9, 0.0);


    glRotatef(rightHipJointXAxis, 1.0, 0.0, 0.0);
    glRotatef(rightHipJointZAxis, 0.0, 0.0, 1.0);

    drawLeg(upperLegUpperRadius, upperLegLowerRadius, upperLegLength);

    glTranslatef(0.0, -(upperLegLength + lowerLegUpperRadius * 0.5), 0.0);


    glRotatef(rightKneeJointXAxis, 1.0, 0.0, 0.0);

    drawLeg(lowerLegUpperRadius, lowerLegLowerRadius, lowerLegLength);

    glPopMatrix();


    glPushMatrix();

    glTranslatef(waistWidth * 0.5, -waistHeight * 0.9, 0.0);


    glRotatef(leftHipJointXAxis, 1.0, 0.0, 0.0);
    glRotatef(leftHipJointZAxis, 0.0, 0.0, 1.0);

    drawLeg(upperLegUpperRadius, upperLegLowerRadius, upperLegLength);

    glTranslatef(0.0, -(upperLegLength + lowerLegUpperRadius * 0.5), 0.0);


    glRotatef(leftKneeJointXAxis, 1.0, 0.0, 0.0);

    drawLeg(lowerLegUpperRadius, lowerLegLowerRadius, lowerLegLength);

    glPopMatrix();

    glutSwapBuffers();
}


void initializeLighting() {

    GLfloat lightAmbient[4] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat lightDiffuse[4] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat lightSpecular[4] = { 0.4, 0.4, 0.4, 1.0 };


    GLfloat materialSpecular[4] = { 0.3, 0.3, 0.3, 1.0 };

    GLint materialShininess = 60;


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);


    glShadeModel(GL_SMOOTH);


    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);


    glMateriali(GL_FRONT, GL_SHININESS, materialShininess);


    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);


    glEnable(GL_COLOR_MATERIAL);


    glEnable(GL_LIGHTING);


    glEnable(GL_LIGHT0);


    glEnable(GL_DEPTH_TEST);


    fieldOfVision = 45.0;
}


void resizeWindow(GLsizei w, GLsizei h) {


    if (h == 0) h = 1;


    glViewport(0, 0, w, h);


    aspect = (GLfloat)w / (GLfloat)h;

    updateCameraPosition();
}


void specialKeys(int key, int x, int y) {

    switch (key) {

    case GLUT_KEY_LEFT:
        *zRotation -= 3.0f;
        break;

    case GLUT_KEY_RIGHT:
        *zRotation += 3.0f;
        break;

    case GLUT_KEY_UP:
        *xRotation -= 3.0f;
        break;

    case GLUT_KEY_DOWN:
        *xRotation += 3.0f;
        break;

    case GLUT_KEY_PAGE_UP:
        *yRotation -= 3.0f;
        break;

    case GLUT_KEY_PAGE_DOWN:
        *yRotation += 3.0f;
        break;
    }

    glutPostRedisplay();
}


void keyPressed(unsigned char key, int x, int y) {

    key = tolower(key);

    if (xRotation != NULL && yRotation != NULL && zRotation != NULL) {

        switch (key) {

        case 'r':
            theta = 0.0;
            alpha = 0.0;
            break;

        case 'a':
            xRotation = &rightShoulderJointXAxis;
            yRotation = &rightShoulderJointZAxis;
            zRotation = &rightShoulderJointZAxis;
            break;

        case 'z':
            xRotation = zRotation = yRotation = &rightElbowJointZAxis;
            break;

        case 's':
            xRotation = &leftShoulderJointXAxis;
            yRotation = &leftShoulderJointZAxis;
            zRotation = &leftShoulderJointZAxis;
            break;

        case 'x':
            xRotation = yRotation = zRotation = &leftElbowJointZAxis;
            break;

        case 'd':
            xRotation = &rightHipJointXAxis;
            yRotation = &rightHipJointZAxis;
            zRotation = &rightHipJointZAxis;
            break;

        case 'c':
            xRotation = yRotation = zRotation = &rightKneeJointXAxis;
            break;

        case 'f':
            xRotation = &leftHipJointXAxis;
            yRotation = &leftHipJointZAxis;
            zRotation = &leftHipJointZAxis;
            break;

        case 'v':
            xRotation = yRotation = zRotation = &leftKneeJointXAxis;
            break;

        case 'w':
            xRotation = &neckJointXAxis;
            yRotation = &neckJointYAxis;
            zRotation = &neckJointZAxis;
            break;

        case '+':
            cameraRadius -= 10.0f;
            break;

        case '-':
            cameraRadius += 10.0f;
            break;

        case 'p':
            // Toggle yoga pose
            yogaPose = !yogaPose;
            break;




        }

        glutPostRedisplay();
    }
}


void mousePressed(int x, int y) {

    if (firstClick) {

        previousX = x;
        previousY = y;

        firstClick = false;

    }
    else {


        if (x > previousX)
            alpha -= 0.05;
        else if (x < previousX)
            alpha += 0.05;

        if (y > previousY)
            theta += 0.05;
        else if (y < previousY)
            theta -= 0.05;


        if (theta > 2 * PI)
            theta = theta - 2 * PI;
        else if (theta < 0.0)
            theta = 2 * PI - theta;

        if (alpha > 2 * PI)
            alpha = alpha - 2 * PI;
        else if (alpha < 0.0)
            alpha = 2 * PI - alpha;

        firstClick = true;

        glutPostRedisplay();
    }
}



int main(int argc, char* argv[]) {

    glutInit(&argc, argv);


    xRotation = &rightShoulderJointXAxis;
    yRotation = &rightShoulderJointZAxis;
    zRotation = &rightShoulderJointZAxis;


    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);


    int x = (screenWidth - WINDOW_WIDTH) / 2;
    int y = (screenHeight - WINDOW_HEIGHT) / 2;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(x, y);
    glutCreateWindow("Corpo Humando 3D");

    glutDisplayFunc(draw);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyPressed);
    glutMotionFunc(mousePressed);

    glutReshapeFunc(resizeWindow);

    initializeLighting();
    glutIdleFunc(update);
   

    glutMainLoop();

    return EXIT_SUCCESS;
}