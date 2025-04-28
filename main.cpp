#include <GL/glut.h>
#include <iostream>
#include "transform.h"

using namespace std;

/* camera.cpp
*
*  abril, 2002 - Luciana Nedel
*  adaptado por Carla Freitas
*  adaptado por Bruno Schneider em marco de 2012
*  modificado por Bruno Schneider em outubro de 2016
*
*  Exemplo para manipulacao de camera em OpenGL
*/

#include <cmath>
#include <iostream>
#include <GL/glut.h>

using namespace std;

bool perspectiva = true;

// estrutura que representa uma camera
typedef struct {
        GLdouble atx;
        GLdouble aty;
        GLdouble atz;
        GLdouble tox;
        GLdouble toy;
        GLdouble toz;
} Observador;

Observador camera;

// Parametros de projecao
GLfloat fAspect;
GLfloat angle = 100;    // angulo de abertura para proj. perspectiva

// limites do volume ortografico (projecao paralela)
GLdouble minX=-100.00;
GLdouble maxX=100.00;
GLdouble minY=-100.00;
GLdouble maxY=100.00;
GLint incremento=0;

// planos near e far para recorte em profundidade
GLdouble near_plane= 2.0;
GLdouble far_plane=240.0;

//matriz de transformacao
double tranformMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};


/* Funcao usada para especificar o volume de visualizacao
*  no caso de projecao perspectiva.
*/
void ComputarProjecao() {
    // Especifica manipulacao da matriz de projecao
    glMatrixMode(GL_PROJECTION);
    // Inicializa matriz com a identidade
    glLoadIdentity();

    // Especifica a projecao perspectiva
    gluPerspective(angle,fAspect,near_plane,far_plane);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa matriz modelview com a identidade
    glLoadIdentity();
    // Especifica posicao do observador e do alvo
    gluLookAt (camera.atx,camera.aty,camera.atz, camera.tox,camera.toy,camera.toz, 0,1,0);
}

void InicializarCamera() {
    camera.atx=0.0;
    camera.aty=15.0;
    camera.atz=-100.0;

    camera.tox=0.0;   // olhando para o ponto central da pir�mide
    camera.toy=15.0;
    camera.toz=0.0;
    ComputarProjecao();
}

// Inicializa��o
void SetupRC(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Limpa a janela e habilita o teste para eliminar faces posteriores
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // habilita o uso de uma cor por face
    glShadeModel (GL_FLAT);
    InicializarCamera();
}

// Chamada pela GLUT quando a janela e' redimensionada.
void ChangeSize(GLsizei w, GLsizei h) {
    // Para prevenir uma divis�o por zero
    if ( h == 0 )
        h = 1;

    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // Calcula a corre��o de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;

    ComputarProjecao();
}

// Minha fun��o de desenho
void RenderScene() {
    // Limpa a janela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha uma pir�mide de base quadrada (lado 25) com faces coloridas
    // localizada na origem, com com altura 30
    glBegin(GL_QUADS);
       // face da base - preta
       glColor3f(0.2f, 0.2f, 0.2f);
       glVertex3i(-25,0,-25);
       glVertex3i(25,0,-25);
       glVertex3i(25,0,25);
       glVertex3i(-25,0,25);
    glEnd();

    glBegin(GL_TRIANGLES);
       // face voltada para eixo z positivo
       glColor3f(1.0f, 0.0f, 0.0f);
       glVertex3i(-25,0,25);
       glVertex3i(25,0,25);
       glVertex3i(0,30,0);

       // face voltada para o eixo x positivo
       glColor3f(1.0f, 1.0f, 0.0f);
       glVertex3i(25,0,25);
       glVertex3i(25,0,-25);
       glVertex3i(0,30,0);

       // face voltada para o eixo z negativo
       glColor3f(0.0f,0.0f,1.0f);
       glVertex3i(25,0,-25);
       glVertex3i(-25,0,-25);
       glVertex3i(0,30,0);

       // face voltada para o eixo x negativo
       glColor3f(0.0f,1.0f, 0.0f);
       glVertex3i(-25,0,-25);
       glVertex3i(-25,0,25);
       glVertex3i(0,30,0);
     glEnd();


    //Garante a execu��o dos comandos de desenho
    glFlush();
}

void DefinirTransformacao(const Vector& v, const Point& p, double angle) {
    Transformation trans;
    trans.loadRotationTransform(v, p, angle);
    double* m = trans.intoFlatMatrix();
    for (int i = 0; i < 16; i++) {
        tranformMatrix[i] = m[i];
    }
    delete[] m;
    ComputarProjecao();
}

void Girar() {
    glMatrixMode(GL_MODELVIEW); // Or GL_PROJECTION depending on your use case
    glMultMatrixd(tranformMatrix);
}

void CallbackTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 't': {
            double x, y, z, angle;
            std::cout << "Defina um vetor (3 doubles), uma posição (3 doubles) e um angulo (radianos).\n";
            std::cout << "Vetor: ";
            std::cin >> x >> y >> z;
            Vector v(x, y, z);

            std::cout << "Posicao: ";
            std::cin >> x >> y >> z;
            Point p(x, y, z);

            std::cout << "Angulo: ";
            std::cin >> angle;

            DefinirTransformacao(v, p, angle);
            break;
        }
        case 'g':
            Girar();
            break;
        case 27: // tecla ESCAPE
            exit(0);
        default:
            // Nao redesenhar a tela
            return;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    // Procedimento de inicializacao da GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400,350);
    glutCreateWindow("Controle da Camera");
    glutKeyboardFunc(CallbackTeclado);
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    SetupRC();
    cout << "Comandos do teclado:\n"
         << "t: definir tranformacao\n"
         << "g: girar\n"
         << "ESC: terminar a execucao\n";
    glutMainLoop();
    return 0;
}

