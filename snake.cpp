#include <iostream>
#include <list>
#include "kb_helper.h"
using namespace std;
class Snake { public:
    enum Face { EAST = 1, NORTH = 2, WEST = 3, SOUTH = 4 };
    int max_x, max_y, is_alive;
    Face face;
    list<int> cells;
    Snake() : max_x(15), max_y(10), is_alive(1) {}
    Snake(int max_x, int max_y) : max_x(max_x), max_y(max_y), is_alive(1) {}
    int having(int a_cell) {
        return this->cells.end() != find(cells.begin(), cells.end(), a_cell);
    }
    void set_face(Face new_face) {
        if (2 != abs(new_face - face)) { face = new_face; }
    }
    int head() { return this->cells.back(); }
    int next_head() {
        int x = this->head() % max_x, y = this->head() / max_x;
             if (EAST  == face) { x =  max_x <= x+1 ? 0 : x+1; }
        else if (WEST  == face) { x =  x <= 0 ? max_x-1 : x-1; }
        else if (NORTH == face) { y =  y <= 0 ? max_y-1 : y-1; }
        else if (SOUTH == face) { y =  max_y <= y+1 ? 0 : y+1; }
        return y * max_x + x;
    }
    int move_and_check(int food_cell) {
        int next = next_head();
        if (having(next)) { is_alive = 0; }
        else {
            cells.push_back(next);
            if (food_cell != next) { cells.pop_front(); }
            else { return 1; }
        }
        return 0;
    }
};
class SnakeLogic { public:
    list<int> field, blanks;
    Snake snake;
    int score, food_cell, max_delay_ms;
    SnakeLogic(Snake s): snake(s), score(0), max_delay_ms(500) {
        int shape = snake.max_x * snake.max_y;
        do { field.push_back(shape); } while (shape-- > 0);
    }
    void make_new_food() {
        blanks.clear();
        int i = 0;
        do { if (!snake.having(i)) { blanks.push_back(i); } }
        while (i++ < field.size());
        list<int>::iterator it = blanks.begin();
        advance(it, rand() % blanks.size());
        food_cell = *(it);
    }
    void get_input() {
        unsigned int waited_MS = 0, input_wait_MS = 10;
        do {
            if (__kbhit()) {
                switch (__getch()) {
                    case 'a': case 'A': snake.set_face(Snake::WEST);  break;
                    case 's': case 'S': snake.set_face(Snake::SOUTH); break;
                    case 'd': case 'D': snake.set_face(Snake::EAST);  break;
                    case 'w': case 'W': snake.set_face(Snake::NORTH); break;
                    case 'X': snake.is_alive = 0; break;
                    default: break;
                }
            }
            delay_ms(input_wait_MS);
            waited_MS += input_wait_MS;
        } while (waited_MS < max_delay_ms);
    }
    void update_data() {
        if (snake.move_and_check(food_cell)) {
            make_new_food();
            score++;
            if (max_delay_ms > 50) { max_delay_ms -= 5; }
        }
    }
    void re_draw() {
        clear_src();
        cout << "SNAKE CPP: a-left, s-down, d-right, w-up, X-exit" << endl;
        int field_cell = 0, width = snake.max_x;
        do {
            if (field_cell % width == 0) { cout << " ] "; }
            cout << ( field_cell == food_cell    ? " @ " :
                      snake.head() == field_cell ? " o " :
                      snake.having(field_cell)   ? " x " : " - " );
            if (++field_cell % width == 0) { cout << "[ " << endl; }
        } while (field_cell < field.size());
        cout << endl << "SCORE: " << score << endl;
    }
    void loop() {
        while (snake.is_alive) { get_input(); update_data(); re_draw(); }
    }
    void setup(int delay_ms) {
        srand( (unsigned int)time(0) );
        max_delay_ms = delay_ms;
        make_new_food();
    }
};

int main(int argc, const char * argv[]) {
    Snake a_snake(15,10);
    a_snake.face = Snake::EAST;
    a_snake.cells = {0, 1, 2};
    
    SnakeLogic game(a_snake);
    game.setup(100);
    game.loop();
    cout << "GAME OVER" << endl;
    return 0;
}
