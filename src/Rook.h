#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
	//store vars from piece to bishop
	Rook(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Rook();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) { };
};