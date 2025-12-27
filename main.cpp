#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


int ombro = 0, cotovelo = 0;

GLuint idsTextura[3];
float rotacaoX = 0.0;
float rotacaoY = 0.0;
float rotacaoZ = 0.0;

float camx = 0.0;
float camy = 2.0;
float camz = 5.0;

float anguloesfera = 0.0;
float velocidade = 1.0;
float anguloRotacao = 0.0;

float anguloLuzX = 0.0;
float anguloLuzY = 0.0;
float anguloLuzZ = 0.0;

float posSol[] = {5.0f, 10.0f, -5.0f, 1.0f}; // Posição do Sol

void configuraIluminacao()
{
 GLfloat luzAmbiente[] = {0.1, 0.1, 0.1, 1.0};   // luz suave
 GLfloat luzDifusa[] = {1.0, 1.0, 1.0, 1.0};     // luz branca
 GLfloat luzPosicao[] = {0.0, 0.0, 5.0, 1.0};    // luz pontual

 glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
 glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
}

void configuraMaterialVermelho()
{
 GLfloat materialDifuso[] = {1.0, 0.5, 0.0, 1.0};    // laranja
 GLfloat materialAmbiente[] = {0.2, 0.0, 0.0, 1.0};  // vermelho escuro
 GLfloat materialEspecular[] = {0.6, 0.6, 0.6, 1.0}; 

 glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
 glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
 glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

 glMaterialf(GL_FRONT, GL_SHININESS, 10); // brilho medio
}

void configuraMaterialVerde()
{
 GLfloat materialDifuso[] = {0.0, 1.0, 0.0, 1.0};    // verde
 GLfloat materialAmbiente[] = {0.0, 0.2, 0.0, 1.0};  // verde escuro
 GLfloat materialEspecular[] = {0.6, 0.6, 0.6, 1.0}; 

 glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
 glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
 glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

 glMaterialf(GL_FRONT, GL_SHININESS, 30); // brilho medio
}

void configuraSombreamento(int sombraTipo)
{
 if (sombraTipo == 0)
  glShadeModel(GL_FLAT);  // sombreamento plano (flat)

 if (sombraTipo == 1)
   glShadeModel(GL_SMOOTH);  // sombreamento suave (Gouraud)

 glutPostRedisplay();  
}

void carregarTextura(const char* nomeArquivo, int indice)
{
 int largura, altura, canais;


 unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);


 if (!dados)
  exit(1);


 // gerar textura
 glGenTextures(1, &idsTextura[indice]);
 glBindTexture(GL_TEXTURE_2D, idsTextura[indice]);


 // configurar parametros da textura
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


 glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB,
               largura, altura, 0, (canais == 4) ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, dados);


 // liberar a memoria da imagem
 stbi_image_free(dados);              
}
void desenhaCubo(float tamanho)
{
 // face frontal
 glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
 glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, tamanho);
  glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, tamanho);
  glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, tamanho, tamanho);
  glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, tamanho, tamanho);
 glEnd();


  // face traseira
  glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, tamanho, -tamanho);
   glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, tamanho, -tamanho);
  glEnd();


  // face superior
  glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, tamanho, -tamanho);
   glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, tamanho, -tamanho);
   glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, tamanho, tamanho);
   glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, tamanho, tamanho);
  glEnd();


  // face inferior
  glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho, tamanho);
   glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho, tamanho);
  glEnd();


  // face direita
  glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, tamanho, -tamanho);
   glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, tamanho, tamanho);
   glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, -tamanho, tamanho);
  glEnd();


  // face esquerda
  glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, -tamanho);
   glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, tamanho, -tamanho);
   glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, tamanho, tamanho);
   glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho, tamanho);
  glEnd();    
}




// Variáveis de posição, rotação e estados hierárquicos
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
float rotX = 0.0f, rotY = 0.0f;
float rotPescoço = 0.0f, rotCabeça = 0.0f, rotBico = 0.0f;

