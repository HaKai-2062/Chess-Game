#include "Piece.h"
#include <iostream>

Piece::Piece(SDL_Renderer* Renderer, PieceType pieceType, int pieceTeam, int xPos, int yPos)
	:m_Renderer(Renderer), m_pieceType(pieceType), m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	//Rendering Piece
	std::string fileName = "";
	switch (m_pieceType)
	{
	case PAWN:
		fileName = "pawn";
		break;
	case KNIGHT:
		fileName = "knight";
		break;
	case BISHOP:
		fileName = "bishop";
		break;
	case ROOK:
		fileName = "rook";
		break;
	case QUEEN:
		fileName = "queen";
		break;
	case KING:
		fileName = "king";
		break;
	default:
		break;
	}

	if (!m_pieceTeam)
		m_pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + fileName+"_bl.png").c_str());
	else
		m_pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + fileName + ".png").c_str());

	if (!m_pieceTexture)
	{
		if (!m_pieceTeam)
			Chess::MissingTexture(false, fileName + "_bl.png");
		else
			Chess::MissingTexture(false, fileName+".png");
		return;
	}
	SDL_Rect pieceRect{};
	pieceRect.w = WIDTH / 8;
	pieceRect.h = HEIGHT / 8;
	pieceRect.x = m_XPos * pieceRect.w;
	pieceRect.y = m_YPos * pieceRect.h;

	SDL_RenderCopy(Renderer, m_pieceTexture, nullptr, &pieceRect);

	//Store the position and team/type in PieceInfo
	//PieceInfo boardPosition[xPos][yPos] = 0;
}
Piece::~Piece()
{
	SDL_DestroyTexture(m_pieceTexture);
}