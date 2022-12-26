#pragma once
#include "Renderer.h"

class Pawn
{
public:
	Pawn(SDL_Renderer*, int, int, int);
	~Pawn();
	void calculatePossibleMoves();
private:
	int m_pieceTeam = 0, m_XPos = 0, m_YPos = 0;
	SDL_Texture* m_pieceTexture = nullptr;
};