#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* m_Renderer, int m_pieceTeam, int m_XPos, int m_YPos)
	: Piece(m_Renderer, PAWN, m_pieceTeam, m_XPos, m_YPos)
{

}

Pawn::~Pawn()
{
	SDL_DestroyTexture(m_pieceTexture);
}

/*
void Pawn::calculatePossibleMoves(SDL_Renderer* Renderer)
{
	//Render 2 blocks below
	if (currentTurn == m_pieceTeam && m_pieceTeam == 1 && !hasMoved)
	{
		SDL_Rect temp1{};
		temp1.w = WIDTH / 8;
		temp1.h = HEIGHT / 8;
		temp1.x = (m_XPos+1) * temp1.w;
		temp1.y = (m_YPos + 1) * temp1.h;

		SDL_SetRenderDrawColor(Renderer, 150, 0, 0, 100);
		SDL_RenderFillRect(Renderer, &temp1);

		//2 blocks below are checked if empty or not
		if (boardPosition[m_XPos + (m_YPos + 1) * 8] == 0 && boardPosition[m_XPos + (m_YPos + 2) * 8] == 0)
		{

		}
		//only the next block seems empty
		else if (boardPosition[m_XPos + (m_YPos + 1) * 8] == 0)
		{

		}
	}
	//Render 2 blocks above
	else if (currentTurn == m_pieceTeam && m_pieceTeam == 2 && !hasMoved)
	{
		//2 blocks above are checked if empty or not
		if (boardPosition[m_XPos + (m_YPos - 1) * 8] == 0 && boardPosition[m_XPos + (m_YPos - 2) * 8] == 0)
		{

		}
		//only the next block seems empty
		else if (boardPosition[m_XPos + (m_YPos - 1) * 8] == 0)
		{

		}
	}
	//Normal Move and no piece below
	else if (currentTurn == m_pieceTeam && m_pieceTeam == 1 && boardPosition[m_XPos + (m_YPos + 1) * 8] == 0)
	{
		//found enemy piece, can take him out here
		if (boardPosition[(m_XPos + 1) + (m_YPos + 1) * 8] == 1 || boardPosition[(m_XPos - 1) + (m_YPos + 1) * 8] == 1)
		{

		}
	}
	//Normal Move and no piece above
	else if (currentTurn == m_pieceTeam && m_pieceTeam == 2 && boardPosition[m_XPos + (m_YPos - 1) * 8] == 0)
	{
		//found enemy piece, can take him out here
		if (boardPosition[(m_XPos + 1) + (m_YPos - 1) * 8] == 1 || boardPosition[(m_XPos - 1) + (m_YPos - 1) * 8] == 1)
		{

		}
	}
	//waittill clicked elsewhere
	//destroy and free textures/rects
}
*/