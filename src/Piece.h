#pragma once
#include "Renderer.h"

enum PieceType { PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4, KING = 5 };

class Piece
{
public:
	PieceType GetPieceType() { return this->m_pieceType; }
	bool HasPieceMoved() { return this->m_hasMoved; }
	bool GetPieceTeam() { return this->m_pieceTeam; }
	float GetPieceX() { return this->m_XPos; }
	float GetPieceY() { return this->m_YPos; }
	std::vector<int>& PossibleMovesVector() { return this->m_possMoves; }
	SDL_Texture* GetTexture() { return this->m_pieceTexture; }
	
	void RenderThePiece(SDL_Renderer*, const PieceType&, const bool&, const float&, const float&);
	void MoveThePiece(SDL_Renderer*, SDL_Texture*, Piece**, int, bool&);

	//every derived class must have these functions
	virtual void RenderPossibleMoves(SDL_Renderer*, const int&, const int&, bool&) = 0;
	//virtual void calculatePossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) = 0;
	//virtual bool isValidMove() = 0;
	//virtual void moveThePiece(int, int) = 0;

	//construction or destrction
	Piece(SDL_Renderer*, PieceType, bool, float, float);
	Piece(const Piece& piece)
		:m_Renderer(piece.m_Renderer), m_pieceType(piece.m_pieceType), m_pieceTeam(piece.m_pieceTeam), m_XPos(piece.m_XPos), m_YPos(piece.m_YPos) {};
	~Piece();

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Texture* m_pieceTexture = nullptr;
	PieceType m_pieceType = PAWN;
	std::vector<int> m_possMoves;
	//true = white
	bool m_pieceTeam = false;
	bool m_hasMoved = false;
	float m_XPos = 0, m_YPos = 0;
};