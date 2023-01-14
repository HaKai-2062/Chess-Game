#include "Knight.h"

Knight::Knight(SDL_Renderer * m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KNIGHT, m_pieceTeam, m_XPos, m_YPos)
{

}

Knight::~Knight()
{
	PossibleMovesVector().clear();
}

void Knight::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	CalculateLegitMoves();
	RenderPossMovesBlock(Renderer);
}

std::vector<int> Knight::CalculatePossibleMoves()
{
	int x = static_cast<int>(m_XPos);
	int y = static_cast<int>(m_YPos);
	std::vector<int> possibleMoves;

	int arrayX[8] = { x + 2, x + 2, x + 1, x + 1, x - 1, x - 1, x - 2, x - 2 };
	int arrayY[8] = { y + 1, y - 1, y + 2, y - 2, y + 2, y - 2, y + 1, y - 1 };

	possibleMoves.clear();
	for (int i = 0; i < 8; i++)
	{
		if (arrayX[i] > -1 && arrayX[i] < 8 && arrayY[i] > -1 && arrayY[i] < 8)
		{
			if (!boardPosition[arrayX[i] + arrayY[i] * 8] || (boardPosition[arrayX[i] + arrayY[i] * 8]->GetPieceTeam() != GetPieceTeam()))
				possibleMoves.push_back(arrayX[i] + arrayY[i] * 8);
		}
	}
	return possibleMoves;
}