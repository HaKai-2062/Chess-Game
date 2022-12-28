#pragma once
#include "Renderer.h"

enum PieceType { PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4, KING = 5 };

class Piece
{
public:
	PieceType GetPieceType() { return this->m_pieceType; };
	int GetPieceTeam() { return this->m_pieceTeam; };
	int GetPieceX() { return this->m_XPos; };
	int GetPieceY() { return this->m_YPos; };
	SDL_Texture* GetTexture() { return this->m_pieceTexture; };

	//every derived class must have these functions
	//virtual bool isValidMove() = 0;
	//virtual void moveThePiece(int, int) = 0;

	//construction or destrction
	Piece(SDL_Renderer*, PieceType, int, int, int);
	//Piece(const Piece& piece)
	//	:m_Renderer(piece.m_Renderer), m_pieceType(piece.m_pieceType), m_pieceTeam(piece.m_pieceTeam), m_XPos(piece.m_XPos), m_YPos(piece.m_YPos) {};
	~Piece();

protected:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Texture* m_pieceTexture = nullptr;
	PieceType m_pieceType = PAWN;
	//true = white
	int m_pieceTeam = 0;
	int m_XPos = 0, m_YPos = 0;
};