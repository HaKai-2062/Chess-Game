#include "King.h"
#include <iostream>
King::King(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KING, m_pieceTeam, m_XPos, m_YPos)
{

}

King::~King()
{
	SDL_DestroyTexture(this->GetPieceTexture());
	SDL_FreeRW(this->GetPieceRW());
	SDL_FreeSurface(this->GetPieceSurface());
	this->PossibleMovesVector().clear();
}

void King::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());

	int arrayX[8] = { x, x, x-1, x+1, x+1, x+1, x-1, x-1};
	int arrayY[8] = { y+1, y-1, y, y, y+1, y-1, y+1, y-1};

	for (int i = 0; i < 8; i++)
	{
		if (arrayX[i] > -1 && arrayX[i] < 8 && arrayY[i] > -1 && arrayY[i] < 8)
		{
			if (!boardPosition[arrayX[i] + arrayY[i] * 8] || (boardPosition[arrayX[i] + arrayY[i] * 8]->GetPieceTeam() != this ->GetPieceTeam()))
				this->PossibleMovesVector().push_back(arrayX[i] + arrayY[i] * 8);
		}
	}
	this->RenderPossMovesBlock(Renderer);
}
