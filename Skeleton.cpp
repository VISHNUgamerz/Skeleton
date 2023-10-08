#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cmath>
#include<cstdio>
#define SLICES  100
#define STACKS  100

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat cameraDistance = 200.0f;
GLfloat armSwingAngle = 0.0f;
bool isArmSwingingForward = true;
int lastMouseX, lastMouseY;
bool isMouseDragging = false;
float fieldOfVision, aspect;
float rightLegAngleX = 0.0f;
float leftLegAngleX = 0.0f;


GLfloat upperArmAngleX = 0.0f;
GLfloat lowerArmAngleX = 0.0f;
GLfloat legSwingAngle = 0.0f;
bool isLegSwingingForward = true;
GLfloat legSwingSpeed = 1.0f;

const float initialArmSwingSpeed = 1.0f;
const float armSwingSpeed = 1.0f;

GLfloat bodyMoveX = 0.0f;
GLfloat bodyMoveZ = 0.0f;
GLfloat bodyMoveSpeed = 0.05f;
GLfloat upperArmAngleX2 = 0.0f;
GLfloat lowerArmAngleX2 = 1.0f;
GLfloat leftUpperArmAngleX = 0.0f;
GLfloat leftLowerArmAngleX = 0.0f;
GLfloat rightUpperArmAngleX = 0.0f;
GLfloat rightLowerArmAngleX = 0.0f;
bool isWalkingForward = true;

void updateAnimation(int value) {
    if (isLegSwingingForward) {
        legSwingAngle += legSwingSpeed;
        if (legSwingAngle >= 25.0f) {
            isLegSwingingForward = false;
        }
    }
    else {
        legSwingAngle -= legSwingSpeed;
        if (legSwingAngle <= -25.0f) {
            isLegSwingingForward = true;
        }
    }

    if (isWalkingForward) {
        bodyMoveZ -= bodyMoveSpeed;
        if (bodyMoveZ <= 5.0f) {
            isWalkingForward = false;
        }
    }
    else {
        bodyMoveZ += bodyMoveSpeed;
        if (bodyMoveZ >= 90.0f) {
            isWalkingForward = true;
        }
    }

    if (isArmSwingingForward) {
        leftUpperArmAngleX += armSwingSpeed;
        leftLowerArmAngleX -= armSwingSpeed;
    }
    else {
        leftUpperArmAngleX -= armSwingSpeed;
        leftLowerArmAngleX += armSwingSpeed;
    }

    if (isArmSwingingForward) {
        rightUpperArmAngleX -= armSwingSpeed;
        rightLowerArmAngleX += armSwingSpeed;
    }
    else {
        rightUpperArmAngleX += armSwingSpeed;
        rightLowerArmAngleX -= armSwingSpeed;
    }

    if (leftUpperArmAngleX >= 25.0f || leftUpperArmAngleX <= -25.0f) {
        isArmSwingingForward = !isArmSwingingForward;
    }

    if (leftUpperArmAngleX < -25.0f || leftUpperArmAngleX > 25.0f) {
        leftUpperArmAngleX = 0.0f;
        leftLowerArmAngleX = 0.0f;
    }

    if (rightUpperArmAngleX < -25.0f || rightUpperArmAngleX > 25.0f) {
        rightUpperArmAngleX = 0.0f;
        rightLowerArmAngleX = 0.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void drawEyes() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-5.0f, 5.0f, 7.5f);
    glutSolidSphere(1.0f, 20, 20);
    glTranslatef(10.0f, 0.0f, 0.0f);
    glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}

void initializeLighting() {
    GLfloat lightAmbient[4] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat lightDiffuse[4] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat lightSpecular[4] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat materialSpecular[4] = { 0.3, 0.3, 0.3, 1.0 };
    GLint materialShininess = 60;

    glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
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
    GLfloat lightDirection[] = { -1.0f, -1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
    glEnable(GL_DEPTH_TEST);
    fieldOfVision = 60.0;
}

void specialKeys(int key, int x, int y) {
    const float armRotationSpeed = 2.0f;
    switch (key) {
    case GLUT_KEY_LEFT:
        leftLowerArmAngleX += armRotationSpeed;
        break;
    case GLUT_KEY_RIGHT:
        rightLowerArmAngleX -= armRotationSpeed;
        break;
    case GLUT_KEY_DOWN:
        leftLowerArmAngleX -= armRotationSpeed;
        break;
    case GLUT_KEY_UP:
        rightLowerArmAngleX += armRotationSpeed;
        break;
    default:
        break;
    }

    if (leftLowerArmAngleX > 45.0f) {
        leftLowerArmAngleX = 45.0f;
    }
    else if (leftLowerArmAngleX < -45.0f) {
        leftLowerArmAngleX = -45.0f;
    }

    if (rightLowerArmAngleX > 45.0f) {
        rightLowerArmAngleX = 45.0f;
    }
    else if (rightLowerArmAngleX < -45.0f) {
        rightLowerArmAngleX = -45.0f;
    }
    glutPostRedisplay();
}

void drawGround() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, -40.0f, -200.0f);
    glVertex3f(-100.0f, -40.0f, 200.0f);
    glVertex3f(100.0f, -40.0f, 200.0f);
    glVertex3f(100.0f, -40.0f, -200.0f);
    glEnd();
}

void drawHead() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(10.0f, 50, 50);
    drawEyes();
}

void drawTorso() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glScalef(1.0f, 2.0f, 0.5f);
    glutSolidCube(20.0f);
    glPopMatrix();
}

