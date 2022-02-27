#include "Engine.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Time::Time() : Start_Time(0), Time_Allowed(21000), Time_Left(0), Current_TicksTime(0), Paused_Time(0)
{
}
Time::~Time()
{
}
void Time::StartTimer()
{
	Start_Time = SDL_GetTicks();
	Current_TicksTime = Start_Time;
}
void Time::Update_TickTime()
{
	Current_TicksTime = SDL_GetTicks();
}
uint32_t Time::Rem_Time()
{
	Current_TicksTime = SDL_GetTicks();
	Time_Left = Time_Allowed - (Current_TicksTime - Start_Time);

	if (Time_Allowed < Current_TicksTime - Start_Time) {					// Will keep the time to over flow below the Start_Time
		Restart_Timer();
	}

	return Time_Left;
}
void Time::Restart_Timer()
{
	Start_Time = Current_TicksTime;
	Time_Left = 0;
}
void Time::Keep_Timer_Steady()
{
	Paused_Time = Current_TicksTime;
	Update_TickTime();
	Start_Time += Current_TicksTime - Paused_Time;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//int Engine::Textures_loaded = -1;
//int Engine::Rect_created = 0;

Engine::Engine() : Window_2D(NULL), Renderer_2D(NULL), Texture_2D(NULL), Event(), WINDOW_WIDTH(1600), WINDOW_HEIGHT(1000), Scoreboard_Height(160), Current_Key_State(0), sensitivity(10), wRenderer(0), hRenderer(0), P1_Left(0), P1_Right(0), P1_Top(0), P1_Bottom(0), P2_Left(0), P2_Right(0), P2_Top(0), P2_Bottom(0), Score_P1(0), Score_P2(0), Textures_loaded(-1), Rect_created(0),  Score_flag(false), Font(NULL), margin(20), Surface_Text(NULL), T_Rect({ 0,0,0,0 }), Text_Color({ 175,175,175,0xff }), T_pair({ 0,0,0,0 }), GamePaused(false), T_Time(0)
{
}

Engine::~Engine()
{
}

void Engine::Start_Game_Time()
{
	StartTimer();
}

void Engine::Starting_Script()
{
	
}

void Engine::Print_Score() 
{
	std::cout << "Player 1 Score = " << Score_P1 << '&' << "Player 2 Score = " << Score_P2 << std::endl;
}

void Engine::Print_Coordinate()
{
	std::cout << "Player 1 (xCO = " << Rect_2D_Target[PLAYER1].x << " , yCO = " << Rect_2D_Target[PLAYER1].y << " , ) & Player 2 (xCO = " << Rect_2D_Target[PLAYER2].x << " , yCO = " << Rect_2D_Target[PLAYER2].y << ")"<<std::endl;
}

bool Engine::Init_2D()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL couldn't be initiallized. Error : " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		if (TTF_Init() == -1) {
			std::cout << "SDL Fonts can't be initialized. Error " << TTF_GetError() << std::endl;
			success = false;
		}
		if (!IMG_Init(IMG_INIT_JPG)) {
			std::cout << "SDL Image couldn't be initiallized. Error : " << IMG_GetError() << std::endl;
			success = false;
		}
		else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2")) {
				std::cout << "Warning! Texture filtering is off..." << std::endl;
			}

			Window_2D = SDL_CreateWindow("2D_Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT,SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
			if (Window_2D == NULL) {
				std::cout << "Window couldn't be created. Error : " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				Renderer_2D = SDL_CreateRenderer(Window_2D, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (Renderer_2D == NULL) {
					std::cout << "Renderer couldn't be created. Error : " << SDL_GetError() << std::endl;
					success = false;
				}
				else {
					SDL_SetRenderDrawColor(Renderer_2D, 150, 150, 150, SDL_ALPHA_OPAQUE);
					
					Font = TTF_OpenFont("data/SevenSegment.ttf", 50);
					if (Font == NULL) {
						std::cout << "Font cannot be loaded. Error " << TTF_GetError() << std::endl;
						success = false;
					}
				}
			}
		}
	}
	return success;
}

