#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	//store vars from piece to bishop
	Bishop(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Bishop();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) { };
};