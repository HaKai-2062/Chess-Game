#include "Queen.h"

Queen::Queen(SDL_Renderer* Renderer, int pieceTeam, int xPos, int yPos)
	:m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	SDL_Texture* pieceTexture = nullptr;
	if (m_pieceTeam == 1)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "queen_bl.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "queen_bl.png");
			return;
		}
	}
	else if (m_pieceTeam == 2)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "queen.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "queen.png");
			return;
		}
	}


	SDL_Rect temp{};
	temp.w = WIDTH / 8;
	temp.h = HEIGHT / 8;
	temp.x = m_XPos * temp.w;
	temp.y = m_YPos * temp.h;

	SDL_RenderCopy(Renderer, pieceTexture, nullptr, &temp);
}