// This function will create room for the SDL_rect and texture for Score P1 & P2
bool Engine::Create_Rect()
{
	bool success = true;
	
	if (Surface_Text != NULL) {
		SDL_FreeSurface(Surface_Text);
		Surface_Text = NULL;
	}
	Surface_Text = TTF_RenderText_Solid(Font, "Sample Text", Text_Color);
	if (Surface_Text == NULL) {
		std::cout << "Surface connot be created from Text. Error " << TTF_GetError() << std::endl;
		success = false;
	}
	else {
		Textures_loaded++;
		Texture_2D.push_back(SDL_CreateTextureFromSurface(Renderer_2D, Surface_Text));
		if (Texture_2D[Textures_loaded] == NULL) {
			std::cout << "Texture couldn't be created from surface_text. Error : " << SDL_GetError() << std::endl;
			Textures_loaded--;
			success = false;
		}
		else {
			if (Textures_loaded == SCORE_FOR_P1) {
				pos_pair.push_back(T_pair);
				Create_Target_Rect(&pos_pair[SCORE_FOR_P1]);
			}
			else if (Textures_loaded == SCORE_FOR_P2) {
				pos_pair.push_back(T_pair);
				Create_Target_Rect(&pos_pair[SCORE_FOR_P2]);
			}
			else if (Textures_loaded == TIMER) {
				pos_pair.push_back(T_pair);
				Create_Target_Rect(&pos_pair[TIMER]);
			}
		}
	}
	SDL_FreeSurface(Surface_Text);
	Surface_Text = NULL;
	return success;
}

bool Engine::Update_Text_Rect(std::string Text, LOADMEDIA Flag)
{
	if (Surface_Text != NULL) {
		SDL_FreeSurface(Surface_Text);
		Surface_Text = NULL;
	}
	if (Texture_2D[Flag] != NULL) {
		SDL_DestroyTexture(Texture_2D[Flag]);
		Texture_2D[Flag] = NULL;
	}
	Surface_Text = TTF_RenderText_Solid(Font, Text.c_str(), Text_Color);
	if (Surface_Text == NULL) {
		std::cout << "Surface connot be created from Text. Error " << TTF_GetError() << std::endl;
		return false;
	}
	else {
		Texture_2D[Flag] = SDL_CreateTextureFromSurface(Renderer_2D, Surface_Text);
		if (Texture_2D[Flag] == NULL) {
			std::cout << "Texture couldn't be created from surface_text. Error : " << SDL_GetError() << std::endl;
			return false;
		}
		else {
			Update_Rect_Size(Flag);
		}
	}
	return true;
}

void Engine::Update_Rect_Size(LOADMEDIA Flag)
{
	if (Flag == SCORE_FOR_P1) {
		T_pair.x = pos_pair[SCOREBOARD].x + margin;
		T_pair.y = pos_pair[SCOREBOARD].y + margin;
		T_pair.w = Surface_Text->w;
		T_pair.h = Surface_Text->h;


		pos_pair[Flag] = T_pair;

	}
	else if (Flag == SCORE_FOR_P2) {
		T_pair.x = pos_pair[SCOREBOARD].x + margin;
		T_pair.y = pos_pair[SCOREBOARD].y + pos_pair[SCORE_FOR_P1].h + margin+10;
		T_pair.w = Surface_Text->w;
		T_pair.h = Surface_Text->h;

		pos_pair[Flag] = T_pair;

	}
	else if (Flag == TIMER) {
		T_pair.x = WINDOW_WIDTH - (400 + margin);
		T_pair.y = pos_pair[SCOREBOARD].y + margin*2;
		T_pair.w = Surface_Text->w;
		T_pair.h = Surface_Text->h;
		
		pos_pair[Flag] = T_pair;
	}

	T_Rect.x = pos_pair[Flag].x;
	T_Rect.y = pos_pair[Flag].y;
	T_Rect.w = pos_pair[Flag].w;
	T_Rect.h = pos_pair[Flag].h;

	Rect_2D_Target[Flag] = T_Rect;
}

