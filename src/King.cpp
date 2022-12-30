#include "King.h"
#include <iostream>
King::King(SDL_Renderer* m_Renderer, bool m_pieceTeam, float m_XPos, float m_YPos)
	: Piece(m_Renderer, KING, m_pieceTeam, m_XPos, m_YPos)
{

}

King::~King()
{
	SDL_DestroyTexture(this->GetTexture());
}
