#pragma once
#include "Character.h"
#include "Bow.h"
#include "Gun.h"
#include "Dagger.h"
#include "Hammer.h"
#include "Sword.h"
#include "Wand.h"


class Player : public Character, public Inventory
{
private:
	MapDraw m_Draw;
	Inventory * m_Main_Bag;
	Inventory * m_Weapon_Bag;
	Inventory * m_Inven_Weapon;
	
public:

	FIGHT Attack_Try();
	void New_Data(string File);
	void Old_Data(string File);
	void Has_Weapon(string Kind);

	void Has_Load_Weapon(string Kind, string Name, int Demage, int Price);
	void Get_Weapon(Weapon * weapon);
	void Load_Weapon(Weapon * weapon);
	void Get_Inven_Wepaon();

	inline string Out_Weapon_Name()
	{
		return m_Weapon->Out_Name();
	}
	inline string Out_Weapon_Kind()
	{
		return m_Weapon->Out_Kind();
	}

	Player();
	~Player();
};