void desenhaEsferaPequena(GLfloat raio)
{
 glutSolidSphere(raio, 20, 20);   
}

// Função para desenhar uma esfera
void desenhaEsfera(float raio, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(raio, 50, 50);
    glPopMatrix();
}


// Função para desenhar um cilindro
void desenhaCilindro(float base, float topo, float altura, float x, float y, float z) {
    GLUquadric *quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(- 80, 1, 0, 0); // Rotaciona para ficar em pé
    gluCylinder(quad, base, topo, altura, 50, 50);
    glPopMatrix();
    gluDeleteQuadric(quad);
}


// Função para desenhar um retângulo achatado
void desenhaRetangulo(float largura, float altura, float x, float y, float z, float angulo) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angulo, 0, 1, 0);
    glScalef(largura, altura, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();
}


// Função para desenhar o pato usando modelo hierárquico
void desenhaPato() {
  glPushMatrix();


    // Corpo
    glColor3f(1.0, 1.0, 1.0); // Branco
    desenhaEsfera(1.0, 0.0, 0.0, 0.0);
       configuraMaterialVermelho();
       configuraSombreamento(0);



    // Pescoço (movimento hierárquico)
      glPushMatrix();
      glTranslatef(0.0, 1.0, 0.0);  // Move para a base do pescoço
      glRotatef((float)ombro, 0, 1, 0); // Rotaciona o pescoço
      desenhaCilindro(0.2, 0.2, 1.0, 0.0, 0.0, 0.0);
             configuraMaterialVermelho();
       configuraSombreamento(0);




    // Cabeça (movimento hierárquico)
      glPushMatrix();
      glTranslatef(0.0, 1.0, 0.0); // Move para a base da cabeça
      glRotatef((float)cotovelo, 0, 1, 0); // Rotaciona a cabeça
      desenhaEsfera(0.5, 0.0, 0.0, 0.0);
             configuraMaterialVermelho();
       configuraSombreamento(0);


    // Olhos
        glColor3f(0.0, 0.0, 0.0); // Preto
        desenhaEsfera(0.1, -0.2, 0.2, 0.4); // Olho esquerdo
        desenhaEsfera(0.1, 0.2, 0.2, 0.4);  // Olho direito


    // Bico (movimento hierárquico)
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.5); // Move para a posição do bico
      glRotatef(rotBico, 1, 0, 0); // Abre/fecha o bico
      glColor3f(1.0, 0.5, 0.0);   // Laranja
      desenhaRetangulo(0.4, 0.1, 0.0, 0.05, 0.0, 0.0);  // Parte de cima
      desenhaRetangulo(0.4, 0.1, 0.0, -0.05, 0.0, 0.0); // Parte de baixo]
      configuraMaterialVermelho();
       configuraSombreamento(0);
    glPopMatrix();


    glPopMatrix(); // Fim da cabeça
    glPopMatrix(); // Fim do pescoço


  glPopMatrix(); // Fim do corpo
}




