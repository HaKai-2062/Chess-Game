#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, PAWN, m_pieceTeam, m_XPos, m_YPos)
{

}

Pawn::~Pawn()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Pawn::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());

	//Render Black Pawn Moves
	if (!this->GetPieceTeam())
	{
		//if next block is empty
		if ((y + 1) < 8 && !boardPosition[x + ((y + 1) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y + 1) * 8);
		}
		//2 blocks below are checked if empty or not and piece shouldnt have moved
		if ((y + 2) < 8 && !boardPosition[x + ((y + 1) * 8)] && !boardPosition[x + ((y + 2) * 8)] && !(this->HasPieceMoved()))
		{
			this->PossibleMovesVector().push_back(x + (y + 2) * 8);
		}
		//found enemy piece on right
		if ((y + 1) < 8 && (x + 1) < 8 && boardPosition[(x + 1) + (y + 1) * 8] && boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y + 1) * 8));
		}
		//found enemy piece on left
		if ((y + 1) < 8 && (x - 1) > -1 && boardPosition[(x - 1) + (y + 1) * 8] && boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y + 1) * 8));
		}
	}

	//Render White Pawn Moves
	else if (this->GetPieceTeam())
	{
		//only the next block seems empty
		if ((y - 1) > -1 && !boardPosition[x + ((y - 1) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y - 1) * 8);
		}
		//2 blocks above are checked if empty or not
		if ((y - 2) > -1 && !boardPosition[x + ((y - 1) * 8)] && !boardPosition[x + ((y - 2) * 8)] && !(this->HasPieceMoved()))
		{
			this->PossibleMovesVector().push_back(x + (y - 2) * 8);
		}
		//found enemy piece on right
		if ((y - 1) > -1 && (x + 1) < 8 && boardPosition[(x + 1) + (y - 1) * 8] && !boardPosition[(x + 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y - 1) * 8));
		}
		//found enemy piece on left
		if ((y - 1) > -1 && (x - 1) > -1 && boardPosition[(x - 1) + (y - 1) * 8] && !boardPosition[(x - 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y - 1) * 8));
		}
	}
	this->RenderPossMovesBlock(Renderer);
}