void drawJoint() {
    glColor3f(0.1f, 0.0f, 0.0f);
    glutSolidSphere(4.0f, 20, 20);
}

void drawUpperArm() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glScalef(0.2f, 0.5f, 0.2f);
    glutSolidCube(20.0f);
    glPopMatrix();
}

void drawLowerArm() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glScalef(0.2f, 0.5f, 0.2f);
    glutSolidCube(20.0f);
    glPopMatrix();
}

void drawArm() {
    glPushMatrix();
    glRotatef(upperArmAngleX, 1.0f, 0.0f, 0.0f);
    drawUpperArm();
    glPushMatrix();
    glTranslatef(0.0f, -8.0f, 0.0f);
    drawJoint();
    glPopMatrix();
    glTranslatef(0.0f, -12.0f, 0.0f);
    glRotatef(lowerArmAngleX, 1.0f, 0.0f, 0.0f);
    drawLowerArm();
    glPopMatrix();
}

void drawKneeJoint() {
    glColor3f(0.1f, 0.0f, 0.0f);
    glutSolidSphere(4.0f, 20, 20);
}

void drawLeg() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -8.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidCube(20.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -13.0f, 0.0f);
    drawKneeJoint();
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    glScalef(0.3f, 0.5f, 0.3f);
    glutSolidCube(20.0f);
    glPopMatrix();
}

void drawNeck() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 22.0f, 0.0f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidCube(20.0f);
    glPopMatrix();
}

void drawBody() {
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 32.0f, 0.0f);
    drawHead();
    glPopMatrix();
    drawNeck();
    drawTorso();
    glPushMatrix();
    glTranslatef(-14.0f, 13.0f, 0.0f);
    glRotatef(leftUpperArmAngleX, 1.0f, 0.0f, 0.0f);
    drawArm();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(14.0f, 13.0f, 0.0f);
    glRotatef(rightUpperArmAngleX, 1.0f, 0.0f, 0.0f);
    drawArm();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.529f, 0.808f, 0.922f, 500.0f);
    glLoadIdentity();
    GLfloat camX = cameraDistance * sin(angleY) * cos(angleX);
    GLfloat camY = cameraDistance * sin(angleX);
    GLfloat camZ = cameraDistance * cos(angleY) * cos(angleX);
    gluLookAt(camX, camY, camZ, 20.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    drawGround();
    glPushMatrix();
    glTranslatef(bodyMoveX, 25.0f, bodyMoveZ);
    glRotatef(armSwingAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -25.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-5.0f, -15.0f, 0.0f);
    glRotatef(legSwingAngle, 1.0f, 0.0f, 0.0f);
    drawLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(5.0f, -15.0f, 0.0f);
    glRotatef(-legSwingAngle, 1.0f, 0.0f, 0.0f);
    drawLeg();
    glPopMatrix();
    drawBody();
    glPushMatrix();
    glTranslatef(-10.0f, 15.0f, 0.0f);
    drawJoint();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(10.0f, 15.0f, 0.0f);
    drawJoint();
    glPopMatrix();
    glPopMatrix();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: %s\n", gluErrorString(error));
    }
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void moveLeft(float speed) {
    bodyMoveX -= speed;
}

void moveRight(float speed) {
    bodyMoveX += speed;
}

void moveForward(float speed) {
    bodyMoveZ -= speed;
}

void moveBackward(float speed) {
    bodyMoveZ += speed;
}

void keyPressed(unsigned char key, int x, int y) {
    const float armRotationSpeed = 2.0f;
    const float legRotationSpeed = 2.0f;

    switch (key) {
    case 'p':
        leftLowerArmAngleX += armRotationSpeed;
        break;
    case 'q':
        leftLowerArmAngleX -= armRotationSpeed;
        break;
    case 'b':
        rightLowerArmAngleX += armRotationSpeed;
        break;
    case 'u':
        rightLowerArmAngleX -= armRotationSpeed;
        break;
    case 'j':
        leftLegAngleX += legRotationSpeed;
        break;
    case 'l':
        leftLegAngleX -= legRotationSpeed;
        break;
    case 'k':
        rightLegAngleX += legRotationSpeed;
        break;
    case 'i':
        rightLegAngleX -= legRotationSpeed;
        break;
    case 'A':
    case 'a':
        moveLeft(0.8f);
        break;
    case 'D':
    case 'd':
        moveRight(0.8f);
        break;
    case 'W':
    case 'w':
        moveForward(0.8f);
        break;
    case 'S':
    case 's':
        moveBackward(0.8f);
        break;
    case 'V':
    case 'v':
        leftLowerArmAngleX = 0.0f;
        rightLowerArmAngleX = 0.0f;
        break;
    default:
        break;
    }

    if (leftLowerArmAngleX > 45.0f) {
        leftLowerArmAngleX = 45.0f;
    }
    else if (leftLowerArmAngleX < -45.0f) {
        leftLowerArmAngleX = -45.0f;
    }

    if (rightLowerArmAngleX > 45.0f) {
        rightLowerArmAngleX = 45.0f;
    }
    else if (rightLowerArmAngleX < -45.0f) {
        rightLowerArmAngleX = -45.0f;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMouseDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            isMouseDragging = false;
        }
    }
}

void motion(int x, int y) {
    if (isMouseDragging) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;
        angleY += deltaX * 0.01f;
        angleX += deltaY * 0.01f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Walking Skeleton Animation");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initializeLighting();
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(0, updateAnimation, 0);
    glutMainLoop();
    return 0;
}