// Função de exibição
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   



    // Aplica transformações globais
    glTranslatef(posX, posY, posZ);

    glLoadIdentity();
    gluLookAt(0.0, 2.0, 5.0, // Posição da câmera
              camx, 1.0, camy, // Ponto para onde a câmera olha
              0.0, 1.0, 0.0); // Vetor "up"
    


    glPushMatrix();
    // Aplica transformações globais
    glTranslatef(posX, posY, posZ);
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(ombro, 0, 1, 0);
    glRotatef(cotovelo, 1, 0, 0);
    // Desenha o pato
    desenhaPato();
    glPopMatrix();

     //
   
    //outro cubo (cenário )
  glPushMatrix();
   glTranslatef(1.0, -6.0, 0.0);
   glPushMatrix();
     glColor3f(0.7, 0.7, 0.7);
     desenhaCubo(6.0);
   glPopMatrix();  
   glPopMatrix();
  glPopMatrix();




 // esfera azul
  glPushMatrix();
   glRotatef(anguloesfera * velocidade, 0.0, 1.0, 0.0);
   glTranslatef(4.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloRotacao * velocidade, 0.0, 1.0, 0.0);
     glColor3f(1.0, 0.0, 1.0);
     desenhaEsfera(0.3, 0.0, 0.0, 0.0);
   glPopMatrix();  
 glPopMatrix();

 // esfera azul
  glPushMatrix();
   glRotatef(anguloesfera * velocidade, 0.0, 1.0, 0.0);
   glTranslatef(1.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloRotacao * velocidade, 0.0, 1.0, 0.0);
     glColor3f(1.0, 0.0, 1.0);
     desenhaEsfera(0.3, 0.0, 0.0, 0.0);
   glPopMatrix();  
 glPopMatrix();


 // esfera azul
  glPushMatrix();
   glRotatef(anguloesfera * velocidade, 0.0, 1.0, 0.0);
   glTranslatef(-3.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloRotacao * velocidade, 0.0, 1.0, 0.0);
     glColor3f(1.0, 0.0, 1.0);
     desenhaEsfera(0.3, 0.0, 0.0, 0.0);
   glPopMatrix();  
 glPopMatrix();



    glutSwapBuffers();
}


// Controle de teclado
void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case 'q': posY += 0.1f; break; // Move para cima
    case 'e': posY -= 0.1f; break; // Move para baixo
    case 'a': posX -= 0.1f; break; // Move para a esquerda
    case 'd': posX += 0.1f; break; // Move para a direita
    case 'w': posZ -= 0.1f; break; // Move para frente
    case 's': posZ += 0.1f; break; // Move para trás


   
   
    case 'b': rotBico += 5.0f; break;    // Abre o bico
    case 'n': rotBico -= 5.0f; break;
    glutPostRedisplay();
    case 'o':
    ombro = (ombro + 5) % 360;
    glutPostRedisplay();
    break;  
    case 'p':
    ombro = (ombro - 5) % 360;
    glutPostRedisplay();
    break;  
    case 'k':
    cotovelo = (cotovelo + 5) % 360;
    glutPostRedisplay();
    break;  
    case 'l':
    cotovelo = (cotovelo - 5) % 360;
    glutPostRedisplay();
    break;  
    case 27:
    exit(0);
    break;
 }  
   
    glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y) {
    switch (tecla) {
    case GLUT_KEY_LEFT: // Rotaciona a câmera para a esquerda
        camx -= 0.9;
        break;
    case GLUT_KEY_RIGHT: // Rotaciona a câmera para a direita
        camx += 0.9;
        break;
    case GLUT_KEY_UP: // Eleva a câmera
        camz += 0.9;
        break;
    case GLUT_KEY_DOWN: // Abaixa a câmera
        camz -= 0.9;
        break;
    }
    glutPostRedisplay(); // Redesenha a tela
}

void atualiza(int valor)
{

 anguloesfera += 1.5 * velocidade;
 anguloRotacao += 5.0 * velocidade;

 
 glutPostRedisplay();
 glutTimerFunc(16, atualiza, 0);   
}


// Inicialização
void inicializa() {
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo azul claro
    glEnable(GL_TEXTURE_2D);



     carregarTextura("texturas/water.jpg", 0);
     carregarTextura("texturas/water.jpg", 1);
     carregarTextura("texturas/water.jpg", 2);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

                //texturas 

 glEnable(GL_DEPTH_TEST); // ativa teste de profundidade
 glEnable(GL_LIGHTING); // ativa a iluminacao
 glEnable(GL_LIGHT0); // ativa a luz 0
 glEnable(GL_COLOR_MATERIAL);


   
}

void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(65.0, (float)largura / (float)altura, 1.0, 50.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}



// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 800);
    glutCreateWindow("Pato 3D com Modelo Hierárquico");


    glutSpecialFunc(tecladoEspecial);
    inicializa();
    glutTimerFunc(16, atualiza, 0); 
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado); // Controle do teclado
    glutMainLoop();

    return 0;
}

