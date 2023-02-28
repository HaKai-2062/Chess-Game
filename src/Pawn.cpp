#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, PAWN, m_pieceTeam, m_XPos, m_YPos)
{

}

Pawn::~Pawn()
{
	PossibleMovesVector().clear();
}

void Pawn::RenderPossibleMoves(SDL_Renderer* Renderer)
{
	CalculateLegitMoves();
	RenderPossMovesBlock(Renderer);
}

std::vector<int> Pawn::CalculatePossibleMoves()
{
	int x = static_cast<int>(m_XPos);
	int y = static_cast<int>(m_YPos);
	std::vector<int> possibleMoves;

	possibleMoves.clear();
	//Render Black Pawn Moves
	if (!GetPieceTeam())
	{
		//if next block is empty
		if ((y + 1) < 8 && !boardPosition[x + ((y + 1) * 8)])
		{
			possibleMoves.push_back(x + (y + 1) * 8);
		}
		//2 blocks below are checked if empty or not and piece shouldnt have moved
		if ((y + 2) < 8 && !boardPosition[x + ((y + 1) * 8)] && !boardPosition[x + ((y + 2) * 8)] && !HasPieceMoved())
		{
			possibleMoves.push_back(x + (y + 2) * 8);

		}
		//found enemy piece on right
		if ((y + 1) < 8 && (x + 1) < 8 && boardPosition[(x + 1) + (y + 1) * 8] && boardPosition[(x + 1) + (y + 1) * 8]->GetPieceTeam())
		{
			possibleMoves.push_back((x + 1) + ((y + 1) * 8));
		}
		//found enemy piece on left
		if ((y + 1) < 8 && (x - 1) > -1 && boardPosition[(x - 1) + (y + 1) * 8] && boardPosition[(x - 1) + (y + 1) * 8]->GetPieceTeam())
		{
			possibleMoves.push_back((x - 1) + ((y + 1) * 8));
		}

		//EnPassasnt Logic
		if (x - 1 > -1 && y + 1 < 8 && Piece::enPassant != 99 && Piece::enPassant == x + (y * 8) && !boardPosition[(x - 1) + (y + 1) * 8] && boardPosition[(x - 1) + y * 8] && boardPosition[(x - 1) + y * 8]->GetPieceType() == PAWN)
		{
			//save enemy piece and evaluate if my enPassant is good
			Piece* tempPiece = boardPosition[(x - 1) + y * 8];
			boardPosition[(x - 1) + y * 8] = nullptr;

			if (boardPosition[x + y * 8]->IsLegitMove((x - 1) + ((y + 1) * 8)))
				possibleMoves.push_back((x - 1) + ((y + 1) * 8));

			if (tempPiece)
				boardPosition[(x - 1) + y * 8] = tempPiece;

			tempPiece = nullptr;
		}
		else if (x + 1 < 8 && y + 1 < 8 && Piece::enPassant != 99 && Piece::enPassant == (x + (y * 8)) * 100 && !boardPosition[(x + 1) + (y + 1) * 8] && boardPosition[(x + 1) + y * 8] && boardPosition[(x + 1) + y * 8]->GetPieceType() == PAWN)
		{
			//save enemy piece and evaluate if my enPassant is good
			Piece* tempPiece = boardPosition[(x + 1) + y * 8];
			boardPosition[(x + 1) + y * 8] = nullptr;

			if (boardPosition[x + y * 8]->IsLegitMove((x + 1) + ((y + 1) * 8)))
				possibleMoves.push_back((x + 1) + ((y + 1) * 8));

			if (tempPiece)
				boardPosition[(x + 1) + y * 8] = tempPiece;

			tempPiece = nullptr;
		}
	}

	//Render White Pawn Moves
	else if (GetPieceTeam())
	{
		//only the next block seems empty
		if ((y - 1) > -1 && !boardPosition[x + ((y - 1) * 8)])
		{
			possibleMoves.push_back(x + (y - 1) * 8);
		}
		//2 blocks above are checked if empty or not
		if ((y - 2) > -1 && !boardPosition[x + ((y - 1) * 8)] && !boardPosition[x + ((y - 2) * 8)] && !HasPieceMoved())
		{
			possibleMoves.push_back(x + (y - 2) * 8);
		}
		//found enemy piece on right
		if ((y - 1) > -1 && (x + 1) < 8 && boardPosition[(x + 1) + (y - 1) * 8] && !boardPosition[(x + 1) + (y - 1) * 8]->GetPieceTeam())
		{
			possibleMoves.push_back((x + 1) + ((y - 1) * 8));
		}
		//found enemy piece on left
		if ((y - 1) > -1 && (x - 1) > -1 && boardPosition[(x - 1) + (y - 1) * 8] && !boardPosition[(x - 1) + (y - 1) * 8]->GetPieceTeam())
		{
			possibleMoves.push_back((x - 1) + ((y - 1) * 8));
		}
		
		//EnPassasnt Logic
		if (x - 1 > -1 && y - 1 > -1 && Piece::enPassant != 99 && Piece::enPassant == x + (y * 8) && !boardPosition[(x - 1) + (y - 1) * 8] && boardPosition[(x - 1) + y * 8] && boardPosition[(x - 1) + y * 8]->GetPieceType() == PAWN)
		{
			//save enemy piece and evaluate if my enPassant is good
			Piece* tempPiece = boardPosition[(x - 1) + y * 8];
			boardPosition[(x - 1) + y * 8] = nullptr;

			if (boardPosition[x + y * 8]->IsLegitMove((x - 1) + ((y - 1) * 8)))
				possibleMoves.push_back((x - 1) + ((y - 1) * 8));

			if (tempPiece)
				boardPosition[(x - 1) + y * 8] = tempPiece;

			tempPiece = nullptr;
		}
		else if (x + 1 < 8 && y - 1 > -1 && Piece::enPassant != 99 && Piece::enPassant == (x + (y * 8)) * 100 && !boardPosition[(x + 1) + (y - 1) * 8] && boardPosition[(x + 1) + y * 8] && boardPosition[(x + 1) + y * 8]->GetPieceType() == PAWN)
		{
			//save enemy piece and evaluate if my enPassant is good
			Piece* tempPiece = boardPosition[(x + 1) + y * 8];
			boardPosition[(x + 1) + y * 8] = nullptr;

			if (boardPosition[x + y * 8]->IsLegitMove((x + 1) + ((y - 1) * 8)))
				possibleMoves.push_back((x + 1) + ((y - 1) * 8));

			if (tempPiece)
				boardPosition[(x + 1) + y * 8] = tempPiece;

			tempPiece = nullptr;
		}
	}
	return possibleMoves;
}