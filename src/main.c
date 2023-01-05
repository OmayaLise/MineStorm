#include <raylib.h>
#include <../../src/game.h>
#include <../../src/mathLibrary.h>


int main(void)
{

    Game game = {0};

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib [core] example - basic window");
    SetTargetFPS(60);
    GameInit(&game);
   
    // Main game loop
    while (game.windowOpen)
    {
       UpdateAndDraw(&game);
    }
    GameUnload(&game);
    
    CloseWindow();

    return 0;
}