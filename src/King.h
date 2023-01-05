#pragma once
#include "Piece.h"

class King : public Piece
{
public:
	//store vars from piece to bishop
	King(SDL_Renderer*, bool, float, float);
	//void DrawBishops();
	//bool isValidMove();
	~King();

	void RenderPossibleMoves(SDL_Renderer*);
	std::vector<int> CalculatePossibleMoves();
	void SetCastling(std::vector<int>&);
};