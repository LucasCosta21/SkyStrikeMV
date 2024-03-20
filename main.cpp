/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Grupo Rosa
 * Versão: Final
 * Created on 28 de Outubro de 2018, 23:46
 */

#define VIDA 5
#define PI 3.14159265
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <iostream>

using namespace std;

float modulo(float numero) {
    if (numero < 0) {
        numero *= -1;
    }
    return numero;
}

float hipotenusa(float catA, float catB) {
    return sqrt(pow(modulo(catA), 2) + pow(modulo(catB), 2));
}

class ObjetoTela {
public:
    //Image
    SDL_Texture* textura;
    //Dimensions and position
    SDL_Rect posicao;

    //carrega imagem

    bool CarregaTextura(SDL_Renderer * gRenderer, char * path) {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        } else {
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if (newTexture == NULL) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            } else {
                //Get image dimensions

                posicao.w = loadedSurface->w;
                posicao.h = loadedSurface->h;

            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        //Return success
        textura = newTexture;
        return textura != NULL;
    }

};

class Aviao : public ObjetoTela {
private:
    float profundor = 0; //angulo de subida ou descida
    float motor = 0; //potencia na aeronave (1-5)
    int vida = 100;
    float velX = 0, velY = 0; //velocidades nos eixos x e y
    float velocidade = 0.3; //velocidade real do vetor

public:
    //get e set motor (int & void)

    Aviao() {
        posicao.x = 0;
        posicao.y = 0;
    }

    float getmotor() {
        return motor;
    }

    void setmotor(float x) {
        motor = x;
    }

    //get e set profundor (int & void)

    float getangulo() {
        return profundor;
    }

    void setangulo(float y) {
        profundor = y;
    }

    //get e set VelX (int & void)

    float getvelX() {
        return velX;
    }

    void setvelX(float x) {
        velX = x;
    }

    //get e set VelY (int & void)

    float getvelY() {
        return velY;
    }

    void setvelY(float y) {
        velY = y;
    }

    //get e set Velocidade (int & void)

    float getvel() {
        return velocidade;
    }

    void setvel(float y) {
        velocidade = y;
    }

    void trataVoo(ObjetoTela alvo) {

        int y, x, auxX, auxY;

        if (alvo.posicao.x > posicao.x) {
            auxX = 1;
        } else {
            auxX = -1;
        }
        if (alvo.posicao.y > posicao.y) {
            auxY = 1;
        } else {
            auxY = -1;
        }

        if (modulo(alvo.posicao.x - posicao.x) > 30
                && modulo(alvo.posicao.y - posicao.y) > 30) {
            velX = (alvo.posicao.x - posicao.x) / 20;
            velY = (alvo.posicao.y - posicao.y) / 20;
        } else if (modulo(alvo.posicao.y - posicao.y) <= 30 && modulo(alvo.posicao.x - posicao.x) <= 30) {
            if (velY <= 4) {
                velY += 0.4;
            }
        }

        if (velX < 0) {
            x = -1;
        } else {
            x = 1;
        }
        if (velY < 0) {
            y = -1;
        } else {
            y = 1;
        }

        if (posicao.y >= 540) {
            if (profundor > 300 && profundor < 360) {
                cout << "pousa";
            }
        }
    }
};

class Tiro : public ObjetoTela {
private:
    float velX;
    float velY;
    float angulo;
    bool projetil; //checa se ele é um projetil ou uma bala estatica   
    float auxX, auxY, auxA, auxPosy, auxPosx, teste; //auxiliares
public:

    Tiro() {
        projetil = false;
    }

    void atira(int x, int y, int a, int posx, int posy) {

        if (!projetil) {
            posicao.x = -30;
            posicao.y = -30;
            velX = 0;
            velY = 0;
            angulo = a;
            auxX = x;
            auxY = y;
            auxA = a;
            auxPosy = posy;
            auxPosx = posx;
            teste = true;
        } else if (projetil) {
            if (teste) {
                posicao.x = auxPosx;
                posicao.y = auxPosy;
                teste = false;
            }
            velX = auxX;
            velY = auxY;
            angulo = auxA;
            if (posicao.x > 1180 || posicao.x < -31 || posicao.y > 700 || posicao.y < -31) {
                projetil = false;
            }
        }
    }

    void setProjetil(bool x) {
        projetil = x;
    }

    bool getProjetil() {
        return projetil;
    }

    float getAngulo() {
        return angulo;
    }

    //get e set VelX (int & void)

    float getvelX() {
        return velX;
    }

    void setvelX(float x) {
        velX = x;
    }

