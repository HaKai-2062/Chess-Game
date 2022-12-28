#pragma once
#include "Piece.h"

class King : public Piece
{
public:
	//store vars from piece to bishop
	King(SDL_Renderer*, int, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~King();
};