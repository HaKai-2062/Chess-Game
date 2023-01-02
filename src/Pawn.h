#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	//store vars from piece to bishop
	Pawn(SDL_Renderer*, bool, float, float);
	//void DrawBishops();
	//bool isValidMove();
	~Pawn();

	void RenderPossibleMoves(SDL_Renderer*);
	std::vector<int> CalculatePossibleMoves();
};