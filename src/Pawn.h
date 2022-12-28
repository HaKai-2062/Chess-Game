#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	//store vars from piece to bishop
	Pawn(SDL_Renderer*, int, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Pawn();
};