#include "Knight.h"

Knight::Knight(SDL_Renderer * m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KNIGHT, m_pieceTeam, m_XPos, m_YPos)
{

}

Knight::~Knight()
{
	SDL_DestroyTexture(this->GetTexture());
}
