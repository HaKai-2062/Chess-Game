#include "King.h"

King::King(SDL_Renderer* Renderer, int pieceTeam, int xPos, int yPos)
	:m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	SDL_Texture* pieceTexture = nullptr;
	if (m_pieceTeam == 1)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "king_bl.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "king_bl.png");
			return;
		}
	}
	else if (m_pieceTeam == 2)
	{
		pieceTexture = IMG_LoadTexture(Renderer, (DIRECTORY + "king.png").c_str());
		if (!pieceTexture)
		{
			Chess::MissingTexture(false, "king.png");
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
King::~King()
{

}