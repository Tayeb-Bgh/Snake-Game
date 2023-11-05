#include "raylib.h"
#include <iostream>
#include "Snake.hpp"
#include <vector>
#include <random>
#include <string>


using namespace std;

void ButtonPressed(int touche,vector<Vector2>& stockPos,vector<Snake>& snake,int vitesseSnake)
{   
    int i,j;
    if (IsKeyPressed(touche))
    {
        if (stockPos[0].x < 0 && stockPos[0].y < 0)
            stockPos[0] = snake[0].DonnePos();
        else
            stockPos.push_back({ snake[0].DonnePos() });

        snake[0].ChangeDir(touche);
    }
    for (i = 1;i <= snake.size() - 1;i++)
    {
        for (j = 0;j < stockPos.size();j++)
        {
            if (snake[i].DonnePos().x == stockPos[j].x && snake[i].DonnePos().y == stockPos[j].y)
            {   
                snake[i].ChangeDir(touche);

                if (i == snake.size() - 1)
                {
                    if (j > 0)
                        stockPos.erase(stockPos.begin()+j);
                    else
                        stockPos[j] = { -10,-10 };
                }
            }
        }
    }
}

Vector2 GeneratePomme(bool& point)
{   
    return { static_cast<float>((rand() % 700) + 50) ,static_cast<float>((rand() % 650) + 110) };
}

void CollisionPomme(bool& point,Vector2 Coor_Point,vector<Snake>& tete, int largeurSnake, Vector2 dimSnake, int& score)
{
    if (CheckCollisionCircleRec({ Coor_Point.x,Coor_Point.y },10, { tete[0].DonnePos().x,tete[0].DonnePos().y,dimSnake.x,dimSnake.y } ))
    {
        score++;
        point = false;
        for (int i(1);i<=4;i++)
            tete.push_back(Snake({ tete[tete.size() - 1].DonnePos().x,tete[tete.size() - 1].DonnePos().y }, { tete[tete.size() - 1].DonneVit().x,tete[tete.size() - 1].DonneVit().y }, dimSnake));
    }
}

bool CollisionSnake(vector<Snake>& tete, vector<Vector2>& stockPosStart)
{   
    bool perdu = false;
    for (int i(4);i < tete.size();i++)
    {    
        if (CheckCollisionPointRec(tete[0].DonnePos(), { tete[i].DonnePos().x,tete[i].DonnePos().y,15,15 }) and ((tete[0].DonneVit().x == 0 && tete[i].DonneVit().y == 0) or (tete[0].DonneVit().y == 0 && tete[i].DonneVit().x == 0)))
        {   
            perdu = true;
            i = tete.size() - 1;
            while (i >= 0)
            {   
                if (i <= 14)
                {
                    stockPosStart[i] = tete[i].DonneVit();
                }
                tete[i].Perdu();
                i--;
            }
            i = tete.size() - 1;
            
        }
    }
    return perdu;
}

void ClearVector(vector<Vector2>& stockPos)
{
    while (stockPos.size() > 1)
    {
        stockPos.pop_back();
    }
    stockPos[0] = {-10,-10};
}

int main(int argc, char* argv[])
{
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Declaration de tous les elements dont on a besoin pour le jeu~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    int i;
    // Initialisation de la fen�tre et du mode graphique
    #define largeurEcran 800
    #define hauteurEcran 800
    InitWindow(largeurEcran, hauteurEcran, "Snake");
    SetTargetFPS(60);
    
    // Creation du serpent 
    #define vitesseSnake 5
    #define largeurSnake 14
    #define longeurSnake 14
    Vector2 dimSnake({ largeurSnake,longeurSnake });
    vector<Snake> snake;

    snake.push_back(Snake()); // La tete 

    for (i=1;i <= 14;i++) // Le reste du corp
        snake.push_back(Snake({ snake[i - 1].DonnePos().x,snake[i - 1].DonnePos().y }, { snake[i - 1].DonneVit().x,snake[i - 1].DonneVit().y }, dimSnake));

    // Declaration des listes chainee qui servent a stocker les coordonnees ou le reste du serpent doit tourner
    vector<Vector2> stockPosZ(1, { -10,-10 });
    vector<Vector2> stockPosS(1, { -10,-10 });
    vector<Vector2> stockPosQ(1, { -10,-10 });
    vector<Vector2> stockPosD(1, { -10,-10 });
    vector<Vector2> stockPosRestart(15, { 0,0 });

    
    bool pomme(false);
    Vector2 Coor_Pomme = { 200,200 };

    bool perdu = false;
    int score = 0;
    string scoreAffiche;
    int time = 0;
    int menu = 0;
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Boucle Pricipal Du Jeu~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    while (!WindowShouldClose())
    {
        if (menu == 1)
        {
            if (perdu == false)
            {
                // Deplacement du Snake
                for (i = 0;i <= snake.size() - 1;i++)
                    snake[i].Deplace(snake[i].DonnePos());

                if (!(snake[0].DonneVit().x == 0 && snake[0].DonneVit().y == 0))
                {
                    ButtonPressed(KEY_W, stockPosZ, snake, vitesseSnake);
                    ButtonPressed(KEY_S, stockPosS, snake, vitesseSnake);
                    ButtonPressed(KEY_D, stockPosD, snake, vitesseSnake);
                    ButtonPressed(KEY_A, stockPosQ, snake, vitesseSnake);

                    // Generer une pomme si il y'en a aucune
                    if (pomme == false)
                    {
                        Coor_Pomme = GeneratePomme(pomme);
                        pomme = true;
                    }
                    CollisionPomme(pomme, Coor_Pomme, snake, largeurSnake, dimSnake, score);


                    if (CollisionSnake(snake, stockPosRestart) == true)
                        perdu = true;
                    else
                        perdu = false;
                }
            }
            else
            {   
                if (snake.size() > 15)
                {   
                    // On diminue la taille du serpent avant le restart de la game
                    if (time >= 3) // on met du delais avant chaque suppression pour faire un truc stylé
                    {
                        snake.pop_back();
                        time = 0;
                    }
                }
                else
                {
                    perdu = false;
                    snake[0] = Snake();
                    for (i = 1;i <= 14;i++) 
                    {
                        snake[i] = Snake({ snake[i - 1].DonnePos().x,snake[i - 1].DonnePos().y }, { snake[i - 1].DonneVit().x,snake[i - 1].DonneVit().y }, dimSnake);
                    }
                    ClearVector(stockPosZ);
                    ClearVector(stockPosS);
                    ClearVector(stockPosQ);
                    ClearVector(stockPosD);
                    score = 0;
                    menu = 0;
                }
            }
            time++;
        }
        else
        {
            if (IsKeyPressed(KEY_SPACE))
                menu = 1;
        }
        
        
        // Dessin des �l�ments du jeu
        BeginDrawing();
        ClearBackground(BLACK);

        if (menu == 0)
            DrawText("Appuyer sur ESPACE pour lancer le jeu !", 100, 200, 30, RED);
        else
            DrawCircle(Coor_Pomme.x, Coor_Pomme.y, 10, RED);

        for (i = 0;i <= snake.size() - 1;i++)   
            DrawRectangleRounded({ static_cast<float> (snake[i].DonnePos().x), static_cast<float>(snake[i].DonnePos().y), largeurSnake, longeurSnake }, 0.5, 6, WHITE);
        
        DrawLine(0, 100, 800, 100, RED);
        scoreAffiche = "Score : " + to_string(score);
        DrawText(scoreAffiche.c_str(), largeurEcran / 2 - 130, 20, 60, WHITE);
      
        EndDrawing();
    }

    CloseWindow();

    return 0;
}


