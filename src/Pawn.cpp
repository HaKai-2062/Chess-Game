#include "Pawn.h"

Pawn::Pawn(SDL_Renderer* Renderer, int pieceTeam, int xPos, int yPos)
	:m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos), m_pieceTexture(nullptr)
{
	SDL_Texture* pieceTexture = nullptr;
	if (m_pieceTeam == 1)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "pawn_bl.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "pawn_bl.png");
			return;
		}
	}
	else if (m_pieceTeam == 2)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "pawn.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "pawn.png");
			return;
		}
	}

	SDL_Rect temp{};
	temp.w = WIDTH / 8;
	temp.h = HEIGHT / 8;
	temp.x = m_XPos * temp.w;
	temp.y = m_YPos * temp.h;

	m_pieceTexture = pieceTexture;
	SDL_RenderCopy(Renderer, pieceTexture, nullptr, &temp);
}

Pawn::~Pawn()
{
	SDL_DestroyTexture(m_pieceTexture);
}

void Pawn::calculatePossibleMoves()
{
	//Render 2 blocks below
	if (currentTurn == m_pieceTeam && m_pieceTeam == 1 && m_YPos == 1)
	{
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
	else if (currentTurn == m_pieceTeam && m_pieceTeam == 2 && m_YPos == 6)
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
}