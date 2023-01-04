#pragma once
#include "Renderer.h"
#include <iostream>

enum PieceType { PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4, KING = 5 };

class Piece
{
public:
	PieceType GetPieceType() { return this->m_pieceType; }
	bool HasPieceMoved() { return this->m_hasMoved; }
	bool GetPieceTeam() { return this->m_pieceTeam; }
	float GetPieceX() { return this->m_XPos; }
	float GetPieceY() { return this->m_YPos; }
	void AddToX(const float& x) { this->m_XPos = this->m_XPos + x; }
	void AddToY(const float& y) { this->m_YPos = this->m_YPos + y; }

	SDL_Texture* GetPieceTexture() { return this->m_pieceTexture; }
	SDL_RWops* GetPieceRW() { return this->m_pieceRW; }
	SDL_Surface* GetPieceSurface() { return this->m_pieceSurface; }
	std::vector<int>& PossibleMovesVector() { return this->m_possMoves; }
	
	void RenderThePiece(SDL_Renderer*, const PieceType&, const bool&, const float&, const float&);
	void RenderPossMovesBlock(SDL_Renderer*);
	void MoveThePiece(SDL_Renderer*, int, bool&);
	void SetKingVariables();
	bool IsLegitMove(const int&);

	//every derived class must have these functions
	virtual void RenderPossibleMoves(SDL_Renderer*) = 0;
	virtual std::vector<int> CalculatePossibleMoves() = 0;
	void CalculateMovesForCheck();
	//virtual void calculatePossibleMoves(SDL_Renderer*, const bool&, const int&, const int&) = 0;
	//virtual bool isValidMove() = 0;
	//virtual void moveThePiece(int, int) = 0;

	//construction or destrction
	Piece(SDL_Renderer*, PieceType, bool, float, float);
	Piece(const Piece& piece)
		:m_Renderer(piece.m_Renderer), m_pieceType(piece.m_pieceType), m_pieceTeam(piece.m_pieceTeam), m_XPos(piece.m_XPos), m_YPos(piece.m_YPos) {};
	~Piece();

protected:
	static bool isBlackInCheck;
	static bool isWhiteInCheck;
	static int blackKingPos;
	static int whiteKingPos;

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_RWops* m_pieceRW = nullptr;
	SDL_Surface* m_pieceSurface = nullptr;
	SDL_Texture* m_pieceTexture = nullptr;
	PieceType m_pieceType = PAWN;
	std::vector<int> m_possMoves;

	float m_XPos = 0, m_YPos = 0;
	//true = white
	bool m_pieceTeam = false;
	bool m_hasMoved = false;
};