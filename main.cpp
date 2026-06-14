#include <iostream>
#include <format>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

static constexpr int window_height {800};
static constexpr int window_width  {800};
static constexpr int cell_size     {2 };
static constexpr int rows          {window_width  / cell_size};
static constexpr int cols          {window_height / cell_size};
static constexpr int num_cells     {rows * cols};

class Cell {
public:
    bool alive {false};

private:

};

std::vector<Cell> m_read {num_cells};
std::vector<Cell> m_write{num_cells};

Cell* grid_at(int x, int y, std::vector<Cell>& grid) {
    if (x >= (int)cols || y >= (int)rows) return nullptr;
    if (x < 0 || y < 0)                   return nullptr;

    return &grid[y * cols + x];
}

void draw_board() {
    for (size_t i {}; i < rows; ++i) {
        for (size_t j {}; j < cols; ++j) {
            Color color {BLACK};
            if (grid_at(j, i, m_read)->alive)
                color = WHITE;
            DrawRectangle(j * cell_size, i * cell_size, cell_size, cell_size, color);
        }
    }
}

int num_neighbours(int x, int y) {
    constexpr std::array<std::pair<int, int>, 8> directions {{
        {-1,  0},  // N
        { 1,  0},  // S
        { 0, -1},  // W
        { 0,  1},  // E
        {-1, -1},  // NW
        {-1,  1},  // NE
        { 1, -1},  // SW
        { 1,  1},  // SE
    }};

    int res {};
    for (const auto& dir : directions) {
        auto* cell = grid_at(x + dir.first, y + dir.second, m_read);
        if (cell && cell->alive) {
            res++;
        }
    }
    return res;
}

void tick() {
    for (size_t i {}; i < rows; ++i) {
        for (size_t j {}; j < cols; ++j) {

            int n {num_neighbours(j, i)}; 
            auto *read_cell = grid_at(j, i, m_read);
            auto *write_cell = grid_at(j, i, m_write);

            if (!read_cell || !write_cell) 
                continue;

            write_cell->alive = (n == 3) || (n == 2 && read_cell->alive);
        }
    }
    //swap the read and the write,
    //std::vectors pointers are swapped when using std::swap so it is O(1)
    std::swap(m_read, m_write);
}

void randomise_board() {
    srand(time(NULL));
    for (size_t i {}; i < rows; ++i) {
        for (size_t j {}; j < cols; ++j) {

            //This gives every cell a 20% chance to be alive at the begginning
            int chance = rand() % 10;
            if (chance <= 2) {
                auto* cell = grid_at(j, i, m_read);
                if (cell) cell->alive = true;
            }
        }
    }
}


int main() {
    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(1000);
    InitWindow(window_height, window_width, "Conway's Game of Life!");
    
    randomise_board();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board();
        tick();
        EndDrawing();
    }
    return 0;
}
