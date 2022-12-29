#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	//store vars from piece to bishop
	Pawn(SDL_Renderer*, bool, int, int);
	//void DrawBishops();
	//bool isValidMove();
	~Pawn();

	void RenderPossibleMoves(SDL_Renderer*, const bool&, const int&, const int&);
};