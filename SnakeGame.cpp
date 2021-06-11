#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <string>

#define MAX_OPTION 3
#define FRAME_WIDTH 60
#define FRAME_HEIGHT 28

using namespace std;

#pragma comment(lib, "winmm.lib")

void gotoxy(int column, int line);
void SetColor(WORD color);
void ShowCur(bool CursorVisibility);
void ChooseOptions();
void StartGame();
void initHighScore();
void setupHighScore();
void checkHighScore(char _name[50], int _score);
void showHighScore();
void loadHighScore();
int whereX();
int whereY();

int Diem = 0;
int MoiAn = 0;
class HighScore {
public:
    int score;
    char name[50];
};

HighScore highscore[5];

struct Point {
    int x, y;
};

class CONRAN {
public:
    struct Point A[100];
    int DoDai;
    Point FOOD;
    CONRAN() {
        DoDai = 3;
        A[0].x = 12; A[0].y = 10;
        A[1].x = 11; A[1].y = 10;
        A[2].x = 10; A[2].y = 10;
        srand(time(NULL));
        FOOD.x = rand() % ((FRAME_WIDTH - 2) - 2 + 1) + 2;
        FOOD.y = rand() % ((FRAME_HEIGHT - 2) - 3 + 1) + 3;
    }
    void Ve() {
        ShowCur(false); // Ẩn con trỏ
        SetColor(12);

        // bảng hướng dẫn
        int Color;
        srand(time(NULL));
        Color = rand() % 15 + 1;
        SetColor(Color);
        for (int i = 70; i < 100; i++) {
            gotoxy(i, 0);
            cout << "#";
            gotoxy(i, 10 - 1);
            cout << "#";
        }
        for (int i = 1; i < 10; i++) {
            gotoxy(70, i);
            cout << "#";
            gotoxy(100 - 1, i);
            cout << "#";
        }
        gotoxy(72, 2);
        cout << " w = len ";
        gotoxy(72, 3);
        cout << " s = xuong ";
        gotoxy(72, 4);
        cout << " a = qua trai ";
        gotoxy(72, 5);
        cout << " s = qua phai ";
        gotoxy(72, 6);
        cout << " space = pause/continue ";
        gotoxy(72, 7);
        cout << " esc = quit ";
        gotoxy(70, 12);
        cout << "Score: " << Diem;

        // Vẽ con rắn
        SetColor(14);
        for (int i = 0; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            if (i == 0)
                cout << "@";
            else
                cout << "o";
        }
    }
    int anmoi(int _HardLevel)
    {
        if (A[0].x == FOOD.x && A[0].y == FOOD.y)
        {
            DoDai++;
            A[DoDai - 1] = A[DoDai - 2];
            PlaySound(TEXT("SNAKE_eatFood.wav"), NULL, SND_ASYNC);
            srand(time(NULL));
            FOOD.x = rand() % ((FRAME_WIDTH - 2) - 2 + 1) + 2;
            FOOD.y = rand() % ((FRAME_HEIGHT - 2) - 3 + 1) + 3;
            MoiAn += 1;
            if (MoiAn % 5 != 0)
            {
                Diem += 10;
            }
            else
                Diem += 20;
            Diem = Diem + (_HardLevel - 1) * 3;
            return 1;
        }
        else
        {
            if (MoiAn % 5 != 4)
            {
                gotoxy(FOOD.x, FOOD.y);
                SetColor(13);
                cout << "o";
                return 0;
            }
            else
            {
                gotoxy(FOOD.x, FOOD.y);
                SetColor(13);
                cout << "0";
                return 0;
            }
        }
    }
    int EndGame() {
        if ((A[0].x == 0) || (A[0].y == 0) || (A[0].x == FRAME_WIDTH - 1) || (A[0].y == FRAME_HEIGHT - 1)) {
            PlaySound(TEXT("SNAKE_endGame.wav"), NULL, SND_ASYNC);
            Sleep(1000);
            return 1;
        }
        else
        {
            for (int i = 1; i < DoDai; i++)
            {
                if (A[0].x == A[i].x && A[0].y == A[i].y) {
                    PlaySound(TEXT("SNAKE_endGame.wav"), NULL, SND_ASYNC);
                    Sleep(1000);
                    return 1;
                }
            }
            return 0;
        }
    }
    void DiChuyen(int Huong) {
        gotoxy(A[DoDai - 1].x, A[DoDai - 1].y);
        cout << " ";
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];
        if (Huong == 0) A[0].x = A[0].x + 1;
        if (Huong == 1) A[0].y = A[0].y + 1;
        if (Huong == 2) A[0].x = A[0].x - 1;
        if (Huong == 3) A[0].y = A[0].y - 1;
    }
};

