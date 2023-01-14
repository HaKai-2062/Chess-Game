#include "King.h"

King::King(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KING, m_pieceTeam, m_XPos, m_YPos)
{

}

King::~King()
{
	PossibleMovesVector().clear();
}

void King::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	CalculateLegitMoves();
	King::PushCastlingMove(this->PossibleMovesVector());
	RenderPossMovesBlock(Renderer);
}

std::vector<int> King::CalculatePossibleMoves()
{
	int x = static_cast<int>(m_XPos);
	int y = static_cast<int>(m_YPos);
	std::vector<int> possibleMoves;

	int arrayX[8] = { x, x, x - 1, x + 1, x + 1, x + 1, x - 1, x - 1 };
	int arrayY[8] = { y + 1, y - 1, y, y, y + 1, y - 1, y + 1, y - 1 };

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

void King::PushCastlingMove(std::vector<int>& possibleMoves)
{
	int x = static_cast<int>(m_XPos);
	int y = static_cast<int>(m_YPos);

	std::vector<int> enemyPossMoves;
	bool canCastleOnLeft = true;
	bool canCastleOnRight = true;

	//is king under check?
	if ((GetPieceTeam() && Piece::isWhiteInCheck) || (!GetPieceTeam() && Piece::isBlackInCheck))
	{
		canCastleOnLeft = false;
		canCastleOnRight = false;
		return;
	}

	
	//castle on left side (alwaays going to be the long castle)
	if (GetPieceType() == KING && !HasPieceMoved() && boardPosition[0 + y * 8] && !boardPosition[0 + y * 8]->HasPieceMoved() && canCastleOnLeft)
	{
		//no piece from 1 to king's pos
		for (int i = 1; i < x; i++)
		{
			//if there is any piece in between set castling to false
			if (boardPosition[i + y * 8])
			{
				canCastleOnLeft = false;
				break;
			}
		}

		//check only x -1 and x - 2 whether it is under attack or not
		for (int i = 0; i < 64; i++)
		{
			if (boardPosition[i] && GetPieceTeam() != boardPosition[i]->GetPieceTeam())
			{
				//should have used CalculateLegitMoves but there was an infinite loop and this should work fine since after mating enemy we cant legally get a turn
				enemyPossMoves.clear();
				enemyPossMoves = boardPosition[i]->CalculatePossibleMoves();

				for (int j = 0; j < enemyPossMoves.size(); j++)
				{
					if (enemyPossMoves[j] == ((x - 1) + (y * 8)) || enemyPossMoves[j] == ((x - 2) + (y * 8)))
					{
						canCastleOnLeft = false;
						i = 64;
						break;
					}
				}
				enemyPossMoves.clear();
			}
		}
		if (canCastleOnLeft)
		{
			if (GetPieceTeam())
			{
				castleBlockWhite[0] = ((x - 2) + (y * 8));
				possibleMoves.push_back((x - 2) + (y * 8));
			}
			else
			{
				castleBlockBlack[0] = ((x - 2) + (y * 8));
				possibleMoves.push_back((x - 2) + (y * 8));
			}
		}
	}


	//castle on left side (alwaays going to be the long castle)
	if (GetPieceType() == KING && !HasPieceMoved() && boardPosition[7 + y * 8] && !boardPosition[7 + y * 8]->HasPieceMoved() && canCastleOnRight)
	{
		//no piece from king's pos to rook
		for (int i = x + 1; i < 7; i++)
		{
			//if there is any piece in between set castling to false
			if (boardPosition[i + y * 8])
			{
				canCastleOnRight = false;
				break;
			}
		}

		//check only x+1 and x+2 whether it is under attack or not
		for (int i = 0; i < 64; i++)
		{
			if (boardPosition[i] && GetPieceTeam() != boardPosition[i]->GetPieceTeam())
			{
				//should have used CalculateLegitMoves but there was an infinite loop and this should work fine since after being mated we cant legally get a turn
				enemyPossMoves.clear();
				enemyPossMoves = boardPosition[i]->CalculatePossibleMoves();

				for (int j = 0; j < enemyPossMoves.size(); j++)
				{
					if (enemyPossMoves[j] == ((x + 1) + (y * 8)) || enemyPossMoves[j] == ((x + 2) + (y * 8)))
					{
						canCastleOnRight = false;
						i = 64;
						break;
					}
				}
				enemyPossMoves.clear();
			}
		}
		if (canCastleOnRight)
		{
			if (GetPieceTeam())
			{
				castleBlockWhite[1] = ((x + 2) + (y * 8));
				possibleMoves.push_back((x + 2) + (y * 8));
			}
			else
			{
				castleBlockBlack[1] = ((x + 2) + (y * 8));
				possibleMoves.push_back((x + 2) + (y * 8));
			}
		}
	}
}