bool Engine::LoadMedia(std::string path)
{
	bool success = true;

	SDL_Surface* Surface_2D = NULL;
	Surface_2D = IMG_Load(path.c_str());

	if (Surface_2D == NULL) {
		std::cout << "Image could not be loaded. Error " << IMG_GetError() << std::endl;
		success = false;
	}
	else {
		Texture_2D.push_back(SDL_CreateTextureFromSurface(Renderer_2D, Surface_2D));
		Textures_loaded++;
		if (Texture_2D[Textures_loaded] == NULL) {
			std::cout << "Texture could not be created. Error " << SDL_GetError() << std::endl;
			Textures_loaded--;
			success = false;
		}
		else {
			if (Textures_loaded == PLAYER2) {
				pos_pair.push_back(PAIR({ WINDOW_WIDTH- Surface_2D->w, (WINDOW_HEIGHT-Scoreboard_Height) - Surface_2D->h, Surface_2D->w, Surface_2D->h }));
				Create_Target_Rect(&pos_pair[PLAYER2]);
				}
			else if(Textures_loaded == PLAYER1){
				pos_pair.push_back(PAIR({ 0, 0, Surface_2D->w, Surface_2D->h }));
				Create_Target_Rect(&pos_pair[PLAYER1]);
			}
			else if (Textures_loaded == MAP) {
				pos_pair.push_back(PAIR({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - Scoreboard_Height }));
				Create_Target_Rect(&pos_pair[MAP]);
			}
			else if (Textures_loaded == SCOREBOARD) {
				pos_pair.push_back(PAIR({ 0, WINDOW_HEIGHT - Scoreboard_Height + (margin / 5), WINDOW_WIDTH, WINDOW_HEIGHT - pos_pair[MAP].h }));
				Create_Target_Rect(&pos_pair[SCOREBOARD]);
			}
		}
	}
	SDL_FreeSurface(Surface_2D);
	Surface_2D = NULL;
	return success;
}

void Engine::Create_Target_Rect(PAIR *position)
{
	T_Rect.x = position->x;
	T_Rect.y = position->y;
	T_Rect.w = position->w;
	T_Rect.h = position->h;

	Rect_2D_Target.push_back(T_Rect);
	Rect_created++;
}

void Engine::Reset_Player_Positions()
{
	Rect_2D_Target[PLAYER1].x = 0;
	Rect_2D_Target[PLAYER1].y = 0;
	Rect_2D_Target[PLAYER2].x = WINDOW_WIDTH - pos_pair[PLAYER2].w;
	Rect_2D_Target[PLAYER2].y = WINDOW_HEIGHT- Scoreboard_Height - pos_pair[PLAYER2].h;

}

bool Engine::CheckCollision()
{
	P1_Left = Rect_2D_Target[PLAYER1].x;
	P1_Right = Rect_2D_Target[PLAYER1].x + Rect_2D_Target[PLAYER1].w;
	P1_Top = Rect_2D_Target[PLAYER1].y;
	P1_Bottom = Rect_2D_Target[PLAYER1].y + Rect_2D_Target[PLAYER1].h;

	P2_Left = Rect_2D_Target[PLAYER2].x;
	P2_Right = Rect_2D_Target[PLAYER2].x + Rect_2D_Target[PLAYER2].w;
	P2_Top = Rect_2D_Target[PLAYER2].y;
	P2_Bottom = Rect_2D_Target[PLAYER2].y + Rect_2D_Target[PLAYER2].h;

	if (P1_Bottom < P2_Top )
		return false;
	if (P1_Top > P2_Bottom)
		return false;
	if (P1_Left > P2_Right)
		return false;
	if (P1_Right < P2_Left)
		return false;
	
	//std::cout << "Collided" << std::endl;

	return true;
}

void Engine::Update_Rem_Time()
{
	T_Time = Rem_Time();

	if (T_Time/50<= 0) {
		Restart_Timer();
		if (!Engine::Score_flag) {
			Score_P2++;
			Reset_Player_Positions();
		}
		else if (Score_flag) {
			Score_P1++;
			Reset_Player_Positions();
		}
	}
}

void Engine::DoNotPauseFunctions()
{
	if (GamePaused) {
		Keep_Timer_Steady();
	}
}

