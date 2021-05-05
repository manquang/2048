
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
/**************************************************************/
using namespace std;
const string WINDOW_TITLE = "2048";
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 600;
const int box_border = 8;
int checkRush = 0;
int score = 0;
int best;
bool cntinue = true;
/***********************************************************************/

void makeBorder(SDL_Renderer* renderer, SDL_Rect& filled_borderRect, SDL_Rect& NewgameRect);
void Init(SDL_Window*& window, SDL_Renderer*& renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void logSDLError(ostream& os, const std::string& msg, bool fatal);
void waitUntilKeyPressed();
void box_location(SDL_Rect& filled_borderRect, SDL_Rect**& box_rect, const int box_size);
bool checkwin(int**& arrValue);
bool checkPut(int**& arrValue);
void randomOnboard(int**& arrValue);
void rushTop(int**& arrValue);
void rushBot(int**& arrValue);
void rushLeft(int**& arrValue);
void rushRight(int**& arrValue);
void moveTop(int**& arrValue, int& score);
void moveBot(int**& arrValue, int& score);
void moveLeft(int**& arrValue, int& score);
void moveRight(int**& arrValue, int& score);
bool checklose(int**& arrValue);
void start(SDL_Renderer* renderer, SDL_Window*& window, SDL_Rect**& box_rect, int**& arrValue,
    int box_size, SDL_Rect& NewgameRect, SDL_Rect& filled_borderRect);
void paint(TTF_Font* font, int value, SDL_Color color, SDL_Renderer* renderer, SDL_Rect box_rect, int box_size);
void paint_string(SDL_Renderer* renderer, SDL_Rect rect, string x, TTF_Font* font, SDL_Color color);
void play_again(int**& arrValue);
void paint_Score_Best(SDL_Renderer* renderer, int score, int best);
void lose(SDL_Renderer* renderer, SDL_Window*& window, int**& arrValue);
void menu(SDL_Renderer* renderer, SDL_Window*& window, SDL_Rect**& box_rect, int**& arrValue,
    int box_size, SDL_Rect& NewgameRect, SDL_Rect& filled_borderRect);
/*****************************************************************/
int main(int argc, char** argv)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Init(window, renderer);
    srand(time(NULL));
    SDL_Rect filled_borderRect, NewgameRect;
    filled_borderRect.x = (SCREEN_WIDTH - 400) / 2; //=50
    filled_borderRect.y = SCREEN_HEIGHT - 450;  //=150
    filled_borderRect.w = 400;
    filled_borderRect.h = 400;
    int box_size = (filled_borderRect.w - 5 * box_border) / 4;
    int** arrValue;
    arrValue = new int* [4];
    for (int i = 0; i < 4; i++)
    {
        arrValue[i] = new int[4];
        for (int j = 0; j < 4; j++)
        {
            arrValue[i][j] = 0;
        }
    }
    SDL_Rect** box_rect;
    box_location(filled_borderRect, box_rect, box_size);

    menu(renderer, window, box_rect, arrValue, box_size, NewgameRect, filled_borderRect);

    for (int i = 0; i < 4; i++)
    {
        delete[]arrValue[i];
    }
    delete[]arrValue;
    quitSDL(window, renderer);
    return 0;
}
/**************************************************/

