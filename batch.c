#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Ball {
  Vector2 position;
  Vector2 velocity;
  float radius;
  float direction;
  Color color;
};

Vector2 getRandomVel() {
  float low = 0.2, high = 0.9;
  return (Vector2){low + (float)rand() / ((float)RAND_MAX / (high - low)) * cos(M_PI / 4),
   low + (float)rand() / ((float)RAND_MAX / (high - low)) * sin(M_PI / 4)};
}

// Function to add a new ball to the vector
void pushBall(struct Ball **balls, int *n, Vector2 pos, float radius) {
  *n = *n + 1; // Increment the number of balls
  *balls = realloc(*balls, *n * sizeof(struct Ball));
  int new_ball_index = *n - 1; // Index of the new ball
  (*balls)[new_ball_index].position = pos;
  (*balls)[new_ball_index].radius = radius;
  (*balls)[new_ball_index].velocity = getRandomVel(); //(Vector2){getRandomFloat() * cos(M_PI / 4), getRandomFloat() * sin(M_PI / 4)};
  (*balls)[new_ball_index].direction = (float)rand() / (float)RAND_MAX * 2.0f * M_PI;
  // Assign a random color to the new ball
  //(*balls)[new_ball_index].color = (Color){255, 0, 255, 255}; //rand() % 256, rand() % 256, rand() % 256, 255};
  (*balls)[new_ball_index].color = (Color){rand() % 256, rand() % 256, rand() % 256, 255};
}

void updateBallPosition(struct Ball *ball, int width, int height) {
  // Update x and y using sine and cosine
  ball->position.x += ball->velocity.x;
  ball->position.y += ball->velocity.y;

  // Reverse velocity if ball hits screen bounds
  if (ball->position.x <= 0 || ball->position.x >= width) {
    ball->velocity.x = -(ball->velocity.x);
  }
  if (ball->position.y <= 0 || ball->position.y >= height) {
    ball->velocity.y = -(ball->velocity.y);
  }
}

int main(void) {
  // Initialize the random seed
  srand(time(NULL));

  // Initialize window
  const int screenWidth = 680;
  const int screenHeight = 720;
  InitWindow(screenWidth, screenHeight, "Move Ball");

  RenderTexture2D buffer = LoadRenderTexture(screenWidth, screenHeight);

  float ballRadius = 20.0f;
  Vector2 ballSpawn = (Vector2){screenWidth/2, screenHeight/2};

  int n = 0; // Number of balls
  struct Ball *balls = malloc(n * sizeof(struct Ball));
  // Initialize the fields for the ball
  /*balls[0].position = ballSpawn;
  balls[0].radius = ballRadius;
  balls[0].velocity = (Vector2){1 * cos(M_PI / 4), 1 * sin(M_PI / 4)};
  balls[0].direction = (float)rand() / (float)RAND_MAX * 2.0f * M_PI;
  balls[0].color = (Color){rand() % 256, rand() % 256, rand() % 256, 255};*/

  /*for (int i = 0; i < 1000; i++) {
   pushBall(&balls, &n, ballSpawn, ballRadius);
  }*/

  // Timing variables
  int frameCounter = 0;
  int addBallCounter = 0;

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    frameCounter++;
    if (frameCounter >= GetFPS()) {
      frameCounter = 0;
      addBallCounter++;
      if (addBallCounter >= 1) { // Add ball every second
        addBallCounter = 0;
        pushBall(&balls, &n, ballSpawn, ballRadius);
      }
    }

    // Move the balls
    for (int i = 0; i < n; i++) {
      /*balls[i].position.x += cosf(balls[i].direction) * 0.3;
      balls[i].position.y += sinf(balls[i].direction) * 0.3;
      if(balls[i].position.x < 0) balls[i].direction = M_PI - balls[i].direction;
      if(balls[i].position.x > screenWidth) balls[i].direction = balls[i].direction - M_PI;
      if(balls[i].position.y < 0) balls[i].direction = balls[i].direction - M_PI;
      if(balls[i].position.y > screenHeight) balls[i].direction = balls[i].direction - M_PI;*/
      updateBallPosition(&balls[i], screenWidth, screenHeight);
    }
    // Draw
    BeginDrawing();
    BeginTextureMode(buffer);
    ClearBackground(RAYWHITE);
    for (int i = 0; i < n; i++) {
      DrawCircleV(balls[i].position, balls[i].radius, balls[i].color);
      DrawFPS(10, 10);
      char text[32];
      sprintf(text, "%d batched balls", n);
      DrawText(text, 10, 40, 40, BLACK);
    }
    EndTextureMode();
    //DrawTexture(buffer.texture, 0, 0, WHITE);
    //DrawTexturePro(buffer.texture, (Rectangle){0, 0, screenWidth, screenHeight}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(buffer.texture, (Rectangle){0, screenHeight, screenWidth, -screenHeight}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0, WHITE);

    EndDrawing();
  }

  // Deallocate the memory for the balls
  free(balls);
  UnloadRenderTexture(buffer);

  // Close window and OpenGL context
  CloseWindow();

  return 0;
}
