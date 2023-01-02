#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
	//store vars from piece to bishop
	Rook(SDL_Renderer*, bool, float, float);
	//void DrawBishops();
	//bool isValidMove();
	~Rook();

	void RenderPossibleMoves(SDL_Renderer*);
	std::vector<int> CalculatePossibleMoves();
};