#include "Pawn.h"
#include <iostream>

Pawn::Pawn(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, PAWN, m_pieceTeam, m_XPos, m_YPos)
{

}

Pawn::~Pawn()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Pawn::RenderPossibleMoves(SDL_Renderer* Renderer, const int& x, const int& y, bool& currentTurn)
{
	if (currentTurn != this->GetPieceTeam())
		return;

	Piece** boardPosition = Chess::GetBoardPos();

	//Render Black Pawn Moves
	if (currentTurn == this->GetPieceTeam() && !this->GetPieceTeam() && !(this->HasPieceMoved()))
	{
		//2 blocks below are checked if empty or not
		if ((y + 2) < 8 && !boardPosition[x + ((y + 1) * 8)] && !boardPosition[x + ((y + 2) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y + 1) * 8);
			this->PossibleMovesVector().push_back(x + (y + 2) * 8);
		}
		//only the next block seems empty
		else if ((y + 1) < 8 && !boardPosition[x + ((y + 1) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y + 1) * 8);
		}
		//found enemy piece on right
		if ((y + 1) < 8 && (x + 1) < 8 && boardPosition[(x + 1) + (y + 1) * 8] && boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y + 1) * 8));
		}
		//found enemy piece on left
		if ((y + 1) < 8 && (x - 1) >= 0 && boardPosition[(x - 1) + (y + 1) * 8] && boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y + 1) * 8));
		}
	}

	//Render White Pawn Moves
	else if (currentTurn == this->GetPieceTeam() && this->GetPieceTeam() && !(this->HasPieceMoved()))
	{
		//2 blocks above are checked if empty or not
		if ((y - 2) >= 0 && !boardPosition[x + ((y - 1) * 8)] && !boardPosition[x + ((y - 2) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y - 1) * 8);
			this->PossibleMovesVector().push_back(x + (y - 2) * 8);
		}
		//only the next block seems empty
		else if ((y - 1) >= 0 && !boardPosition[x + ((y - 1) * 8)])
		{
			this->PossibleMovesVector().push_back(x + (y - 1) * 8);
		}
		//found enemy piece on right
		if ((y - 1) >= 0 && (x + 1) < 8 && boardPosition[(x + 1) + (y - 1) * 8] && !boardPosition[(x + 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y - 1) * 8));
		}
		//found enemy piece on left
		if ((y - 1) >= 0 && (x - 1) >= 0 && boardPosition[(x - 1) + (y - 1) * 8] && !boardPosition[(x - 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y - 1) * 8));
		}
	}

	//Black's Normal Move and no piece below
	else if (currentTurn == this->GetPieceTeam() && !this->GetPieceTeam())
	{
		if ((y + 1) < 8 && !boardPosition[x + (y + 1) * 8])
		{
			this->PossibleMovesVector().push_back(x + (y + 1) * 8);
		}
		//found enemy piece on right
		if ((y + 1) < 8 && (x + 1) < 8 && boardPosition[(x + 1) + (y + 1) * 8] && boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y + 1) * 8));
		}
		//found enemy piece on right
		if ((y + 1) < 8 && (x - 1) >= 0 && boardPosition[(x - 1) + (y + 1) * 8] && boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y + 1) * 8));
		}
	}
	//White's Normal Move and no piece above
	else if (currentTurn == this->GetPieceTeam() && this->GetPieceTeam())
	{
		if ((y - 1) >= 0 && !boardPosition[x + (y - 1) * 8])
		{
			this->PossibleMovesVector().push_back(x + (y - 1) * 8);
		}
		//found enemy piece on right
		if ((y - 1) >= 0 && (x + 1) < 8 && boardPosition[(x + 1) + (y - 1) * 8] && !boardPosition[(x + 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x + 1) + ((y - 1) * 8));
		}
		//found enemy piece on left
		if ((y - 1) >= 0 && (x -1) >= 0 && boardPosition[(x - 1) + (y - 1) * 8] && !boardPosition[(x - 1) + (y - 1) * 8]->GetPieceTeam())
		{
			this->PossibleMovesVector().push_back((x - 1) + ((y - 1) * 8));
		}
	}

	//Render the possible Moves Box here
	for (int i = 0; i < this->PossibleMovesVector().size(); i++)
	{
		SDL_Rect temp1{ Chess::GetBlockX(this->PossibleMovesVector()[i]) * (WIDTH / 8), Chess::GetBlockY(this->PossibleMovesVector()[i]) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
		SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 128);
		SDL_RenderFillRect(Renderer, &temp1);
	}
}