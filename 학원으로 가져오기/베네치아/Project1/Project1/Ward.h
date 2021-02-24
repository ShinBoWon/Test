#pragma once
#include "MapDraw.h" // Ward 파일 불러오는 파일 이름 Word 인거 확인해 두기
class Ward
{
private:
	int m_iLocation_x; // x,y 좌표 위치
	int m_iLocation_y;
	string m_strWard; // 불러 들여온 단어
	MapDraw m_Draw;

public:
	void Drop();
	void Live();
	void Die();
	Ward();
	~Ward();
};

