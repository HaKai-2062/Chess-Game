#include "Knight.h"

Knight::Knight(SDL_Renderer * m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KNIGHT, m_pieceTeam, m_XPos, m_YPos)
{

}

Knight::~Knight()
{
	SDL_DestroyTexture(this->GetTexture());
}

void Knight::RenderPossibleMoves(SDL_Renderer* Renderer)
{

	Piece** const boardPosition = Chess::GetBoardPos();

	int x = static_cast<int>(this->GetPieceX());
	int y = static_cast<int>(this->GetPieceY());


	if((x + 2 < 8 && y + 1 < 8) && (!boardPosition[(x + 2) + ((y + 1) * 8)] || (boardPosition[(x + 2) + ((y + 1) * 8)] && boardPosition[(x + 2) + ((y + 1) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x + 2) + ((y + 1) * 8));

	if ((x + 2 < 8 && y - 1 > -1) && (!boardPosition[(x + 2) + ((y - 1) * 8)] || (boardPosition[(x + 2) + ((y - 1) * 8)] && boardPosition[(x + 2) + ((y - 1) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x + 2) + ((y - 1) * 8));

	if ((x - 2 > -1 && y + 1 < 8) && (!boardPosition[(x - 2) + ((y + 1) * 8)] || (boardPosition[(x - 2) + ((y + 1) * 8)] && boardPosition[(x - 2) + ((y + 1) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x - 2) + ((y + 1) * 8));

	if ((x - 2 > -1 && y - 1 > -1) && (!boardPosition[(x - 2) + ((y - 1) * 8)] || (boardPosition[(x - 2) + ((y - 1) * 8)] && boardPosition[(x - 2) + ((y - 1) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x - 2) + ((y - 1) * 8));

	if ((x + 1 < 8 && y - 2 > -1) && (!boardPosition[(x + 1) + ((y - 2) * 8)] || (boardPosition[(x + 1) + ((y - 2) * 8)] && boardPosition[(x + 1) + ((y - 2) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x +1) + ((y - 2) * 8));

	if ((x + 1 < 8 && y + 2 < 8) && (!boardPosition[(x + 1) + ((y + 2) * 8)] || (boardPosition[(x + 1) + ((y + 2) * 8)] && boardPosition[(x + 1) + ((y + 2) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x + 1) + ((y + 2) * 8));

	if ((x - 1 > -1 && y -2 > -1) && (!boardPosition[(x - 1) + ((y - 2) * 8)] || (boardPosition[(x - 1) + ((y - 2) * 8)] && boardPosition[(x - 1) + ((y - 2) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x - 1) + ((y - 2) * 8));

	if ((x - 1 > -1 && y + 2 < 8) && (!boardPosition[(x - 1) + ((y + 2) * 8)] || (boardPosition[(x - 1) + ((y + 2) * 8)] && boardPosition[(x - 1) + ((y + 2) * 8)]->GetPieceTeam() != this->GetPieceTeam())))
		this->PossibleMovesVector().push_back((x - 1) + ((y + 2) * 8));

	this->RenderPossMovesBlock(Renderer);
}