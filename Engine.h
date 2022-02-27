#include<iostream>
#include<SDL.h>
#include<cstdint>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<vector>
#include<string>
#include<Windows.h>

struct PAIR {

	int x, y, w, h;
};

class Time
{
private:
	uint32_t Start_Time;
	uint32_t Time_Allowed;
	uint32_t Time_Left;
	uint32_t Current_TicksTime;
	uint32_t Paused_Time;
public:
	Time();
	~Time();
protected:
	void Update_TickTime();
	void StartTimer();
	uint32_t Rem_Time();
	void Restart_Timer();
	void Keep_Timer_Steady();
};

class Engine :protected Time
{
public:
	Engine();
	~Engine();
	bool Init_2D();
	bool LoadMedia(std::string);
	void run_2D_engine();
	void Restart();
	SDL_Event Event;
	void Check_For_Keyboard_State();
	void Clear_Renderer();
	void Close();
	bool CheckCollision();
	void Print_Coordinate();
	void Print_Score();
	bool Create_Rect();
	void Keyboard_Shortcut(bool*, bool*);
	void Start_Game_Time();
	void DoNotPauseFunctions();

private:
	uint32_t T_Time;
	void Update_Rem_Time();
	//void get_UpdatedClock();
	void Starting_Script();
	bool GamePaused;
	enum LOADMEDIA {
		MAP, PLAYER1, PLAYER2, SCOREBOARD, SCORE_FOR_P1, SCORE_FOR_P2, TIMER
	};
	PAIR T_pair;
	SDL_Color Text_Color;
	void Update_Rect_Size(LOADMEDIA);    // This will be called inside UPDATE_SCRE_RECT to update the size and pos_pair structure.
	bool Update_Text_Rect(std::string, LOADMEDIA);   // This function will be called inside Run_2D_Engine();
	uint8_t margin;
	TTF_Font* Font;
	uint16_t Score_P1, Score_P2;
	bool Score_flag;
	int Scoreboard_Height;
	void Reset_Player_Positions();
	uint16_t P1_Left, P1_Right, P1_Top, P1_Bottom, P2_Left, P2_Right, P2_Top, P2_Bottom;
	void Create_Target_Rect(PAIR*);
	int wRenderer, hRenderer;
	uint8_t sensitivity;
	const Uint8 *Current_Key_State;
	int Textures_loaded;
	int Rect_created;
	std::vector<PAIR> pos_pair; 
	uint16_t WINDOW_WIDTH;
	uint16_t WINDOW_HEIGHT;
	SDL_Rect T_Rect;
	SDL_Surface* Surface_Text;
	SDL_Window *Window_2D;
	SDL_Renderer *Renderer_2D;
	std::vector<SDL_Texture*> Texture_2D;
	std::vector<SDL_Rect> Rect_2D_Target;
};