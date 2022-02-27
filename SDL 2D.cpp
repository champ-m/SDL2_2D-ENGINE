#include"Engine.h"
int main(int argc, char* args[])
{
	Engine Engine2D;
	Engine2D.Init_2D();
	Engine2D.LoadMedia("data/map.jpg");
	Engine2D.LoadMedia("data/player1.png");
	Engine2D.LoadMedia("data/player2.png"); 
	Engine2D.LoadMedia("data/scoreboard1.jpg");
	Engine2D.Create_Rect(); // one time for player 1 score rect.
	Engine2D.Create_Rect(); // one time for player 2 score rect.
	Engine2D.Create_Rect(); // for Timer 
	bool Quit = false;
	bool pause = false;
	bool restart = false;
	Engine2D.Start_Game_Time();
	while (!Quit)
	{
		while (SDL_PollEvent(&Engine2D.Event) != 0)
		{
			switch (Engine2D.Event.type) {
			case SDL_QUIT:
				Quit = true;
				break;
			}
		}
		Engine2D.Keyboard_Shortcut(&restart, &pause);
		Engine2D.DoNotPauseFunctions();						//Function which should keep running even when the game is paused.
		if (!pause) {
			Engine2D.Check_For_Keyboard_State();
			Engine2D.run_2D_engine();
			Engine2D.Clear_Renderer();
			//Engine2D.Print_Coordinate();
		}
	}

	Engine2D.Close();

	return 0;
}