int main()
{
    initHighScore();
    setupHighScore();
    ChooseOptions();
    loadHighScore();
    return 0;
}

// Kiểm tra đã có file highscore.txt chưa
void initHighScore() {
    fstream f;
    f.open("highscore.txt", ios::in);
    if (f.fail()) {
        f.close();
        f.open("highscore.txt", ios::out);
        char data[100] = "HIGH SCORE\n"
            "PLAYER 0\n"
            "PLAYER 0\n"
            "PLAYER 0\n"
            "PLAYER 0\n"
            "PLAYER 0\n";
        f << data;
        f.close();
    }
    f.close();
}

// Load high score vào chương trình
void setupHighScore() {
    fstream f;
    f.open("highscore.txt", ios::in);
    char line[50];
    int Count = 0, i = 0;
    while (!f.eof()) {
        Count++;
        f >> line;
        if (Count > 2)
            if (Count % 2 == 1) {
                strcpy_s(highscore[i].name, line);
            }
            else {
                highscore[i].score = atoi(line);
                i++;
            }
    }
    f.close();
}

// Kiểm tra xem có lọt top không
void checkHighScore(char _name[50], int _score) {
    for (int i = 0; i < 5; i++) {
        if (_score >= highscore[i].score) {
            for (int j = 4; j > i; j--) {
                strcpy_s(highscore[j].name, highscore[j - 1].name);
                highscore[j].score = highscore[j - 1].score;
            }
            strcpy_s(highscore[i].name, _name);
            highscore[i].score = _score;
            break;
        }
    }
}

// Hiển thị top người chơi điểm cao
void showHighScore() {
    system("cls");
    gotoxy(105, 28);
    cout << "esc = return";
    fstream f;
    f.open("highscore.txt", ios::in);
    string data;
    int i = 0;
    int y = 10;

    while (!f.eof()) {
        f >> data;
        i++;
        if (i == 13)
            break;
        if (i == 1) {
            gotoxy(60, y);
            SetColor(6);
            cout << data;
            continue;
        }
        if (i == 2) {
            gotoxy(65, y);
            SetColor(6);
            cout << data;
            y++;
            continue;
        }
        if (i % 2 == 1) {
            gotoxy(50, y);
            SetColor(9);
            cout << data;
            continue;
        }
        if (i % 2 == 0) {
            gotoxy(75, y);
            SetColor(9);
            cout << data;
            y++;
            continue;
        }
    }

    f.close();

    char t = _getch();
    while (t != 27)
        t = _getch();
    if (t == 27)
        ChooseOptions();
}

void loadHighScore() {
    fstream f;
    f.open("highscore.txt", ios::out);
    f << "HIGH SCORE\n";
    for (int i = 0; i < 5; i++) {
        f << highscore[i].name;
        f << "\t\t";
        f << highscore[i].score;
        f << "\n";
    }
    f.close();
}