void logSDLError(ostream& os, const std::string& msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}
void Init(SDL_Window*& window, SDL_Renderer*& renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout, "SDL_Init", true);
    if (TTF_Init() == -1) logSDLError(std::cout, "Font", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "SDL_Init", true);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "SDL_Init", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void makeBorder(SDL_Renderer* renderer, SDL_Rect& filled_borderRect, SDL_Rect& NewgameRect)
{
    SDL_Rect image;
    image.x = 0;
    image.y = 0;
    image.w = 500;
    image.h = 600;
    SDL_Surface* tempSurface = IMG_Load("Image/Konachan.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_RenderCopy(renderer, texture, NULL, &image);
    SDL_Color color = { 255, 26, 26 };
    TTF_Font* font = TTF_OpenFont("OpenSans-ExtraBold.ttf", 200);
    SDL_Rect q2048_, menu;
    q2048_.x = 30;
    q2048_.y = 20;
    q2048_.w = 160;
    q2048_.h = 80;
    menu.x = 220;
    menu.y = 80;
    menu.w = 110;
    menu.h = 40;
    NewgameRect.x = 340;
    NewgameRect.y = 80;
    NewgameRect.w = 110;
    NewgameRect.h = 40;
    tempSurface = IMG_Load("Image/Newgame.png");
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_RenderCopy(renderer, texture, NULL, &NewgameRect);
    tempSurface = IMG_Load("Image/menu.png");
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_RenderCopy(renderer, texture, NULL, &menu);
    SDL_SetRenderDrawColor(renderer, 173, 173, 133, 255);
    paint_string(renderer, q2048_, "2048", font, color);

    SDL_SetRenderDrawColor(renderer, 173, 173, 133, 255);
    SDL_RenderFillRect(renderer, &filled_borderRect);
    SDL_FreeSurface(tempSurface);
    SDL_DestroyTexture(texture);
}
void box_location(SDL_Rect& filled_borderRect, SDL_Rect**& box_rect, const int box_size)
{
    box_rect = new SDL_Rect * [4];
    box_rect[0] = new SDL_Rect[4];
    box_rect[0][0].x = filled_borderRect.x + box_border;
    box_rect[0][0].y = filled_borderRect.y + box_border;
    box_rect[0][0].w = box_size;
    box_rect[0][0].h = box_size;
    for (int i = 0; i < 4; i++)
    {
        if (i == 0) {}
        else box_rect[i] = new SDL_Rect[4];
        if (i == 0) {}
        else {
            box_rect[i][0].x = box_rect[i - 1][0].x;
            box_rect[i][0].y = box_rect[i - 1][0].y + box_border + box_size;
            box_rect[i][0].w = box_size;
            box_rect[i][0].h = box_size;
        }
        for (int j = 1; j < 4; j++)
        {
            box_rect[i][j].x = box_rect[i][j - 1].x + box_border + box_size;
            box_rect[i][j].y = box_rect[i][j - 1].y;
            box_rect[i][j].w = box_size;
            box_rect[i][j].h = box_size;
        }
    }
}
void start(SDL_Renderer* renderer, SDL_Window*& window, SDL_Rect**& box_rect, int**& arrValue,
    int box_size, SDL_Rect& NewgameRect, SDL_Rect& filled_borderRect)
{
    TTF_Font* font = TTF_OpenFont("OpenSans-ExtraBold.ttf", 80);
    SDL_Color color = { 71, 107, 107,255 };
    if (cntinue) {
        randomOnboard(arrValue);
        randomOnboard(arrValue);
    }
    SDL_Surface* tempSurface;
    SDL_Texture* texture;
    SDL_Event e;
    bool win = false, stop = false;
    while (1) {
        SDL_Event ev;
        fstream myfile("best.txt", ios::in);
        myfile >> best;
        myfile.close();
        while (checkwin(arrValue)) {
            win = true;
            SDL_Rect bruh, playagainrect;
            bruh.x = 100;
            bruh.y = 200;
            bruh.w = 300;
            bruh.h = 200;
            tempSurface = IMG_Load("Image/0ff89b0e9bdfba93618bcfab75c5fd86.jpg");
            texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_RenderCopy(renderer, texture, NULL, &bruh);
            playagainrect.x = 150;
            playagainrect.y = 400;
            playagainrect.w = 200;
            playagainrect.h = 100;
            tempSurface = IMG_Load("Image/playagain.png");
            texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_RenderCopy(renderer, texture, NULL, &playagainrect);
            SDL_RenderPresent(renderer);
            if (SDL_WaitEvent(&ev) == 0) return;
            if (ev.type == SDL_QUIT)
            {
                stop = true;
            }
            if (ev.type == SDL_MOUSEBUTTONDOWN)
                if (ev.button.button == SDL_BUTTON_LEFT && ev.button.x >= playagainrect.x && ev.button.y >= playagainrect.y &&
                    ev.button.x <= playagainrect.x + playagainrect.w && ev.button.y <= playagainrect.h + playagainrect.y)
                {
                    win = false;
                    score = 0;
                    play_again(arrValue);
                    randomOnboard(arrValue);
                    randomOnboard(arrValue);
                    SDL_Rect image;
                    image.x = 0;
                    image.y = 0;
                    image.w = 500;
                    image.h = 600;
                    tempSurface = IMG_Load("Image/Konachan.png");
                    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                    SDL_RenderCopy(renderer, texture, NULL, &image);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(3000);
                    break;
                }
        }
        while (checklose(arrValue) && (!win)) { lose(renderer, window, arrValue);continue; }
        checkRush = 0;
        if (SDL_WaitEvent(&e) == 0)return;
        if (e.type == SDL_QUIT) stop = true;
        if (e.type == SDL_MOUSEBUTTONDOWN && (!win))
        {
            if (e.button.button == SDL_BUTTON_LEFT && e.button.x >= NewgameRect.x &&
                e.button.x <= (NewgameRect.x + NewgameRect.w) && e.button.y >= NewgameRect.y &&
                e.button.y <= (NewgameRect.y + NewgameRect.h))
            {
                score = 0;
                play_again(arrValue);
                randomOnboard(arrValue);
                randomOnboard(arrValue);
                continue;
            }
            if (e.button.button == SDL_BUTTON_LEFT && e.button.x >= 220 && e.button.x <= 330
                && e.button.y >= 80 && e.button.y <= 120)
            {
                cntinue = false;
                menu(renderer, window, box_rect, arrValue, box_size, NewgameRect, filled_borderRect);
            }
        }
        if (e.type == SDL_KEYDOWN && (!win))
        {
            if (e.key.keysym.sym == SDLK_LEFT) moveLeft(arrValue, score);
            else if (e.key.keysym.sym == SDLK_RIGHT) moveRight(arrValue, score);
            else if (e.key.keysym.sym == SDLK_DOWN) moveBot(arrValue, score);
            else if (e.key.keysym.sym == SDLK_UP) moveTop(arrValue, score);
            if (checkRush == 0) continue;
            randomOnboard(arrValue);
        }
        makeBorder(renderer, filled_borderRect, NewgameRect);
        for (int i = 0;i < 4;i++)
        {
            for (int j = 0;j < 4;j++)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 204, 255);
                SDL_RenderFillRect(renderer, &box_rect[i][j]);
                paint(font, arrValue[i][j], color, renderer, box_rect[i][j], box_size);
            }
        }
        if (score > best)
        {
            fstream yourfile("best.txt", ios::out);
            yourfile << score;
            yourfile.close();
        }
        paint_Score_Best(renderer, score, best);
        SDL_RenderPresent(renderer);
        if (stop) { SDL_DestroyWindow(window); break; }
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(tempSurface);
}
bool checkwin(int**& arrValue)
{
    bool win = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (arrValue[i][j] == 2048) win = true;
        }
    }
    return win;
}
bool checkPut(int**& arrValue)
{
    bool put = 0;
    for (int i = 0;i < 4;i++)
    {
        for (int j = 0;j < 4;j++)
        {
            if (arrValue[i][j] == 0) put = 1;
        }
    }
    return put;
}
void randomOnboard(int**& arrValue)
{
    int i = (rand() % 2) * 2 + 2;
    if (checkPut(arrValue))
    {
        int x = rand() % 4;
        int y = rand() % 4;
        if (arrValue[x][y] == 0) arrValue[x][y] = i;
        else {
            while (arrValue[x][y] != 0)
            {
                x = rand() % 4;
                y = rand() % 4;
            }
            arrValue[x][y] = i;
        }
    }
}
void rushTop(int**& arrValue)
{

    for (int i = 1;i < 4;i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (arrValue[i][j] != 0)
            {
                for (int z = i; z > 0;z--)
                {
                    if (arrValue[z - 1][j] == 0)
                    {
                        ++checkRush;
                        arrValue[z - 1][j] = arrValue[z][j];
                        arrValue[z][j] = 0;

                    }
                }
            }
        }
    }

}
void rushBot(int**& arrValue)
{
    for (int i = 2;i >= 0;i--)
    {
        for (int j = 3; j >= 0; j--)
        {
            if (arrValue[i][j] != 0)
            {
                for (int z = i; z < 3;z++)
                {
                    if (arrValue[z + 1][j] == 0)
                    {
                        ++checkRush;
                        arrValue[z + 1][j] = arrValue[z][j];
                        arrValue[z][j] = 0;
                    }
                }
            }
        }
    }
}
void rushLeft(int**& arrValue)
{
    for (int j = 1;j < 4;j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (arrValue[i][j] != 0)
            {
                for (int z = j; z > 0;z--)
                {
                    if (arrValue[i][z - 1] == 0)
                    {
                        ++checkRush;
                        arrValue[i][z - 1] = arrValue[i][z];
                        arrValue[i][z] = 0;
                    }
                }
            }
        }
    }
}
void rushRight(int**& arrValue)
{
    for (int j = 2;j >= 0;j--)
    {
        for (int i = 3; i >= 0; i--)
        {
            if (arrValue[i][j] != 0)
            {
                for (int z = j; z < 3;z++)
                {
                    if (arrValue[i][z + 1] == 0)
                    {
                        ++checkRush;
                        arrValue[i][z + 1] = arrValue[i][z];
                        arrValue[i][z] = 0;
                    }
                }
            }
        }
    }
}
void moveTop(int**& arrValue, int& score)
{
    rushTop(arrValue);
    for (int i = 1;i < 4;i++)
    {
        for (int j = 0;j < 4; j++)
        {
            if (arrValue[i][j] != 0)
            {
                if (arrValue[i - 1][j] == arrValue[i][j])
                {
                    ++checkRush;
                    score += arrValue[i][j] * 2;
                    arrValue[i - 1][j] += arrValue[i][j];
                    arrValue[i][j] = 0;
                }
            }
        }
    }
    rushTop(arrValue);
}
void moveBot(int**& arrValue, int& score)
{
    rushBot(arrValue);
    for (int i = 2;i >= 0;i--)
    {
        for (int j = 3;j >= 0; j--)
        {
            if (arrValue[i][j] != 0)
            {
                if (arrValue[i + 1][j] == arrValue[i][j])
                {
                    ++checkRush;
                    score += arrValue[i][j] * 2;
                    arrValue[i + 1][j] += arrValue[i][j];
                    arrValue[i][j] = 0;
                }
            }
        }
    }
    rushBot(arrValue);
}
void moveLeft(int**& arrValue, int& score)
{
    rushLeft(arrValue);
    for (int i = 0;i < 4; i++)
    {
        for (int j = 1;j < 4;j++)
        {
            if (arrValue[i][j] != 0)
            {
                if (arrValue[i][j - 1] == arrValue[i][j])
                {
                    ++checkRush;
                    score += arrValue[i][j] * 2;
                    arrValue[i][j - 1] += arrValue[i][j];
                    arrValue[i][j] = 0;
                }
            }
        }
    }
    rushLeft(arrValue);
}
void moveRight(int**& arrValue, int& score)
{
    rushRight(arrValue);
    for (int j = 2;j >= 0;j--)
    {
        for (int i = 3;i >= 0; i--)
        {
            if (arrValue[i][j] != 0)
            {

                if (arrValue[i][j + 1] == arrValue[i][j])
                {
                    ++checkRush;
                    score += arrValue[i][j] * 2;
                    arrValue[i][j + 1] += arrValue[i][j];
                    arrValue[i][j] = 0;
                }
            }
        }
    }
    rushRight(arrValue);
}
bool checklose(int**& arr)
{
    bool lose = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - 1; j++) {
            if (arr[i][j] == arr[i][j + 1]) lose = 0;
        }
    }
    for (int i = 0; i < 4 - 1; i++) {
        for (int j = 0; j < 4; j++) {
            if (arr[i][j] == arr[i + 1][j]) lose = 0;
        }
    }
    return lose;
}
void paint(TTF_Font* font, int value, SDL_Color color, SDL_Renderer* renderer, SDL_Rect box_rect, int box_size)
{
    SDL_Rect number;
    if (value != 0) {
        switch (value)
        {
        case 2:case 4:case 8:
            number.w = box_size / 3;
            number.h = box_size / 4 * 3;
            number.x = box_rect.x + box_size / 3;
            number.y = box_rect.y + box_size / 8;
            break;
        case 16:case 32: case 64:
            number.w = box_size / 5 * 3;
            number.h = box_size / 4 * 3;
            number.x = box_rect.x + box_size / 5;
            number.y = box_rect.y + box_size / 8;
            break;
        case 128: case 256: case 512:
            number.w = box_size / 5 * 3;
            number.h = box_size / 7 * 4;
            number.x = box_rect.x + box_size / 5;
            number.y = box_rect.y + box_size / 7 * 1.5;
            break;
        case 1024: case 2048:
            number.w = box_size / 5 * 4;
            number.h = box_size / 7 * 4;
            number.x = box_rect.x + box_size / 10;
            number.y = box_rect.y + box_size / 7 * 1.5;
        }
        string x = to_string(value);
        paint_string(renderer, number, x, font, color);
    }
}
void paint_string(SDL_Renderer* renderer, SDL_Rect rect, string x, TTF_Font* font, SDL_Color color)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, x.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0.0, NULL, SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}
void play_again(int**& arrValue)
{
    for (int i = 0;i < 4;i++)
    {
        for (int j = 0;j < 4;j++)
        {
            arrValue[i][j] = 0;
        }
    }
}
void paint_Score_Best(SDL_Renderer* renderer, int score, int best)
{
    TTF_Font* font = TTF_OpenFont("OpenSans-ExtraBold.ttf", 50);
    SDL_Color color;
    SDL_Rect ScoreValue, BestValue, ScoreRect, Score_String, BestRect, Best_String;
    ScoreRect.x = 220;
    ScoreRect.y = 15;
    ScoreRect.w = 110;
    ScoreRect.h = 50;
    Score_String.x = 250;
    Score_String.y = 17;
    Score_String.w = 50;
    Score_String.h = 20;
    //Score
    color = { 214, 214, 194 };
    SDL_SetRenderDrawColor(renderer, 173, 173, 133, 255);
    SDL_RenderFillRect(renderer, &ScoreRect);
    paint_string(renderer, Score_String, "Score", font, color);

    Best_String.x = 375;
    Best_String.y = 17;
    Best_String.w = 40;
    Best_String.h = 20;
    BestRect.x = 340;
    BestRect.y = 15;
    BestRect.w = 110;
    BestRect.h = 50;
    //  Best
    color = { 214, 214, 194 };
    SDL_SetRenderDrawColor(renderer, 173, 173, 133, 255);
    SDL_RenderFillRect(renderer, &BestRect);
    paint_string(renderer, Best_String, "Best", font, color);

    color = { 255,255,255 };
    string x = to_string(score);
    ScoreValue.y = 35;
    ScoreValue.h = 30;
    ScoreValue.x = (220 + 110 / 2) - (x.length() + 1) * 10 / 2;
    ScoreValue.w = (x.length() + 1) * 10;
    paint_string(renderer, ScoreValue, x, font, color);
    x = to_string(best);
    BestValue.y = 35;
    BestValue.h = 30;
    BestValue.x = (340 + 110 / 2) - (x.length() + 1) * 10 / 2;
    BestValue.w = (x.length() + 1) * 10;
    paint_string(renderer, BestValue, x, font, color);
}
void lose(SDL_Renderer* renderer, SDL_Window*& window, int**& arrValue)
{
    SDL_Color color;
    TTF_Font* font = TTF_OpenFont("OpenSans-ExtraBold.ttf", 80);
    SDL_Rect lose_rect, GameOverRect, PlayAgainRect;
    lose_rect.x = SCREEN_WIDTH / 2 - 300 / 2;
    lose_rect.y = SCREEN_HEIGHT / 2 - 100;
    lose_rect.w = 300;
    lose_rect.h = 200;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &lose_rect);

    //Game Over
    GameOverRect.w = 240;
    GameOverRect.h = 70;
    GameOverRect.x = 130;
    GameOverRect.y = 230;
    SDL_Surface* tempSurface = IMG_Load("Image/gameover.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_RenderCopy(renderer, texture, NULL, &GameOverRect);
    //Play Again
    PlayAgainRect.w = 120;
    PlayAgainRect.h = 40;
    PlayAgainRect.x = 190;
    PlayAgainRect.y = 320;
    tempSurface = IMG_Load("Image/playagain.png");
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_RenderCopy(renderer, texture, NULL, &PlayAgainRect);

    SDL_RenderPresent(renderer);
    SDL_Event e;
    if (SDL_WaitEvent(&e) == 0)return;
    if (e.type == SDL_QUIT)
    {
        SDL_DestroyWindow(window);
    }
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_LEFT && e.button.x >= PlayAgainRect.x &&
            e.button.x <= (PlayAgainRect.x + PlayAgainRect.w) && e.button.y >= PlayAgainRect.y &&
            e.button.y <= (PlayAgainRect.y + PlayAgainRect.h))
        {
            score = 0;
            play_again(arrValue);
            randomOnboard(arrValue);
            randomOnboard(arrValue);
        }
    }
}
void menu(SDL_Renderer* renderer, SDL_Window*& window, SDL_Rect**& box_rect, int**& arrValue,
    int box_size, SDL_Rect& NewgameRect, SDL_Rect& filled_borderRect)
{
    SDL_Surface* tempSurface;
    SDL_Texture* texture;
    while (1) {
        SDL_Rect background;
        background.x = 0;
        background.y = 0;
        background.w = 500;
        background.h = 600;
        SDL_SetRenderDrawColor(renderer, 230, 240, 255, 255);
        SDL_RenderFillRect(renderer, &background);
        SDL_RenderPresent(renderer);
        SDL_Rect Exit, Playgame, Help;
        Exit.x = 150;
        Exit.y = 360;
        Exit.w = 200;
        Exit.h = 60;
        Playgame.x = 150;
        Playgame.y = 180;
        Playgame.w = 200;
        Playgame.h = 70;
        Help.x = 150;
        Help.y = 270;
        Help.w = 200;
        Help.h = 70;
        tempSurface = IMG_Load("Image/help.jpg");
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_RenderCopy(renderer, texture, NULL, &Help);
        tempSurface = IMG_Load("Image/playgame.jpg");
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_RenderCopy(renderer, texture, NULL, &Playgame);
        tempSurface = IMG_Load("Image/exit.png");
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_RenderCopy(renderer, texture, NULL, &Exit);
        SDL_RenderPresent(renderer);
        SDL_Event e;
        while (SDL_WaitEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) SDL_DestroyWindow(window);
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (e.button.x >= Playgame.x && e.button.x <= Playgame.x + Playgame.w
                        && e.button.y >= Playgame.y && e.button.y <= Playgame.y + Playgame.h)
                        start(renderer, window, box_rect, arrValue, box_size, NewgameRect, filled_borderRect);
                    if (e.button.x >= Help.x && e.button.x <= Help.x + Help.w
                        && e.button.y >= Help.y && e.button.y <= Help.y + Help.h)
                    {
                        SDL_Rect huongdan;
                        huongdan.x = 50;
                        huongdan.y = 100;
                        huongdan.w = 400;
                        huongdan.h = 400;
                        tempSurface = IMG_Load("Image/huongdan.png");
                        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                        SDL_RenderCopy(renderer, texture, NULL, &huongdan);
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &huongdan);
                        SDL_RenderPresent(renderer);
                        SDL_Event ev;
                        while (SDL_WaitEvent(&ev) != 0)
                        {
                            if (ev.type == SDL_QUIT) SDL_DestroyWindow(window);
                            if (ev.type == SDL_MOUSEBUTTONDOWN)
                            {
                                if (ev.button.button == SDL_BUTTON_LEFT)
                                {

                                    menu(renderer, window, box_rect, arrValue, box_size, NewgameRect, filled_borderRect);
                                }
                            }
                        }
                    }
                    if (e.button.x >= Exit.x && e.button.x <= Exit.x + Exit.w
                        && e.button.y >= Exit.y && e.button.y <= Exit.h + Exit.y)
                        SDL_DestroyWindow(window);
                }
            }
        }
    }
    SDL_FreeSurface(tempSurface);
    SDL_DestroyTexture(texture);
}

