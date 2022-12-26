#pragma once
#include "Renderer.h"

class King
{
public:
	King(SDL_Renderer*, int, int, int);
	~King();
private:
	int m_pieceTeam = 0 , m_XPos = 0, m_YPos = 0;
};