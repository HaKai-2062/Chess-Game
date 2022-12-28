#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
	//store vars from piece to bishop
	Rook(SDL_Renderer*, int, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Rook();
};