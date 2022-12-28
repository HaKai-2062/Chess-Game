#include "Knight.h"

Knight::Knight(SDL_Renderer * m_Renderer, int m_pieceTeam, int m_XPos, int m_YPos)
	: Piece(m_Renderer, KNIGHT, m_pieceTeam, m_XPos, m_YPos)
{

}

Knight::~Knight()
{
	SDL_DestroyTexture(m_pieceTexture);
}
