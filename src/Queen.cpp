#include "Queen.h"

Queen::Queen(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, QUEEN, m_pieceTeam, m_XPos, m_YPos)
{

}

Queen::~Queen()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Queen::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());

	//Render Top Right Moves
	int tempX = x + 1;	//increament
	int tempY = y - 1;	//decrement

	while (tempX < 8 && tempY > -1)
	{
		//no piece present
		if (!boardPosition[tempX + (tempY * 8)])
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX++;
		tempY--;
	}

	//Render Top Left Moves
	tempX = x - 1;	//decrement
	tempY = y - 1;	//decrement

	while (tempX > -1 && tempY > -1)
	{
		//no piece present
		if (!boardPosition[tempX + (tempY * 8)])
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX--;
		tempY--;
	}

	//Render Botttom Right Moves
	tempX = x + 1;	//increament
	tempY = y + 1;	//increament

	while (tempX < 8 && tempY < 8)
	{
		//no piece present
		if (!boardPosition[tempX + (tempY * 8)])
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX++;
		tempY++;
	}

	//Render Botttom Leftt Moves
	tempX = x - 1;	//decreament
	tempY = y + 1;	//increament

	while (tempX > -1 && tempY < 8)
	{
		//no piece present
		if (!boardPosition[tempX + (tempY * 8)])
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (tempY * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (tempY * 8)] && boardPosition[tempX + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX--;
		tempY++;
	}

	//Render --> Moves
	tempX = x + 1;
	while (tempX < 8)
	{
		//no piece present
		if (!boardPosition[tempX + (y * 8)])
			this->PossibleMovesVector().push_back(tempX + (y * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (y * 8)] && boardPosition[tempX + (y * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (y * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (y * 8)] && boardPosition[tempX + (y * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX++;
	}
	//Render <-- Moves
	tempX = x - 1;
	while (tempX > -1)
	{
		//no piece present
		if (!boardPosition[tempX + (y * 8)])
			this->PossibleMovesVector().push_back(tempX + (y * 8));
		//if enemy piece present
		else if (boardPosition[tempX + (y * 8)] && boardPosition[tempX + (y * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(tempX + (y * 8));
			//no need to loop further in row
			break;
		}
		//same team so break loop
		else if (boardPosition[tempX + (y * 8)] && boardPosition[tempX + (y * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempX--;
	}
	//Render DOWN Moves
	tempY = y + 1;
	while (tempY < 8)
	{
		//no piece present
		if (!boardPosition[x + (tempY * 8)])
			this->PossibleMovesVector().push_back(x + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[x + (tempY * 8)] && boardPosition[x + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(x + (tempY * 8));
			//no need to loop further in column
			break;
		}
		//same team so break loop
		else if (boardPosition[x + (tempY * 8)] && boardPosition[x + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
			break;

		tempY++;
	}
	//Render UP Moves
	tempY = y - 1;
	while (tempY > -1)
	{
		//no piece present
		if (!boardPosition[x + (tempY * 8)])
			this->PossibleMovesVector().push_back(x + (tempY * 8));
		//if enemy piece present
		else if (boardPosition[x + (tempY * 8)] && boardPosition[x + (tempY * 8)]->GetPieceTeam() != this->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back(x + (tempY * 8));
			//no need to loop further in column
			break;
		}
		//same team so break loop
		else if (boardPosition[x + (tempY * 8)] && boardPosition[x + (tempY * 8)]->GetPieceTeam() == this->GetPieceTeam())
		{
			break;
		}

		tempY--;
	}

	this->RenderPossMovesBlock(Renderer);
}