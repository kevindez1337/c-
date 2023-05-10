# c-
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Определяем константы
const int FIELD_SIZE = 10;
const int BOMB_COUNT = 10;

// Структура для хранения информации о клетке
struct Cell {
    bool bomb = false; // наличие бомбы
    bool opened = false; // клетка открыта
    int nearby_bombs = 0; // количество бомб рядом
};

// Определяем тип игрового поля
typedef vector<vector<Cell>> Field;

// Функция для генерации случайного числа
int rand_int(int from, int to) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(from, to);
    return dist(gen);
}

// Функция для инициализации игрового поля
void init(Field& field) {
    // Создаем пустое поле
    field.resize(FIELD_SIZE, vector<Cell>(FIELD_SIZE));

    // Размещаем бомбы на поле
    int bombs = 0;
    while (bombs < BOMB_COUNT) {
        int x = rand_int(0, FIELD_SIZE - 1);
        int y = rand_int(0, FIELD_SIZE - 1);
        if (!field[x][y].bomb) {
            field[x][y].bomb = true;
            bombs++;
        }
    }

    // Рассчитываем количество бомб рядом с каждой клеткой
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (field[i][j].bomb) {
                field[i][j].nearby_bombs = -1;
            } else {
                int count = 0;
                for (int x = max(i - 1, 0); x <= min(i + 1, FIELD_SIZE - 1); x++) {
                    for (int y = max(j - 1, 0); y <= min(j + 1, FIELD_SIZE - 1); y++) {
                        if (field[x][y].bomb) {
                            count++;
                        }
                    }
                }
                field[i][j].nearby_bombs = count;
            }
        }
    }
}

// Функция для открытия клетки
void open_cell(Field& field, int x, int y) {
    if (x < 0 || x >= FIELD_SIZE || y < 0 || y >= FIELD_SIZE || field[x][y].opened) {
        return;
    }
    field[x][y].opened = true;
    if (field[x][y].bomb) {
        cout << "Game over!" << endl;
        exit(0);
    } else if (field[x][y].nearby_bombs == 0) {
        for (int i = max(x - 1, 0); i <= min(x + 1, FIELD_SIZE - 1); i++) {
            for (int j = max(y - 1, 0); j <= min(y + 1, FIELD_SIZE - 1); j++) {
                open_cell(field, i, j);
            }
        }
    }
}

// Функция для проверки, закончена ли игра
bool is_game_finished(const Field& field)
