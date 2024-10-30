#include <raylib.h>
#include <time.h>

#define MAX_TAIL_LENGTH 144 

int cordgenerator() {
    int position = (rand() % ((600 + 1) / 50)) * 50;
    return position;
}

int main(void) {
    srand(time(0));

    bool gameOver = false;

    const int screenWidth = 600;
    const int screenHeight = 600;

    int posX = 300;
    int posY = 300;

    int fruitx = cordgenerator();
    int fruity = cordgenerator();

    InitWindow(screenWidth, screenHeight, "snake");

    InitAudioDevice();
    Music music = LoadMusicStream("background.mp3"); 
    PlayMusicStream(music); 

    const int cellSize = 50;
    const int numCols = screenWidth / cellSize;
    const int numRows = screenHeight / cellSize;

    Rectangle player = {posX, posY, 50, 50};
    Rectangle fruit = {fruitx, fruity, 50, 50};

    Rectangle playersec[MAX_TAIL_LENGTH]; 
    int tailLength = 0; 

    int direction = 0; 
    SetTargetFPS(10); 

    while (!WindowShouldClose() && !gameOver) {
         UpdateMusicStream(music);
        
        if (IsKeyPressed(KEY_RIGHT) && direction != 4) {
            direction = 3;
        }
        if (IsKeyPressed(KEY_UP) && direction != 2) {
            direction = 1;
        }
        if (IsKeyPressed(KEY_LEFT) && direction != 3) {
            direction = 4;
        }
        if (IsKeyPressed(KEY_DOWN) && direction != 1) {
            direction = 2;
        }

        
        Rectangle previousHead = player; 
        switch (direction) {
            case 1: posY -= 50; break; // Up
            case 2: posY += 50; break; // Down
            case 3: posX += 50; break; // Right
            case 4: posX -= 50; break; // Left
        }

       
        player.x = posX;
        player.y = posY;

        
        if (tailLength > 0) {
            for (int i = tailLength - 1; i > 0; i--) {
                playersec[i] = playersec[i - 1]; 
            }
            playersec[0] = previousHead; 
        }

        
        if (CheckCollisionRecs(player, fruit)) {
            if (tailLength < MAX_TAIL_LENGTH) {
                tailLength++; 
            }
            fruitx = cordgenerator();
            fruity = cordgenerator();
            fruit.x = fruitx;
            fruit.y = fruity;
        }

        if (posX > 600 || posY > 600 || posX < 0 || posY < 0){
            gameOver = true;
        }

        for (int i=0; i < tailLength; i++){
            if (CheckCollisionRecs(player, playersec[i]) == true){
                gameOver = true;
            }
        }

        
        BeginDrawing();
        ClearBackground(BLACK);

        
        for (int i = 0; i <= numCols; i++) {
            DrawLine(i * cellSize, 0, i * cellSize, screenHeight, WHITE);
        }
        for (int j = 0; j <= numRows; j++) {
            DrawLine(0, j * cellSize, screenWidth, j * cellSize, WHITE);
        }

        
        DrawRectangleRec(player, GREEN);
        for (int i = 0; i < tailLength; i++) {
            DrawRectangleRec(playersec[i], GREEN);
        }

        
        DrawRectangleRec(fruit, RED);

        EndDrawing();
    }
    
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
