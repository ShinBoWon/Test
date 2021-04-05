#include "Play.h"

Play::Play()
{

}

void Play::Stage_Box() // 인터페이스로 
{
	MapDraw::GetInstance()->BoxErase(WIDTH, HEIGHT);
	string Stage = to_string(m_Player->Stage);
	MapDraw::GetInstance()->DrawMidText("★ " + Stage + " Stage  ★", WIDTH, HEIGHT * 0.2f);
	Sleep(ONE_SEC * 1.5f);
	MapDraw::GetInstance()->DrawMidText("                           ", WIDTH, HEIGHT * 0.2f);
}

void Play::Input_Data(string Name)
{
	Player *Player_Data;
	Player_Data = new Player;

	Player_Data->Name = Name;
	Player_Data->Score = 0;
	Player_Data->Stage = 1;
	Player_Data->Life = START_LIFE;

	m_Player = Player_Data;	 // 플레이어 등록 완료  정도? 

	WordManager::GetInstance()->Get_List();// 단어 데이터를 다 얻은 시점.
	m_iStage_Socre = 1000;
}
										
void Play::Delete_Data()
{
	delete m_Player;
	WordManager::GetInstance()->Delete_List();
}

void Play::Save_Game()
{
	ofstream save;
	save.open("Rank.txt", ios::app);
	if (save.is_open())
	{
		save << endl;
		save << m_Player->Name << " " << m_Player->Stage << " " << m_Player->Score;
		save.close();
	}
}

void Play::Main_Play()
{
	while (m_Player->Life != 0 && m_Player->Stage < LAST_STAGE)
	{
		Interface::GetInstance()->Show_Information(m_Player->Life, m_Player->Score, m_Player->Name);
		WordManager::GetInstance()->Check_Stage(m_Player->Stage);
		Stage_Box();

		Game_Play();
		if (m_Player->Life != 0)
			Stage_up();
	}

	if (m_Player->Stage > LAST_STAGE)
		Interface::GetInstance()->Win_Text();
	else
		MapDraw::GetInstance()->BoxDraw(0, 0, WIDTH, HEIGHT);

	Save_Game();
	Delete_Data();
}

void Play::Stage_up()
{
	m_Player->Stage++;
	m_iStage_Socre += 200;
	m_Player->Score = 0;
	WordManager::GetInstance()->Delete_Virus();
	Interface::GetInstance()->Erase_Input_Box();
}

void Play::Get_Score()
{
	int Plus_Score = rand() % 100 + 70; // 70 점에서 169 점 사이의 점수 획듯
	m_Player->Score += Plus_Score;
	Interface::GetInstance()->Player_Score(m_Player->Score);
}

void Play::Game_Play()
{
	int Height_Location, Timer, Main_Time = clock();
	int Penalty_Time = 0;
	string Input_Word = "";
	bool Enter = false, Life_Check = false,Penalty_Check = true;
	Interface::GetInstance()->Input_Box();
	while (m_Player->Life != 0 && m_Player->Score <= m_iStage_Socre)
	{
		Timer = clock();

		if (kbhit() && Penalty_Check)
			Enter_Word(Input_Word, Enter);

		if (Enter && Penalty_Check)
		{
			if (!WordManager::GetInstance()->Chekcing_Word(Input_Word))// 틀렸을때 패널티
			{
				MapDraw::GetInstance()->DrawMidText("                       ", WIDTH, HEIGHT * 0.7f + 3);
				RED					
				Input_Word = " Fail Compare !! "; 
				MapDraw::GetInstance()->DrawMidText(Input_Word, WIDTH, HEIGHT * 0.7f + 3);
				ORIGINAL
					Penalty_Check = false;
				Penalty_Time = clock();
			}
			else
			{
				MapDraw::GetInstance()->DrawMidText("                 ", WIDTH, HEIGHT * 0.7f + 3);
				Get_Score();
				Input_Word = "";
				MapDraw::GetInstance()->DrawMidText(Input_Word, WIDTH, HEIGHT * 0.7f + 3);
			}
			Enter = false;
		} // 엔터 값 까지.

		if (!Penalty_Check && (Timer - Penalty_Time >= ONE_SEC * 3 ))
		{
			Penalty_Check = true;
			Input_Word = "";
			MapDraw::GetInstance()->DrawMidText("                 ", WIDTH, HEIGHT * 0.7f + 3);
			Penalty_Time = 0;
		}
		if (WordManager::GetInstance()->Drop_Time_Control(m_Player->Stage, Main_Time, Timer, Life_Check))
		{
			WordManager::GetInstance()->Word_Drop();
			Interface::GetInstance()->Input_Box();

			if (!Penalty_Check)
				RED
			else
				BLUE
				MapDraw::GetInstance()->DrawMidText(Input_Word, WIDTH, HEIGHT * 0.7f + 3);
			ORIGINAL
		}

		if (Life_Check)
		{
			m_Player->Life--;
			Interface::GetInstance()->Player_Life(m_Player->Life);
			Life_Check = false;
		}
	}
}

