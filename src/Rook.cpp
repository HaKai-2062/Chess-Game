#include "Rook.h"

Rook::Rook(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, ROOK, m_pieceTeam, m_XPos, m_YPos)
{

}

Rook::~Rook()
{
	SDL_DestroyTexture(this->GetPieceTexture());
	SDL_FreeRW(this->GetPieceRW());
	SDL_FreeSurface(this->GetPieceSurface());
	this->PossibleMovesVector().clear();
}

void Rook::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	this->CalculateMovesForCheck();
	this->RenderPossMovesBlock(Renderer);
}

std::vector<int> Rook::CalculatePossibleMoves()
{
	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());
	std::vector<int> possbleMoves;

	int arrayX[4] = { x + 1, x - 1, x, x };
	int arrayY[4] = { y, y, y + 1, y - 1 };

	possbleMoves.clear();
	for (int i = 0; i < 4; i++)
	{
		while (arrayX[i] > -1 && arrayX[i] < 8 && arrayY[i] > -1 && arrayY[i] < 8)
		{
			//no piece present
			if (!boardPosition[arrayX[i] + (arrayY[i] * 8)])
				possbleMoves.push_back(arrayX[i] + (arrayY[i] * 8));
			//if enemy piece present
			else if (boardPosition[arrayX[i] + (arrayY[i] * 8)] && boardPosition[arrayX[i] + (arrayY[i] * 8)]->GetPieceTeam() != this->GetPieceTeam())
			{
				possbleMoves.push_back(arrayX[i] + (arrayY[i] * 8));
				//no need to loop further in row
				break;
			}
			//same team so break loop
			else if (boardPosition[arrayX[i] + (arrayY[i] * 8)] && boardPosition[arrayX[i] + (arrayY[i] * 8)]->GetPieceTeam() == this->GetPieceTeam())
				break;

			if (i == 0)
				arrayX[i]++;
			else if (i == 1)
				arrayX[i]--;
			else if (i == 2)
				arrayY[i]++;
			else
				arrayY[i]--;
		}
	}
	return possbleMoves;
}