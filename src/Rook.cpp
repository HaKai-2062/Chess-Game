#include "Rook.h"

Rook::Rook(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, ROOK, m_pieceTeam, m_XPos, m_YPos)
{

}

Rook::~Rook()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Rook::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());

	//Render --> Moves
	int tempValue = x+1;
	while (tempValue < 8)
	{
		//no piece present
		if (!boardPosition[tempValue + (y * 8)])
			this->PossibleMovesVector().push_back(tempValue + (y * 8));
		//if enemy piece present
		else if (boardPosition[tempValue + (y * 8)] && boardPosition[tempValue + (y * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempValue + (y * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempValue + (y * 8)] && boardPosition[tempValue + (y * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempValue++;
	}
	//Render <-- Moves
	tempValue = x-1;
	while (tempValue > -1)
	{
		//no piece present
		if (!boardPosition[tempValue + (y * 8)])
			this->PossibleMovesVector().push_back(tempValue + (y * 8));
		//if enemy piece present
		else if (boardPosition[tempValue + (y * 8)] && boardPosition[tempValue + (y * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempValue + (y * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempValue + (y * 8)] && boardPosition[tempValue + (y * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempValue--;
	}
	//Render DOWN Moves
	tempValue = y+1;
	while (tempValue < 8)
	{
		//no piece present
		if (!boardPosition[x + (tempValue * 8)])
			this->PossibleMovesVector().push_back(x + (tempValue * 8));
		//if enemy piece present
		else if (boardPosition[x + (tempValue * 8)] && boardPosition[x + (tempValue * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(x + (tempValue * 8));
			//no need to loop further in column
			break;
		}
		//same team so break loop
		else if (boardPosition[x + (tempValue * 8)] && boardPosition[x + (tempValue * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempValue++;
	}
	//Render UP Moves
	tempValue = y-1;
	while (tempValue > -1)
	{
		//no piece present
		if (!boardPosition[x + (tempValue * 8)])
			this->PossibleMovesVector().push_back(x + (tempValue * 8));
		//if enemy piece present
		else if (boardPosition[x + (tempValue * 8)] && boardPosition[x + (tempValue * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(x + (tempValue * 8));
			//no need to loop further in column
			break;
		}
		//same team so break loop
		else if (boardPosition[x + (tempValue * 8)] && boardPosition[x + (tempValue * 8)]->GetPieceTeam() == this->GetPieceTeam())
		{
			break;
		}

		tempValue--;
	}
	this->RenderPossMovesBlock(Renderer);
}