    //get e set VelY (int & void)

    float getvelY() {
        return velY;
    }

    void setvelY(float y) {
        velY = y;
    }
};

class TelaJogo {
private:
    ObjetoTela mira;
    Aviao player;
    Aviao inimigo, inimigoGhost;
    Tiro tiroPlayer;
    Tiro tiroInimigo;
    ObjetoTela raycast; //sistema auxiliar na IA inimiga
    ObjetoTela explosao; //sprites explosao
    ObjetoTela bgFase; //background fase
    ObjetoTela bgMenu; //background Menu
    ObjetoTela bgRegras; //background Regras
    ObjetoTela barraPlayer; //barra de vida
    //sons do jogo
    Mix_Music *fase1; //musica que toca durante o jogo
    Mix_Music *fase2; //musica que toca durante o jogo
    Mix_Chunk *efeito; //efeito especial das colisões
    Mix_Music *musicaMenu; //musica que toca durante o Menu

    //Dimensões da tela
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 650;

    //Janela Renderizada
    SDL_Window* gWindow = NULL;

    //Renderer
    SDL_Renderer* gRenderer = NULL;

    void inputMenu(bool * quit, bool * play, bool * regras) {
        SDL_Event e;

        while (SDL_PollEvent(&e) != 0) {
            //User requests quit

            if (e.type == SDL_QUIT) {
                *quit = true;
            } else if (!*regras && e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        *play = false;
                        break;
                    case SDLK_r:
                        *regras = true;
                        break;
                    case SDLK_q:
                        *quit = true;
                        break;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                mira.posicao.x = e.button.x - 10;
                mira.posicao.y = e.button.y - 10;
                if (mira.posicao.y >= 550) {
                    mira.posicao.y = 550;
                }
            } else if (*regras) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_f:
                            *regras = false;
                            break;
                    }
                }
            }
        }
    }

    void playerInput(bool * quit, bool * atira) {
        //Event handler
        SDL_Event e;
        //Handle events on queue   
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit

            if (e.type == SDL_QUIT) {
                *quit = true;
            } else if (e.type == SDL_MOUSEMOTION) {
                mira.posicao.x = e.button.x - 10;
                mira.posicao.y = e.button.y - 10;
                if (mira.posicao.y >= 550) {
                    mira.posicao.y = 550;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {

            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        *atira = true;
                        break;
                }
            }
        }

        if (mira.posicao.x > player.posicao.x) {
            if (mira.posicao.y < player.posicao.y) {
                player.setangulo(360 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI);
                //cout << 360 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
            if (mira.posicao.y > player.posicao.y) {
                player.setangulo(atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI);
                //cout << atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
        }
        if (mira.posicao.x < player.posicao.x) {
            if (mira.posicao.y < player.posicao.y) {
                player.setangulo(180 + atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI);
                //cout << atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
            if (mira.posicao.y > player.posicao.y) {
                player.setangulo(180 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI);
                //cout << 180 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
        }

        //controlador IA
        if (player.posicao.x > inimigo.posicao.x) {
            if (player.posicao.y < inimigo.posicao.y) {
                inimigo.setangulo(360 - atan((modulo(inimigo.posicao.y - player.posicao.y)) / modulo((inimigo.posicao.x - player.posicao.x))) * 180 / PI);
                //cout << 360 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
            if (player.posicao.y > inimigo.posicao.y) {
                inimigo.setangulo(atan((modulo(inimigo.posicao.y - player.posicao.y)) / modulo((inimigo.posicao.x - player.posicao.x))) * 180 / PI);
                //cout << atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
        }
        if (player.posicao.x < inimigo.posicao.x) {
            if (player.posicao.y < inimigo.posicao.y) {
                inimigo.setangulo(180 + atan((modulo(inimigo.posicao.y - player.posicao.y)) / modulo((inimigo.posicao.x - player.posicao.x))) * 180 / PI);
                //cout << atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
            if (player.posicao.y > inimigo.posicao.y) {
                inimigo.setangulo(180 - atan((modulo(inimigo.posicao.y - player.posicao.y)) / modulo((inimigo.posicao.x - player.posicao.x))) * 180 / PI);
                //cout << 180 - atan((modulo(player.posicao.y - mira.posicao.y)) / modulo((player.posicao.x - mira.posicao.x))) * 180 / PI << " ";
            }
        }
    }

    void atira(bool * atira) {
        //cout << bala.getProjetil();
        if (*atira && !tiroPlayer.getProjetil()) {
            tiroPlayer.setProjetil(true);
            *atira = false;
        }
    }

    void inimigoAtira(bool * atira) {
        if (*atira && !tiroInimigo.getProjetil()) {
            tiroInimigo.setProjetil(true);
            *atira = false;
        }
    }

    void montaTela() {
        int i;

        player.posicao.x = 400;
        player.posicao.y = 400;

        inimigo.posicao.y = 100;
        inimigo.posicao.x = 100;

        barraPlayer.posicao.x = 390;
        barraPlayer.posicao.y = 20;

        inimigoGhost.posicao.y = 200;
        inimigoGhost.posicao.x = 200;

        bgFase.posicao.y = 25;
        bgFase.posicao.x = 0;

        explosao.posicao.x = -100;
        explosao.posicao.y = -100;

        bgMenu.posicao.y = 25;
        bgMenu.posicao.x = 0;

        bgRegras.posicao.y = 25;
        bgRegras.posicao.x = 0;

        tiroPlayer.posicao.x = 30;
        tiroPlayer.posicao.y = 30;
    }

    void desenha(bool quit, bool regras, int i, int fase) {
        string root;
        SDL_Point leftTop;

        //pivot de rotacao do raycast
        leftTop.x = 0;
        leftTop.y = 0;

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        // Clear the entire screen to our selected color.
        SDL_RenderClear(gRenderer);
        SDL_RenderCopyEx(gRenderer, bgFase.textura, NULL, &(bgFase.posicao), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, mira.textura, NULL, &(mira.posicao), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, player.textura, NULL, &(player.posicao), player.getangulo(), NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, inimigo.textura, NULL, &(inimigo.posicao), inimigo.getangulo(), NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, barraPlayer.textura, NULL, &(barraPlayer.posicao), 0, NULL, SDL_FLIP_NONE);
        //for (i = 0; i < 15; i++) {
        SDL_RenderCopyEx(gRenderer, tiroPlayer.textura, NULL, &(tiroPlayer.posicao), tiroPlayer.getAngulo(), NULL, SDL_FLIP_NONE);
        //}
        SDL_RenderCopyEx(gRenderer, tiroInimigo.textura, NULL, &(tiroInimigo.posicao), tiroInimigo.getAngulo(), NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, explosao.textura, NULL, &(explosao.posicao), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(gRenderer, raycast.textura, NULL, &(raycast.posicao), inimigo.getangulo(), &leftTop, SDL_FLIP_NONE);

        //atualiza sprites explosão
        root = "textures/explosion/sprite_";
        root += to_string(i);
        root += ".png";
        char *y = new char[root.length() + 1];
        strcpy(y, root.c_str());
        explosao.CarregaTextura(gRenderer, y);

        if (quit) {
            SDL_RenderCopyEx(gRenderer, bgMenu.textura, NULL, &(bgMenu.posicao), 0, NULL, SDL_FLIP_NONE);
            if (regras) {
                SDL_RenderCopyEx(gRenderer, bgRegras.textura, NULL, &(bgRegras.posicao), 0, NULL, SDL_FLIP_NONE);
            }
        }
        //Update screen
        SDL_RenderPresent(gRenderer);
    }

    void trataFisica(bool pouso, bool crash) {

        // distancia += velocidade
        player.posicao.y += player.getvelY();
        player.posicao.x += player.getvelX();
        inimigo.posicao.y += inimigo.getvelY();
        inimigo.posicao.x += inimigo.getvelX();
        inimigoGhost.posicao.y += inimigoGhost.getvelY();
        inimigoGhost.posicao.x += inimigoGhost.getvelX();
        tiroPlayer.posicao.y += tiroPlayer.getvelY();
        tiroPlayer.posicao.x += tiroPlayer.getvelX();
        tiroInimigo.posicao.y += tiroInimigo.getvelY()*2;
        tiroInimigo.posicao.x += tiroInimigo.getvelX()*2;

    }

    void trataIA(bool * atira) {
        raycast.posicao.x = inimigo.posicao.x;
        raycast.posicao.y = inimigo.posicao.y;

        if (SDL_HasIntersection(&raycast.posicao, &player.posicao)) {
            *atira = true;
            cout << "sss";
        }
    }

    bool Musica(int x) {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            return false;
            cout << "Erro ao Iniciar funcao de audio\n";
        } else {
            Mix_OpenAudio(45100, AUDIO_S16SYS, 1, 512);
            fase1 = Mix_LoadMUS("sons/Enjoy The Silence.mp3");
            fase2 = Mix_LoadMUS("sons/Little Dark Age.mp3");
            efeito = Mix_LoadWAV("sons/Explosao.mp3");
            musicaMenu = Mix_LoadMUS("sons/Metro.mp3");
        }

        if (x == 0) {
            Mix_PlayMusic(fase1, -1);
        }
        if (x == 1) {
            Mix_PlayMusic(fase2, -1);
        }
        if (x == 2) {
            Mix_PlayChannel(-1, efeito, 0);
        }
        if (x == 3) {
            Mix_PlayMusic(musicaMenu, -1);
        }
    }

public:

    bool init() {
        //Initialization flag
        bool success = true;

        Mix_OpenAudio(45100, AUDIO_S16SYS, 1, 512);
        efeito = Mix_LoadWAV("sons/Explosao.mp3");

        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                printf("Warning: Linear texture filtering not enabled!");
            }

            //Create window
            gWindow = SDL_CreateWindow("SkyStrike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL) {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Create vsynced renderer for window
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if (gRenderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags)) {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }
        return success;
    }

    bool loadMedia() {
        //Loading success flag
        bool success = true;
        int i = 0;
        string root;
        string indice;

        mira.CarregaTextura(gRenderer, "textures/mira.png");
        player.CarregaTextura(gRenderer, "textures/tucano.png");
        inimigo.CarregaTextura(gRenderer, "textures/tucano_enemy.png");
        tiroPlayer.CarregaTextura(gRenderer, "textures/bala.png");
        tiroInimigo.CarregaTextura(gRenderer, "textures/bala.png");
        bgFase.CarregaTextura(gRenderer, "textures/bg1.png");
        bgMenu.CarregaTextura(gRenderer, "textures/bgMenu.png");
        bgRegras.CarregaTextura(gRenderer, "textures/bgMenuRegras.png");
        explosao.CarregaTextura(gRenderer, "textures/explosion/sprite_1");
        barraPlayer.CarregaTextura(gRenderer, "textures/barraPlayer.png");
        //raycast.CarregaTextura(gRenderer, "textures/bgMenu.png");

        mira.posicao.w = 20;
        mira.posicao.h = 20;

        tiroInimigo.posicao.w = 16;
        tiroInimigo.posicao.h = 8;

        barraPlayer.posicao.w = 500;
        barraPlayer.posicao.h = 20;

        tiroPlayer.posicao.w = 16;
        tiroPlayer.posicao.h = 8;

        explosao.posicao.w = 68;
        explosao.posicao.h = 68;

        raycast.posicao.w = 250;
        raycast.posicao.h = 30;

        player.posicao.w = 64;
        player.posicao.h = 32;

        inimigo.posicao.w = 64;
        inimigo.posicao.h = 32;

        bgFase.posicao.w = 1280;
        bgFase.posicao.h = 600;

        bgMenu.posicao.w = 1280;
        bgMenu.posicao.h = 600;

        bgRegras.posicao.w = 1280;
        bgRegras.posicao.h = 600;

        return success;
    }

    void close() {
        int i;

        //Free loaded images
        SDL_DestroyTexture(mira.textura);
        SDL_DestroyTexture(player.textura);
        SDL_DestroyTexture(inimigo.textura);
        SDL_DestroyTexture(bgFase.textura);
        SDL_DestroyTexture(bgMenu.textura);
        SDL_DestroyTexture(barraPlayer.textura);
        //for (i = 0; i < 74; i++) {
        SDL_DestroyTexture(explosao.textura);
        //}
        SDL_DestroyTexture(bgRegras.textura);
        //for (i = 0; i < 15; i++) {
        SDL_DestroyTexture(tiroPlayer.textura);
        //}
        SDL_DestroyTexture(tiroInimigo.textura);
        SDL_DestroyTexture(raycast.textura);

        //Destroy window	
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    bool gameLoop() {
        //Main loop flag
        bool quit = true;
        bool quitMenu = false;
        bool Regras = false;
        bool atirou = false;
        bool inimigoAtirou = false;
        bool pouso = false;
        bool acidente = false;
        int gameOver = 0;
        int vidaPlayer = 100;
        int vidaInimigo = 100;
        int i;
        int fase = 1;
        //int i;

        montaTela();
        //While running
        while (!quitMenu) {
            //Musica(3);
            inputMenu(&quitMenu, &quit, &Regras);
            desenha(quit, Regras, i, fase);
            if (quit) {
                montaTela();
                vidaPlayer = 20;
                vidaInimigo = 20;
                fase = 1;
                gameOver = 0;
                pouso = false;
                bgFase.CarregaTextura(gRenderer, "textures/bg1.png");
                Musica(0);
            }
            while (!quit) {
                barraPlayer.posicao.w = 25 * vidaPlayer;

                //input do player
                playerInput(&quit, &atirou);
                
                if (fase == 1) {
                    if (player.posicao.y >= 560 && player.posicao.x > 295 && player.posicao.x < 791) {
                        player.posicao.y = 560;
                        if (player.getangulo() > 300 && player.getangulo() < 360) {
                            pouso = true;
                        } else {
                            acidente = true;
                        }
                    }
                }
                
                if (fase == 2) {
                    if (player.posicao.y >= 456 && player.posicao.x > 869 && player.posicao.x < 1280) {
                        player.posicao.y = 456;
                        if (player.getangulo() > 300 && player.getangulo() < 360) {
                            pouso = true;
                        } else {
                            acidente = true;
                        }
                    }
                }
            

            //cuida da angulacao e velocidade do tiro na hora que é atirado
            tiroPlayer.atira(player.getvelX(), player.getvelY(), player.getangulo(), player.posicao.x, player.posicao.y);
            tiroInimigo.atira(inimigo.getvelX(), inimigo.getvelY(), inimigo.getangulo(), inimigo.posicao.x, inimigo.posicao.y);

            //trata das posicoes em funcao da velocidade
            trataFisica(pouso, acidente);

            //trata colisoes
            if (SDL_HasIntersection(&tiroPlayer.posicao, &inimigo.posicao)) {
                i = 0;
                explosao.posicao.x = inimigo.posicao.x;
                explosao.posicao.y = inimigo.posicao.y;
                vidaInimigo -= 1;
                Musica(2);
                Mix_PlayChannel(-1, efeito, 0);
            }

            if (SDL_HasIntersection(&tiroInimigo.posicao, &player.posicao)) {
                i = 0;
                explosao.posicao.x = player.posicao.x;
                explosao.posicao.y = player.posicao.y;
                vidaPlayer -= 1;
                Musica(2);
                Mix_PlayChannel(-1, efeito, 0);
            }

            if (acidente) {
                explosao.posicao.x = player.posicao.x;
                explosao.posicao.y = player.posicao.y;
                Musica(2);
                Mix_PlayChannel(-1, efeito, 0);
            }

            if (vidaPlayer <= 0 || acidente) {
                gameOver = 1;
                if (fase == 1) {
                    bgFase.CarregaTextura(gRenderer, "textures/bg1go.png");
                } else if (fase == 2) {
                    bgFase.CarregaTextura(gRenderer, "textures/bg2go.png");
                }
                desenha(quitMenu, Regras, i, fase);
                SDL_Delay(4000);
                quit = true;
            } else if (fase == 1 && vidaInimigo <= 0 && pouso) {
                fase = 2;
                vidaInimigo = 20;
                bgFase.CarregaTextura(gRenderer, "textures/bg2d.png");
                desenha(quitMenu, Regras, i, fase);
                SDL_Delay(4000);
                pouso = false;
                player.posicao.y = 300;
                bgFase.CarregaTextura(gRenderer, "textures/bg2.png");
                Musica(1);
            } else if (fase == 2 && vidaInimigo <= 0 && pouso) {
                gameOver = 2;
                bgFase.CarregaTextura(gRenderer, "textures/bg2go2.png");
                desenha(quitMenu, Regras, i, fase);
                SDL_Delay(4000);
                quit = true;
            }

            cout << vidaInimigo << " ";

            //atrela o raycast ao inimigo
            trataIA(&inimigoAtirou);

            //movimenta os avioes na tela
            player.trataVoo(mira);
            inimigoGhost.trataVoo(player);
            if (vidaInimigo <= 0) {
                inimigo.posicao.x = 2000;
            } else {
                inimigo.trataVoo(inimigoGhost);
            }
            //testa se o player ou o inimigo atiram
            atira(&atirou);
            inimigoAtira(&inimigoAtirou);

            if (i < 73) {
                i++;
            }

            //trata de desenhar os objetos do jogo
            desenha(quitMenu, Regras, i, fase);
        }
    }
}
};

int main(int argc, char* args[]) {
    TelaJogo SkyStrike;

    //Start up SDL and create window
    if (!SkyStrike.init()) {
        cout << "Failed to initialize!\n";
    } else {
        //Load media
        if (!SkyStrike.loadMedia()) {
            cout << "Failed to load media!\n";
        } else {
            SkyStrike.gameLoop();
        }
    }

    //Free resources and close SDL
    SkyStrike.close();

    return 0;
}

