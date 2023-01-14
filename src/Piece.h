#pragma once
#include "Renderer.h"
#include <iostream>

enum PieceType { PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4, KING = 5 };

class Piece
{
public:
	PieceType GetPieceType() { return m_pieceType; }
	bool HasPieceMoved() { return m_hasMoved; }
	bool GetPieceTeam() { return m_pieceTeam; }
	float GetPieceX() { return m_XPos; }
	float GetPieceY() { return m_YPos; }

	std::vector<int>& PossibleMovesVector() { return m_possMoves; }
	
	void RenderThePiece(SDL_Renderer*, const PieceType&, const bool&, const float&, const float&);
	void RenderPossMovesBlock(SDL_Renderer*);
	void MoveThePiece(SDL_Renderer*, int, bool&);
	void SetKingVariables();
	void SetEnPassant(const int&, const int&, const int&, const int&);
	void SetCastling(SDL_Renderer*, const int&, const int&, bool&);
	void SetPawnPromotion(SDL_Renderer*, const int&, const int&);
	bool EndGameReached();

	//every derived class must have these functions
	virtual void RenderPossibleMoves(SDL_Renderer*) = 0;
	virtual std::vector<int> CalculatePossibleMoves() = 0;
	void CalculateLegitMoves();
	bool IsLegitMove(const int&);

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
	//enPassant is stored in a hacky way, 1-64 would be piece on right of board, 99 would be not in enPassant and (1-64) * 100 would be piece on left of board
	static int enPassant;
	static int enPassantEnd;
	//to castle both left and right side of king
	static int castleBlockBlack[2];
	static int castleBlockWhite[2];
	float m_XPos = 0, m_YPos = 0;

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Texture* m_pieceTexture = nullptr;
	PieceType m_pieceType = PAWN;
	std::vector<int> m_possMoves;

	//true = white
	bool m_pieceTeam = false;
	bool m_hasMoved = false;
};