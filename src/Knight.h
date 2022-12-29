#pragma once
#include "Piece.h"

class Knight : public Piece
{
public:
	//store vars from piece to bishop
	Knight(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Knight();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) { };
};