void ChooseOptions() {
    ShowCur(false);
    system("cls");
    gotoxy(47, 13);
    SetColor(4);
    cout << ">> START GAME <<";
    gotoxy(50, 14);
    SetColor(15);
    cout << "HIGH SCORE";
    gotoxy(50, 15);
    SetColor(15);
    cout << "QUIT GAME";

    char pse;
    int Option = 1;
    pse = _getch();

    while (pse != 13) {
        if (pse == 's') {
            Option++;
            if (Option > MAX_OPTION)
                Option = 1;
            if (Option == 1) {
                gotoxy(47, 15);
                cout << "               ";
                gotoxy(47, 13);
                SetColor(4);
                cout << ">> START GAME <<";
                gotoxy(50, 14);
                SetColor(15);
                cout << "HIGH SCORE";
                gotoxy(50, 15);
                SetColor(15);
                cout << "QUIT GAME";
            }
            else if (Option == 2) {
                gotoxy(47, 13);
                cout << "                ";
                gotoxy(50, 13);
                SetColor(15);
                cout << "START GAME";
                gotoxy(47, 14);
                SetColor(4);
                cout << ">> HIGH SCORE <<";
                gotoxy(50, 15);
                SetColor(15);
                cout << "QUIT GAME";
            }
            else if (Option == 3) {
                gotoxy(47, 14);
                cout << "                ";
                gotoxy(50, 13);
                SetColor(15);
                cout << "START GAME";
                gotoxy(50, 14);
                SetColor(15);
                cout << "HIGH SCORE";
                gotoxy(47, 15);
                SetColor(4);
                cout << ">> QUIT GAME <<";
            }
        }
        else {
            if (pse == 'w') {
                Option--;
                if (Option < 1)
                    Option = MAX_OPTION;
                if (Option == 1) {
                    gotoxy(47, 14);
                    cout << "                ";
                    gotoxy(47, 13);
                    SetColor(4);
                    cout << ">> START GAME <<";
                    gotoxy(50, 14);
                    SetColor(15);
                    cout << "HIGH SCORE";
                    gotoxy(50, 15);
                    SetColor(15);
                    cout << "QUIT GAME";
                }
                else if (Option == 2) {
                    gotoxy(47, 15);
                    cout << "               ";
                    gotoxy(50, 13);
                    SetColor(15);
                    cout << "START GAME";
                    gotoxy(47, 14);
                    SetColor(4);
                    cout << ">> HIGH SCORE <<";
                    gotoxy(50, 15);
                    SetColor(15);
                    cout << "QUIT GAME";
                }
                else if (Option == 3) {
                    gotoxy(47, 13);
                    cout << "                ";
                    gotoxy(50, 13);
                    SetColor(15);
                    cout << "START GAME";
                    gotoxy(50, 14);
                    SetColor(15);
                    cout << "HIGH SCORE";
                    gotoxy(47, 15);
                    SetColor(4);
                    cout << ">> QUIT GAME <<";
                }
            }
        }
        pse = _getch();
    }
    if (Option == 1) {
        //PlaySound(TEXT("SNAKE_start.wav"), NULL, SND_ASYNC);
        StartGame();
    }
    else if (Option == 2) {
        showHighScore();
        return;
    }
    else if (Option == 3) {
        system("cls");
        return;
    }
}

