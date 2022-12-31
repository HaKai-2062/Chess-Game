#include "Piece.h"
#include <iostream>
#include <cmath>

Piece::Piece(SDL_Renderer* Renderer, PieceType pieceType, bool pieceTeam, float xPos, float yPos)
	:m_Renderer(Renderer), m_pieceType(pieceType), m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	RenderThePiece(Renderer, pieceType, pieceTeam, xPos, yPos);
}
Piece::~Piece()
{
	SDL_DestroyTexture(this->m_pieceTexture);
	this->PossibleMovesVector().clear();
}

void Piece::RenderThePiece(SDL_Renderer* Renderer, const PieceType& pieceType, const bool& pieceTeam, const float& xPos, const float& yPos)
{
	//Rendering Piece
	std::string fileName = "";
	switch (this->m_pieceType)
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

	if (!this->m_pieceTeam)
		this->m_pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + fileName + "_bl.png").c_str());
	else
		this->m_pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + fileName + ".png").c_str());

	if (!this->m_pieceTexture)
	{
		if (!this->m_pieceTeam)
			Chess::MissingTexture(false, fileName + "_bl.png");
		else
			Chess::MissingTexture(false, fileName + ".png");
		return;
	}
	SDL_Rect pieceRect{};
	pieceRect.w = WIDTH / 8;
	pieceRect.h = HEIGHT / 8;
	pieceRect.x = (int)(m_XPos * pieceRect.w);
	pieceRect.y = (int)(m_YPos * pieceRect.h);

	SDL_RenderCopy(Renderer, m_pieceTexture, nullptr, &pieceRect);
}

void Piece::MoveThePiece(SDL_Renderer* Renderer, SDL_Texture* Image, int boardPositionToMove, bool& currentTurn)
{
	if (currentTurn != this->GetPieceTeam())
		return;

	Piece** const boardPosition = Chess::GetBoardPos();
	
	this->m_hasMoved = true;

	int xStart = static_cast<int>(this->GetPieceX());
	int yStart = static_cast<int>(this->GetPieceY());

	int xEnd = Chess::GetBlockX(boardPositionToMove);
	int yEnd = Chess::GetBlockY(boardPositionToMove);

	// Set the number of steps for the animation
	float steps = 16.0f;
	
	// Calculate the x and y step sizes for the animation
	float xStep = (xEnd - xStart) / steps;
	float yStep = (yEnd - yStart) / steps;

	for (int i = 0; i < steps; i++)
	{
		this->AddToX(xStep);
		this->AddToY(yStep);

		//Destroy All piece texture to free all the memory leaks
		for (int j = 0; j < 64; j++)
		{
			if (boardPosition[j])
			{
				SDL_DestroyTexture(boardPosition[j]->GetTexture());
				boardPosition[j]->PossibleMovesVector().clear();
			}
		}
		//The piece is rerenderer here
		SDL_RenderClear(Renderer);
		SDL_Rect rectangle{ 0, 0, WIDTH / 2, HEIGHT / 2 };
		SDL_QueryTexture(Image, nullptr, nullptr, &rectangle.w, &rectangle.h);
		SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
		Chess::RenderAllPiece(Renderer);
		SDL_RenderPresent(Renderer);
	}

	int startPosition = xStart + (yStart * 8);
	int endPosition = xEnd + (yEnd * 8);

	//if piece exists then destroy it
	if (boardPosition[endPosition])
		delete boardPosition[endPosition];
	//assign new position to the piece
	boardPosition[endPosition] = boardPosition[startPosition];
	//set prev piece position to null
	boardPosition[startPosition] = nullptr;

	//change the turn
	currentTurn = !currentTurn;
}