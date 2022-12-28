#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	//store vars from piece to bishop
	Bishop(SDL_Renderer*, int, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Bishop();
};