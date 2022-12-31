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

void Piece::MoveThePiece(SDL_Renderer* Renderer, SDL_Texture* Image, Piece** boardPosition, int boardPositionToMove, bool& currentTurn)
{
	if (currentTurn != this->GetPieceTeam())
		return;

	this->m_hasMoved = true;

	float xStart = (float)this->GetPieceX();
	float yStart = (float)this->GetPieceY();

	float xEnd = (float)Chess::GetBlockX(boardPositionToMove);
	float yEnd = (float)Chess::GetBlockY(boardPositionToMove);

	bool xDiffIncrease = true;
	bool yDiffIncrease = true;


	if (xEnd - xStart < 0)
		xDiffIncrease = false;

	if (yEnd - yStart < 0)
		yDiffIncrease = false;


	float diff = 0.0f;

	//a simple lerp function that runs for 10 times
	while (1)
	{
		if (diff > 1.0f)
			break;

		diff = diff + 0.1f;

		if (xStart != xEnd)
			this->m_XPos = xStart + (xEnd - xStart) * diff;

		if (yStart != yEnd)
			this->m_YPos = yStart + (yEnd - yStart) * diff;

		//Destroy All piece texture to free all the memory leaks
		for (int i = 0; i < 64; i++)
		{
			if (boardPosition[i])
			{
				SDL_DestroyTexture(boardPosition[i]->GetTexture());
				boardPosition[i]->PossibleMovesVector().clear();
			}
		}

		SDL_Rect rectangle{ 0, 0, WIDTH / 2, HEIGHT / 2 };
		SDL_QueryTexture(Image, nullptr, nullptr, &rectangle.w, &rectangle.h);
		//The piece is rerenderer here
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
		Chess::RenderAllPiece(Renderer);
		SDL_RenderPresent(Renderer);
	}

	std::cout << (int)(xEnd + (yEnd * 8) )<< std::endl;
	std::cout << (int)(xStart + (yStart * 8)) << std::endl;

	//if piece exists then destroy it
	if (boardPosition[(int)(xEnd + (yEnd * 8))])
	{
		delete boardPosition[(int)(xEnd + (yEnd * 8))];
		std::cout << "Destroyed the piece :x" << std::endl;
	}
	//assign new position to the piece
	boardPosition[(int)(xEnd + (yEnd * 8))] = boardPosition[(int)(xStart + (yStart * 8))];
	//set prev piece position to null
	boardPosition[(int)(xStart + (yStart * 8))] = nullptr;

	//change the turn
	currentTurn = !currentTurn;
}