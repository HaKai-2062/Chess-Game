#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	//store vars from piece to bishop
	Queen(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Queen();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) { };
};