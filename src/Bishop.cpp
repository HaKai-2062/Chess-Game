#include "Bishop.h"

Bishop::Bishop(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, BISHOP, m_pieceTeam, m_XPos, m_YPos)
{

}

Bishop::~Bishop()
{
	SDL_DestroyTexture(this->GetPieceTexture());
	SDL_FreeRW(this->GetPieceRW());
	SDL_FreeSurface(this->GetPieceSurface());
	this->PossibleMovesVector().clear();
}

void Bishop::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());

	int arrayX[4] = {x + 1, x + 1, x - 1, x - 1};
	int arrayY[4] = {y + 1, y - 1, y + 1, y - 1};

	for (int i = 0; i < 4; i++)
	{
		while (arrayX[i] > -1 && arrayX[i] < 8 && arrayY[i] > -1 && arrayY[i] < 8)
		{
			//no piece present
			if (!boardPosition[arrayX[i] + (arrayY[i] * 8)])
				this->PossibleMovesVector().push_back(arrayX[i] + (arrayY[i] * 8));
			//if enemy piece present
			else if (boardPosition[arrayX[i] + (arrayY[i] * 8)] && boardPosition[arrayX[i] + (arrayY[i] * 8)]->GetPieceTeam() != this->GetPieceTeam())
			{
				this->PossibleMovesVector().push_back(arrayX[i] + (arrayY[i] * 8));
				//no need to loop further in row
				break;
			}
			//same team so break loop
			else if (boardPosition[arrayX[i] + (arrayY[i] * 8)] && boardPosition[arrayX[i] + (arrayY[i] * 8)]->GetPieceTeam() == this->GetPieceTeam())
				break;

			if (i == 0)
			{
				arrayX[i]++;
				arrayY[i]++;
			}
			else if (i == 1)
			{
				arrayX[i]++;
				arrayY[i]--;
			}
			else if (i == 2)
			{
				arrayX[i]--;
				arrayY[i]++;
			}
			else
			{
				arrayX[i]--;
				arrayY[i]--;
			}
		}
	}

	this->RenderPossMovesBlock(Renderer);
}