void Play::Enter_Word(string & enter_word , bool  & Enter)
{
	char word = NULL;
	bool Checking = true; // 문제 있다 .아무리 나간다고 해도 Checking 에서 true 로 바뀌기 때문에 main_Play 에서 관리하는게 더 좋다

	if (enter_word.length() <= MAX_WORD && Checking) // 그 확인 도 해보자 씨플.
	{
		word = getch();
		if (word == 13 && enter_word.length() >= 1)
			Enter = true;
		else if (word == 8 && enter_word.length() > 0)
			enter_word.pop_back();
		else if (word >= 'a' && word <= 'z')
			enter_word += word;
		BLUE
			MapDraw::GetInstance()->DrawMidText("                   ", WIDTH, HEIGHT * 0.7f + 3);
		MapDraw::GetInstance()->DrawMidText(enter_word, WIDTH, HEIGHT * 0.7f + 3);
		ORIGINAL
	}
}

string Play::Get_Name()
{
	char Name_Ward;
	string Name; 

	BLUE
		MapDraw::GetInstance()->DrawMidText("                   ", WIDTH, HEIGHT* 0.7f + 3);
	MapDraw::GetInstance()->DrawMidText("이름을 입력 하세요", WIDTH, HEIGHT * 0.6f);
	MapDraw::GetInstance()->DrawMidText("", WIDTH, HEIGHT * 0.7f + 3);
	while (Name.length() < 10)
	{
		Name_Ward = getch();
		if (Name_Ward == 13 && Name != "")
			break;
		else if (Name_Ward == 8 && Name.length() > 0)
			Name.pop_back();
		else if (Name_Ward >= 'a' && Name_Ward <= 'z')
			Name += Name_Ward;		

		MapDraw::GetInstance()->DrawMidText("                   ", WIDTH, HEIGHT * 0.7f + 3);
		MapDraw::GetInstance()->DrawMidText(Name, WIDTH, HEIGHT * 0.7f + 3);

	}
	if (Name.length() == 10)
	{
		MapDraw::GetInstance()->DrawMidText("10글자 입력 초과!!", WIDTH, HEIGHT * 0.6f + 1);
		char skip = getch();
	}
	ORIGINAL
		return Name;
}
			   
void Play::Main()
{
	Interface::GetInstance()->Make_Box();
	while (1)
	{
		Interface::GetInstance()->Main_Menu();
		switch (MapDraw::GetInstance()->MenuSelectCursor(3,3 /* 메뉴 간의 거리  */,WIDTH / 2 - 8, HEIGHT * 0.4f /* 커서의 시작 위치  */))
		{	
		case MENU_GAME_START:
			MapDraw::GetInstance()->BoxErase(WIDTH, HEIGHT);
			Interface::GetInstance()->Show_Reading();
			Input_Data(Get_Name());
			Main_Play();
			break;
		case MENU_RANK:
			Interface::GetInstance()->Erase_Main_Menu();
			m_Rank.Rank_List();
			break;
		case MENU_GAME_EIXT:
			Interface::DeleteInstance();
			WordManager::DeleteInstance();
			MapDraw::DeleteInstance();
			return;
		}
	}
} 

Play::~Play()
{
}
