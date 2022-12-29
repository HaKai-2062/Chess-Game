#pragma once
#include "Piece.h"

class King : public Piece
{
public:
	//store vars from piece to bishop
	King(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~King();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) { };
};