// Bắt đầu Game
void StartGame() {
    system("cls");
    cout << "Choose your level (1 - 5): ";
    int _HardLevel;
    cin >> _HardLevel;
    int speed = 600 - _HardLevel * 100; // Calculate speed depend on level
    system("cls");
    PlaySound(TEXT("SNAKE_start.wav"), NULL, SND_ASYNC);
    Diem = 0;
    MoiAn = 0;
    CONRAN r;
    int Huong = 0;
    char t;

    // Vẽ khung
    SetColor(13);
    for (int i = 0; i < FRAME_WIDTH; i++) {
        gotoxy(i, 0);
        cout << "#";
        gotoxy(i, FRAME_HEIGHT - 1);
        cout << "#";
    }
    for (int i = 1; i < FRAME_HEIGHT; i++) {
        gotoxy(0, i);
        cout << "#";
        gotoxy(FRAME_WIDTH - 1, i);
        cout << "#";
    }

    char pse;
    int pc = 0;
    while (1) {
        if (_kbhit()) {
            t = _getch();
            //PlaySound(TEXT("SNAKE_beep.wav"), NULL, SND_ASYNC);
            if (t == 'a') Huong = 2;
            else if (t == 'w') Huong = 3;
            else if (t == 'd') Huong = 0;
            else if (t == 's') Huong = 1;
            else if (t == ' ')
            {
                gotoxy(FRAME_WIDTH / 2 - 10, FRAME_HEIGHT / 2);
                cout << "press SPACE to continue";
                pse = _getch();
                while (pse != ' ' && pse != 27)
                    pse = _getch();
                if (pse == ' ')
                {
                    gotoxy(FRAME_WIDTH / 2 - 10, FRAME_HEIGHT / 2);
                    cout << "                       ";
                }
                else {
                    if (pse == 27)
                    {
                        system("cls");
                        gotoxy(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);
                        cout << "END GAME";
                        Sleep(1000);
                        system("cls");
                        break;
                    }
                }
            }
            else if (t == 27)
            {
                gotoxy(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);
                cout << "END GAME";
                Sleep(1000);
                system("cls");
                break;
            }
        }
        r.Ve();
        r.anmoi(_HardLevel);
        if (r.EndGame())
        {
            system("cls");
            //gotoxy(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);
            gotoxy(50, 12);
            cout << "Your score: " << Diem << "\n";
            char _name[50];
            gotoxy(35, 13);
            cout << "Ten cua ban(khong nhap dau cach): ";
            cin >> _name;
            checkHighScore(_name, Diem);
            Sleep(500);
            system("cls");
            SetColor(6);
            gotoxy(47, 12);
            cout << "Ban co muon choi lai?";
            char Ops;
            int process = 1;
            gotoxy(47, 13);
            SetColor(4);
            cout << ">> Co <<";
            gotoxy(50, 14);
            SetColor(15);
            cout << "Khong";

            Ops = _getch();
            while (Ops != 13) {
                if (Ops == 'w') {
                    if (process == 1) {
                        process = 2;
                        gotoxy(47, 13);
                        cout << "        ";
                        gotoxy(50, 13);
                        SetColor(15);
                        cout << "Co";
                        gotoxy(47, 14);
                        SetColor(4);
                        cout << ">> Khong <<";
                    }
                    else {
                        gotoxy(47, 14);
                        cout << "           ";
                        gotoxy(47, 13);
                        SetColor(4);
                        cout << ">> Co <<";
                        gotoxy(50, 14);
                        SetColor(15);
                        cout << "Khong";
                        process = 1;
                    }
                }
                else {
                    if (Ops == 's') {
                        if (process == 1) {
                            process = 2;
                            gotoxy(47, 13);
                            cout << "        ";
                            gotoxy(50, 13);
                            SetColor(15);
                            cout << "Co";
                            gotoxy(47, 14);
                            SetColor(4);
                            cout << ">> Khong <<";
                        }
                        else {
                            process = 1;
                            gotoxy(47, 14);
                            cout << "           ";
                            gotoxy(47, 13);
                            SetColor(4);
                            cout << ">> Co <<";
                            gotoxy(50, 14);
                            SetColor(15);
                            cout << "Khong";
                        }
                    }
                }
                Ops = _getch();
            }

            if (Ops == 13) {
                if (process == 1) {
                    //PlaySound(TEXT("SNAKE_start.wav"), NULL, SND_ASYNC);
                    StartGame();
                }
                else {
                    loadHighScore();
                    ChooseOptions();
                }
            }

            break;
        }
        r.DiChuyen(Huong);
        Sleep(speed);
    }
}

// Đưa con trỏ đến vị trí column line
void gotoxy(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}

// whereX, whereY trả về vị trí hiện tại của con trỏ
int whereX() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.X;
    return -1;
}

int whereY() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.Y;
    return -1;
}

// Đổi màu chữ trên màn hình console
void SetColor(WORD color) {
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// Ẩn con trỏ trên màn hình console
void ShowCur(bool CursorVisibility) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
    SetConsoleCursorInfo(handle, &cursor);
}