void Engine::run_2D_engine()
{
	SDL_RenderClear(Renderer_2D);

	SDL_SetRenderDrawColor(Renderer_2D, 0x00, 0x00, 0x00, 0xff);


	Update_Rem_Time();


	Update_Text_Rect("Score P 1 : " + std::to_string(Score_P1), SCORE_FOR_P1);
	Update_Text_Rect("Score P2 : " + std::to_string(Score_P2), SCORE_FOR_P2);
	Update_Text_Rect("Time Left : " + std::to_string(T_Time/1000 ), TIMER);

	SDL_RenderCopy(Renderer_2D, Texture_2D[MAP], NULL, &Rect_2D_Target[MAP] );						// render map
	SDL_RenderCopy(Renderer_2D, Texture_2D[PLAYER1], NULL, &Rect_2D_Target[PLAYER1]);				//render player
	SDL_RenderCopy(Renderer_2D, Texture_2D[PLAYER2], NULL, &Rect_2D_Target[PLAYER2]);				//render player
	SDL_RenderCopy(Renderer_2D, Texture_2D[SCOREBOARD], NULL, &Rect_2D_Target[SCOREBOARD]);			// render scoreboard
	SDL_RenderCopy(Renderer_2D, Texture_2D[SCORE_FOR_P1], NULL, &Rect_2D_Target[SCORE_FOR_P1]);		// render Score for P1
	SDL_RenderCopy(Renderer_2D, Texture_2D[SCORE_FOR_P2], NULL, &Rect_2D_Target[SCORE_FOR_P2]);		// render Score for P2
	SDL_RenderCopy(Renderer_2D, Texture_2D[TIMER], NULL, &Rect_2D_Target[TIMER]);					// render Timer Clock

	SDL_RenderPresent(Renderer_2D);

}

void Engine::Keyboard_Shortcut(bool* restart, bool *Pause)
{
	Current_Key_State = SDL_GetKeyboardState(NULL);
	
	if (Current_Key_State[SDL_SCANCODE_R])
	{
		Restart();
	}

	if (Current_Key_State[SDL_SCANCODE_P] && GamePaused == false) {
		*Pause = true;
		GamePaused = true;
		Sleep(150);
	}
	else if (Current_Key_State[SDL_SCANCODE_P] && GamePaused == true) {
		*Pause = false;
		GamePaused = false;
		Sleep(150);
	}
}

void Engine::Check_For_Keyboard_State()
{
	Current_Key_State = SDL_GetKeyboardState(NULL);
	
	if (CheckCollision()) {
		if (!Score_flag) {
			Restart_Timer();
			Reset_Player_Positions();
			Score_flag = true;
			Score_P1++;
		}
		else if (Score_flag) {
			Restart_Timer();
			Reset_Player_Positions();
			Score_P2++;
			Score_flag = false;
		}
		Print_Score();
	}
	if (Current_Key_State[SDL_SCANCODE_W] && Rect_2D_Target[PLAYER1].y > 0) {
		Rect_2D_Target[PLAYER1].y -= sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_A] && Rect_2D_Target[PLAYER1].x > 0) {
		Rect_2D_Target[PLAYER1].x -= sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_S] && (Rect_2D_Target[PLAYER1].y + Rect_2D_Target[PLAYER1].h) < Rect_2D_Target[MAP].h) {
		Rect_2D_Target[PLAYER1].y += sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_D] && (Rect_2D_Target[PLAYER1].x + Rect_2D_Target[PLAYER1].w) < WINDOW_WIDTH) {
		Rect_2D_Target[PLAYER1].x += sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_UP] && Rect_2D_Target[PLAYER2].y > 0) {
		Rect_2D_Target[PLAYER2].y -= sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_LEFT] && Rect_2D_Target[PLAYER2].x > 0) {
		Rect_2D_Target[PLAYER2].x -= sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_DOWN] && (Rect_2D_Target[PLAYER2].y + Rect_2D_Target[PLAYER2].h) < (Rect_2D_Target[MAP].h)) {
		Rect_2D_Target[PLAYER2].y += sensitivity;
	}
	if (Current_Key_State[SDL_SCANCODE_RIGHT] && (Rect_2D_Target[PLAYER2].x + Rect_2D_Target[PLAYER2].w) < WINDOW_WIDTH) {
		Rect_2D_Target[PLAYER2].x += sensitivity;
	}
}

void Engine::Clear_Renderer()
{
	SDL_RenderClear(Renderer_2D);
}

void::Engine::Restart()
{
	Restart_Timer();
	Score_P1 = 0;
	Score_P2 = 0;
	Reset_Player_Positions();
}

void Engine::Close()
{
	TTF_CloseFont(Font);
	Font = NULL;
	for (int i = 0; i <(int)Texture_2D.size(); i++) {
		SDL_DestroyTexture(Texture_2D[i]);
		Texture_2D[i] = NULL;
	}
	Texture_2D.clear();
	pos_pair.clear();
	Current_Key_State = NULL;
	SDL_DestroyRenderer(Renderer_2D);
	Renderer_2D = NULL;
	SDL_DestroyWindow(Window_2D);
	Window_2D = NULL;
	IMG_Quit();
	SDL_Quit();
}
