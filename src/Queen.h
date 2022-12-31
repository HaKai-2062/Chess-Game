#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	//store vars from piece to bishop
	Queen(SDL_Renderer*, bool, float, float);
	//void DrawBishops();
	//bool isValidMove();
	~Queen();

	void RenderPossibleMoves(SDL_Renderer*) { };
};