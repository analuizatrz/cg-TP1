#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SOIL.h>
#include "inimigo.h"

enum Tela {MENU,JOGO,PAUSA,REINICIA,SAIR,INSTRUCOES,VITORIA,GAMEOVER,CREDITOS};
enum Tela telaAtual;

struct ponto posicaoMira;
struct tamanho tamanhoMira;
struct ponto posicaoMouse;

struct  botao{
    struct ponto posicao;
    struct tamanho dimensao;
};

typedef struct botao Botao;

Botao novoJogo, instrucoes, creditos;

Inimigo *listaInimigos;

int tempo, periodo, atingidos, fugitivos, nivel, pontuacao, vidas;

GLuint texturaFundo, texturaPato, texturaGato, texturaMira;

float patoPosicao, patoLargura, gatoPosicao, gatoLargura;


GLuint botaoNovoJogo, botaoInstrucoes, botaoCreditos;

//escreve no mundo
void escrever(void *fonte, char *palavra, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);
    for (i = 0; i < strlen (palavra); i++)
        glutBitmapCharacter(fonte, palavra[i]);
}
//coloca o plano de fundo do jogo
void desenhaCenario(void){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaFundo);
    glBegin(GL_QUADS);
        // Associamos um canto da textura para cada vértice
        glTexCoord2f(0, 0); glVertex3f(0, 500, 0);
        glTexCoord2f(1, 0); glVertex3f(500, 500, 0);
        glTexCoord2f(1, 1); glVertex3f(500, 0, 0);
        glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void desenhaGato(void){
}
void desenhaPato(Inimigo *inimigo){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPato);
    glBegin(GL_QUADS);
        glTexCoord2f(patoPosicao, 0);               glVertex3f(inimigo->posicao.x, inimigo->posicao.y + inimigo->dimensao.altura, 0);
        glTexCoord2f(patoPosicao + patoLargura, 0); glVertex3f(inimigo->posicao.x + inimigo->dimensao.largura, inimigo->posicao.y + inimigo->dimensao.altura, 0);
        glTexCoord2f(patoPosicao + patoLargura, 1); glVertex3f(inimigo->posicao.x + inimigo->dimensao.largura, inimigo->posicao.y, 0);
        glTexCoord2f(patoPosicao, 1);               glVertex3f(inimigo->posicao.x, inimigo->posicao.y, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
void desenhaMira(void) {
    glutSetCursor(GLUT_CURSOR_NONE);
    glPushMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texturaMira);
    glTranslated(posicaoMira.x,posicaoMira.y,0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-tamanhoMira.largura,  tamanhoMira.altura, 0);
        glTexCoord2f(0, 0); glVertex3f(-tamanhoMira.largura, -tamanhoMira.altura, 0);
        glTexCoord2f(1, 0); glVertex3f( tamanhoMira.largura, -tamanhoMira.altura, 0);
        glTexCoord2f(1, 1); glVertex3f( tamanhoMira.largura,  tamanhoMira.altura, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
}
void desenhaInimigos(Inimigo *listaInimigos){
    if(listaInimigos != NULL){
        Inimigo *aux = listaInimigos;
        while(aux != NULL){
            desenhaPato(aux);
            aux = aux->proximo;
        }
    }
}
// botoes que compoem o menu
void desenhaBotao(Botao botao, GLuint textura){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTranslated(botao.posicao.x, botao.posicao.y,0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-botao.dimensao.largura, -botao.dimensao.altura, 0);
        glTexCoord2f(0, 0); glVertex3f(-botao.dimensao.largura,  botao.dimensao.altura, 0);
        glTexCoord2f(1, 0); glVertex3f( botao.dimensao.largura,  botao.dimensao.altura, 0);
        glTexCoord2f(1, 1); glVertex3f( botao.dimensao.largura, -botao.dimensao.altura, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
//desenha a tela menu
void desenhaMenu(void){
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    desenhaBotao(novoJogo, botaoNovoJogo);
    desenhaBotao(creditos, botaoCreditos);
    desenhaBotao(instrucoes, botaoInstrucoes);
}
void mensagem(GLuint tela){
    char texto[20];
    switch(telaAtual){
        case JOGO:
            sprintf(texto,"Pontuação : %d",(pontuacao));
            escrever(GLUT_BITMAP_HELVETICA_18,texto,10,15,0);
            sprintf(texto,"Nivel : %d",nivel);
            escrever(GLUT_BITMAP_HELVETICA_18,texto,210,15,0);
            sprintf(texto,"Vidas : %d",(vidas-fugitivos));
            escrever(GLUT_BITMAP_HELVETICA_18,texto,400,15,0);
            break;
        case REINICIA:
            escrever(GLUT_BITMAP_HELVETICA_18,"JOGO PAUSADO",160,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"tecle <r> para reiniciar,<m> para menu ou <c> para continuar",5,280,0);
            break;
        case PAUSA:
            escrever(GLUT_BITMAP_HELVETICA_18,"JOGO PAUSADO",160,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"tecle <p> para voltar ao jogo",120,280,0);
            break;
        case SAIR:
            escrever(GLUT_BITMAP_HELVETICA_18,"JOGO PAUSADO",160,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"tecle <s> para sair do jogo ou <c> para continuar",50,280,0);
            break;
        case VITORIA:
            escrever(GLUT_BITMAP_HELVETICA_18,"VOCE VENCEU !!!!",170,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"tecle <m> para menu ou <j> para novo jogo",80,280,0);
            break;
        case GAMEOVER:
            escrever(GLUT_BITMAP_HELVETICA_18,"VOCE PERDEU !!!!",160,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"tecle <m> para menu ou <j> para novo jogo",80,280,0);
            break;
        case CREDITOS:
            escrever(GLUT_BITMAP_HELVETICA_18,"CREDITOS",200,250,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Jogo desenvolvido por Ana Luiza Sanches",90,280,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Agredecimentos a Higor Fischer e Igor Chagas",80,310,0);
            break;
        case INSTRUCOES:
          escrever(GLUT_BITMAP_HELVETICA_18,"INSTRUCOES",200,240,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Use o mouse para mover a mira e atingir os patos",50,260,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Os patos que escapam resultam em vidas perdidas",50,285,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Voce possui apenas 10 vidas, fique atento !",80,310,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Se quiser fazer uma pausa <p> ou reiniciar <r>",70,335,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"o jogo fique a vontade !",80,360,0);
            escrever(GLUT_BITMAP_HELVETICA_18,"Aproveite o jogo ",200,385,0);

            break;
        default:
            break;
    }
}
void desenhaCena(void){
    glClear(GL_COLOR_BUFFER_BIT);
    switch(telaAtual){
        case MENU:
            desenhaCenario();
            desenhaMenu();
            break;
        case JOGO:
            desenhaCenario();
            mensagem(JOGO);
            desenhaInimigos(listaInimigos);
            desenhaMira();
            break;
        case PAUSA:
            desenhaCenario();
            mensagem(PAUSA);
            break;
        case REINICIA:
            desenhaCenario();
            mensagem(REINICIA);
            break;
        case SAIR:
            desenhaCenario();
            mensagem(SAIR);
            break;
        case VITORIA:
            desenhaCenario();
            mensagem(VITORIA);
            break;
        case GAMEOVER:
            desenhaCenario();
           	mensagem(GAMEOVER);
            break;
        case CREDITOS:
            desenhaCenario();
            mensagem(CREDITOS);
            break;
        case INSTRUCOES:
            desenhaCenario();
            mensagem(INSTRUCOES);
            break;
        default:
            break;
    }
    glFlush();
}
// importa as texturas
void inicializaTexturas(void){
    texturaPato = SOIL_load_OGL_texture("pato.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (texturaPato == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    texturaFundo = SOIL_load_OGL_texture("fundo.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (texturaFundo == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    texturaMira = SOIL_load_OGL_texture("mira.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (texturaMira == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    botaoNovoJogo = SOIL_load_OGL_texture("BotaoNovoJogo.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (botaoNovoJogo == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    botaoCreditos = SOIL_load_OGL_texture("BotaoCreditos.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (botaoCreditos == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    botaoInstrucoes = SOIL_load_OGL_texture("BotaoInstrucoes.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y );
    if (botaoInstrucoes == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}
// atribui valores as variaveis que definem tamanho e posição dos botoes do menu
void inicializaMenu(void){

    novoJogo.posicao.x = 250;
    novoJogo.posicao.y = 150;
    novoJogo.dimensao.altura = 40;
    novoJogo.dimensao.largura = 160;

    instrucoes.posicao.x = 250;
    instrucoes.posicao.y = 250;
    instrucoes.dimensao.altura = 40;
    instrucoes.dimensao.largura = 160;

    creditos.posicao.x = 250;
    creditos.posicao.y = 350;
    creditos.dimensao.altura = 40;
    creditos.dimensao.largura = 160;
}
// atrinui valores a variaveis do jogo (estado inicial)
void inicializaJogo(void){
    listaInimigos = NULL;
    tempo = 0;
    periodo = 30;
    atingidos = 0;
    fugitivos = 0;
    nivel = 1;
    pontuacao = 0;
    vidas = 10;

    posicaoMira.x = 0;
    posicaoMira.y = 0;

    tamanhoMira.largura = 30;
    tamanhoMira.altura = 30;

     patoPosicao = 0;
     patoLargura = 0.25;
     gatoPosicao = 0;
     gatoLargura = 0.5;
}
// Inicia algumas variáveis de estado
void inicializa(void){
    // cor para limpar a tela
    glClearColor(0.3, 0.3, 0.3, 0);      // branco
    inicializaTexturas();
    inicializaMenu();
    telaAtual = MENU;
}
// Callback de redimensionamento
void redimensiona(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){
    switch(telaAtual){
        case MENU:
        break;
        case JOGO:
            switch(key){
                // Tecla P : pausa o jogo
                case 112:
                    telaAtual = PAUSA;
                    break;
                // Tecla R : reinicia o jogo (com dialogo de confirmação)
                case 114:
                    telaAtual = REINICIA;
                    break;
                //Tecla ESC : sai do jogo (com dialogo de confirmação)
                case 27:
                    telaAtual = SAIR;
                    break;
                }
            break;
        case PAUSA:
            // Tecla P: volta ao jogo
            if(key == 112)
                telaAtual = JOGO;
            break;
        case REINICIA:
            switch(key){
                // Tecla C: continua o jogo
                case 99:
                    telaAtual = JOGO;
                    break;
                // Tecla M: menu
                case 109:
                    telaAtual = MENU;
                    break;
                // Tecla R: reinicia o jogo
                case 114:
                    inicializaJogo();
                    telaAtual = JOGO;
                    break;                
            }
            break;
        case SAIR:
            switch(key){
                // Tecla C: continua o jogo
                case 99:
                    telaAtual = JOGO;
                    break;
                // Tecla S: sai do jogo 
                case 115:
                    exit(0);
                    break;
                // Tecla ESC: sai do jogo
                case 27:
                    exit(0);
                    break;
            } 
            break;
        case INSTRUCOES:
            telaAtual = MENU;
            break;
        case VITORIA:
            switch(key){
                // Tecla M: menu
                case 109:
                    telaAtual = MENU;
                    break;
                // Tecla J: novo jogo
                case 106:
                    inicializaJogo();
                    telaAtual = JOGO;
                    break;
            }
            break;
        case GAMEOVER:
            switch(key){
                // Tecla M: menu
                case 109:
                    telaAtual = MENU;
                    break;
                // Tecla J: novo jogo
                case 106:
                    inicializaJogo();
                    telaAtual = JOGO;
                    break;
            }
            break;
        case CREDITOS:
            telaAtual = MENU;
            break;
        default:
            break;
    }
}
//verifica se o mouse clicou em algum botao do menu
int cliqueBotao(Botao botao, int x, int y){
    if((botao.posicao.x - botao.dimensao.largura) < x && x < (botao.posicao.x + botao.dimensao.largura)
    && (botao.posicao.y - botao.dimensao.altura ) < y && y < (botao.posicao.y + botao.dimensao.altura))
        return 1; // retorna true se o clique foi dentro do botao
    else
        return 0; // senao false
}
// callback de clique do mouse
void cliqueMouse(int button, int state, int x, int y){
    if(state == GLUT_UP){
        switch(telaAtual){
            case MENU:
                if(cliqueBotao(novoJogo,x,y)){
                    inicializaJogo();
                    telaAtual = JOGO;
                }
                if(cliqueBotao(creditos,x,y))
                    telaAtual = CREDITOS;
                if(cliqueBotao(instrucoes,x,y))
                    telaAtual = INSTRUCOES;
                break;
            case JOGO:
                listaInimigos = colisao(x,y,listaInimigos,&atingidos);
                break;
            default:
                break;
        }
    }
}

void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}
void atualizaMira(void){
    // faz a mira andar na direção do ponteiro
    posicaoMira.x += (posicaoMouse.x - posicaoMira.x);
    posicaoMira.y += (posicaoMouse.y - posicaoMira.y);
}
/* cria inimigo: o x é definido aleatoriamente, podendo receber um dos 6 valores:
200,230,260,290,310,340. Dessa forma os patos correm no que corresponde ao chao
do cenario.
A velocidade c aumenta de acordo com o nivel que a pessoa esta
*/
void criaInimigo(){
    srand(time(NULL));
    float x = (float) ((rand()%6)* 30)+ 200;
    float v = (float) (rand()%3+1)*nivel;
    listaInimigos = inserirInimigo(novoInimigo(0,x,v),listaInimigos);    
}
//atualiza sprite do pato, permitindo animação
void atualizaSprite(){
    if(patoPosicao < 0.75)
      patoPosicao += 0.25;
    else
      patoPosicao = 0;
}
//atualiza variaveis do jogo como tempo, periodo e pontuação
void atualizaVariaveis(void){
    if(tempo%periodo == 0){
        criaInimigo();
    }
    atualizaMira();
    listaInimigos = atualizaInimigos(listaInimigos,&fugitivos);
    if(tempo%4 == 0){
        atualizaSprite();
    }
    tempo++;
    pontuacao = atingidos*25;
    if(pontuacao == 100){
        nivel = 2;
        periodo = 25;
    }
    if(pontuacao == 300){
        nivel = 3;
        periodo = 20;
    }
    if(pontuacao == 600){
        nivel = 4;
        periodo = 15;
    }
    if(pontuacao == 900){
        nivel = 5;
        periodo = 10;
    }
    if(pontuacao == 1200){
        nivel = 6;
        periodo = 5;
    }
    if(pontuacao == 1500){
        telaAtual = VITORIA;
    }
    if(vidas-fugitivos <= 0){
        telaAtual = GAMEOVER;
    }
}
void atualiza(int indice) {
    switch(telaAtual){
        case JOGO:
            atualizaVariaveis();
            break;
        default:
            break;
    }
    // Pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay ();
    // Se registra novamente, para que fique sempre sendo chamada (30 FPS)
    glutTimerFunc(33, atualiza, 0);
}
// Rotina principal
int main(int argc, char **argv){
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Tiro ao Pato");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    // Registra a função "movimentoMouse" para executar sempre que o mouse mexer
    glutPassiveMotionFunc(movimentoMouse);
    // glutMouseFunc(cliqueMouse);
    glutMouseFunc(cliqueMouse);
    // Registra a função "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, atualiza, 0);
    glutMainLoop();
    return 0;
}