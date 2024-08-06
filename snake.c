#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SNAKE_SIZE 10
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int dx, dy;
} Snake;

Snake snake;
Point food;
int score = 0;

void initGame() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    // Initialize random seed
    srand(time(NULL));

    // Initialize snake
    snake.length = 5;
    snake.dx = SNAKE_SIZE;
    snake.dy = 0;
    for (int i = 0; i < snake.length; ++i) {
        snake.body[i].x = 100 - i * SNAKE_SIZE;
        snake.body[i].y = 100;
    }

    // Initialize food
    food.x = (rand() % (SCREEN_WIDTH / SNAKE_SIZE)) * SNAKE_SIZE;
    food.y = (rand() % (SCREEN_HEIGHT / SNAKE_SIZE)) * SNAKE_SIZE;
}

void handleInput() {
    if (kbhit()) {
        char ch = getch();
        switch (ch) {
            case 'w': if (snake.dy == 0) { snake.dx = 0; snake.dy = -SNAKE_SIZE; } break;
            case 's': if (snake.dy == 0) { snake.dx = 0; snake.dy = SNAKE_SIZE; } break;
            case 'a': if (snake.dx == 0) { snake.dx = -SNAKE_SIZE; snake.dy = 0; } break;
            case 'd': if (snake.dx == 0) { snake.dx = SNAKE_SIZE; snake.dy = 0; } break;
        }
    }
}

void updateGame() {
    // Move the snake
    for (int i = snake.length - 1; i > 0; --i) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0].x += snake.dx;
    snake.body[0].y += snake.dy;

    // Check for collision with walls
    if (snake.body[0].x < 0 || snake.body[0].x >= SCREEN_WIDTH ||
        snake.body[0].y < 0 || snake.body[0].y >= SCREEN_HEIGHT) {
        printf("Game Over! Your score: %d\n", score);
        getch();
        exit(0);
    }

    // Check for collision with itself
    for (int i = 1; i < snake.length; ++i) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            printf("Game Over! Your score: %d\n", score);
            getch();
            exit(0);
        }
    }

    // Check for collision with food
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        score++;
        snake.length++;
        food.x = (rand() % (SCREEN_WIDTH / SNAKE_SIZE)) * SNAKE_SIZE;
        food.y = (rand() % (SCREEN_HEIGHT / SNAKE_SIZE)) * SNAKE_SIZE;
    }
}

void renderGame() {
    cleardevice();

    // Draw snake
    for (int i = 0; i < snake.length; ++i) {
        setfillstyle(SOLID_FILL, GREEN); // Snake color
        setcolor(GREEN);
        bar(snake.body[i].x, snake.body[i].y, snake.body[i].x + SNAKE_SIZE, snake.body[i].y + SNAKE_SIZE);
    }

    // Draw food
    setfillstyle(SOLID_FILL, RED); // Food color
    setcolor(RED);
    bar(food.x, food.y, food.x + SNAKE_SIZE, food.y + SNAKE_SIZE);

    // Render score
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    setcolor(WHITE);
    outtextxy(10, 10, scoreText);

    delay(100);
}

int main() {
    initGame();

    while (1) {
        handleInput();
        updateGame();
        renderGame();
    }

    closegraph();
    return 0;
}
