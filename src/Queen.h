#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	//store vars from piece to bishop
	Queen(SDL_Renderer*, int, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Queen();
};