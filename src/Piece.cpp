#include "Piece.h"
#include <iostream>
#include <cmath>

#include "C_Files/bishop.c"
#include "C_Files/king.c"
#include "C_Files/knight.c"
#include "C_Files/pawn.c"
#include "C_Files/queen.c"
#include "C_Files/rook.c"

#include "C_Files/bishop_bl.c"
#include "C_Files/king_bl.c"
#include "C_Files/knight_bl.c"
#include "C_Files/pawn_bl.c"
#include "C_Files/queen_bl.c"
#include "C_Files/rook_bl.c"

Piece::Piece(SDL_Renderer* Renderer, PieceType pieceType, bool pieceTeam, float xPos, float yPos)
	:m_Renderer(Renderer), m_pieceType(pieceType), m_pieceTeam(pieceTeam), m_XPos(xPos), m_YPos(yPos)
{
	RenderThePiece(Renderer, pieceType, pieceTeam, xPos, yPos);
}
Piece::~Piece()
{
	SDL_DestroyTexture(this->GetPieceTexture());
	SDL_FreeRW(this->GetPieceRW());
	SDL_FreeSurface(this->GetPieceSurface());
	this->PossibleMovesVector().clear();
}

bool Piece::isBlackInCheck = false;
bool Piece::isWhiteInCheck = false;
int Piece::blackKingPos = 4;
int Piece::whiteKingPos = 60;

void Piece::RenderThePiece(SDL_Renderer* Renderer, const PieceType& pieceType, const bool& pieceTeam, const float& xPos, const float& yPos)
{
	//Rendering Piece
	std::string fileName = "";
	switch (this->m_pieceType)
	{
	case PAWN:
		fileName = "pawn";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)pawn_bl_png, sizeof(pawn_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)pawn_png, sizeof(pawn_png));
		break;
	case KNIGHT:
		fileName = "knight";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)knight_bl_png, sizeof(knight_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)knight_png, sizeof(knight_png));
		break;
	case BISHOP:
		fileName = "bishop";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)bishop_bl_png, sizeof(bishop_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)bishop_png, sizeof(bishop_png));
		break;
	case ROOK:
		fileName = "rook";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)rook_bl_png, sizeof(rook_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)rook_png, sizeof(rook_png));
		break;
	case QUEEN:
		fileName = "queen";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)queen_bl_png, sizeof(queen_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)queen_png, sizeof(queen_png));
		break;
	case KING:
		fileName = "king";
		if (!this->m_pieceTeam)
			this->m_pieceRW = SDL_RWFromMem((void*)king_bl_png, sizeof(king_bl_png));
		else
			this->m_pieceRW = SDL_RWFromMem((void*)king_png, sizeof(king_png));
		break;
	default:
		break;
	}
	//SDL_Surface* is set here
	this->m_pieceSurface = IMG_Load_RW(this->m_pieceRW, 1);
	if (!this->m_pieceSurface)
	{
		std::cout << "surf error\n";
		if (!this->m_pieceTeam)
			Chess::MissingTexture(false, fileName + "_bl.png");
		else
			Chess::MissingTexture(false, fileName + ".png");
		return;
	}
	//SDL_Texture* is set here
	this->m_pieceTexture = SDL_CreateTextureFromSurface(Renderer, this->m_pieceSurface);
	if (!this->m_pieceTexture)
	{
		std::cout << "texture error\n";
		if (!this->m_pieceTeam)
			Chess::MissingTexture(false, fileName + "_bl.png");
		else
			Chess::MissingTexture(false, fileName + ".png");
		return;
	}

	SDL_Rect pieceRect{};
	pieceRect.w = WIDTH / 8;
	pieceRect.h = HEIGHT / 8;
	pieceRect.x = (int)(m_XPos * pieceRect.w);
	pieceRect.y = (int)(m_YPos * pieceRect.h);

	SDL_RenderCopy(Renderer, m_pieceTexture, nullptr, &pieceRect);
}

void Piece::MoveThePiece(SDL_Renderer* Renderer, int boardPositionToMove, bool& currentTurn)
{
	if (currentTurn != this->GetPieceTeam())
		return;

	this->m_hasMoved = true;

	int xStart = static_cast<int>(this->GetPieceX());
	int yStart = static_cast<int>(this->GetPieceY());

	int xEnd = Chess::GetBlockX(boardPositionToMove);
	int yEnd = Chess::GetBlockY(boardPositionToMove);

	// Set the number of steps for the animation
	float steps = 16.0f;
	
	// Calculate the x and y step sizes for the animation
	float xStep = (xEnd - xStart) / steps;
	float yStep = (yEnd - yStart) / steps;

	for (int i = 0; i < steps; i++)
	{
		this->AddToX(xStep);
		this->AddToY(yStep);

		//The piece is rerenderer here
		SDL_RenderClear(Renderer);
		Chess::DestroyAllPieceTextures();
		Chess::DrawChessBoard(Renderer);
		Chess::RenderAllPiece(Renderer);
		SDL_RenderPresent(Renderer);
	}

	int startPosition = xStart + (yStart * 8);
	int endPosition = xEnd + (yEnd * 8);

	//if piece exists then destroy it
	if (boardPosition[endPosition])
		delete boardPosition[endPosition];
	//assign new position to the piece
	boardPosition[endPosition] = boardPosition[startPosition];
	//set prev piece position to null
	boardPosition[startPosition] = nullptr;

	//change the turn
	currentTurn = !currentTurn;

	//if king then set its position and set the kingInCheck flag
	Piece::SetKingVariables();
}

void Piece::RenderPossMovesBlock(SDL_Renderer* Renderer)
{
	//Render the possible Moves Box here
	for (int i = 0; i < this->PossibleMovesVector().size(); i++)
	{
		SDL_Rect temp1{ Chess::GetBlockX(this->PossibleMovesVector()[i]) * (WIDTH / 8), Chess::GetBlockY(this->PossibleMovesVector()[i]) * (HEIGHT / 8),  WIDTH / 8 , HEIGHT / 8 };
		//enemy piece found
		if (boardPosition[this->PossibleMovesVector()[i]] && boardPosition[this->PossibleMovesVector()[i]]->GetPieceTeam() != this->GetPieceTeam())
			SDL_SetRenderDrawColor(Renderer, 255, 20, 25, 96);
		//empty piece found
		else
			SDL_SetRenderDrawColor(Renderer, 0, 255, 25, 96);
		SDL_RenderFillRect(Renderer, &temp1);
	}
}

//ATM i only run away from piece that is checking me

void Piece::CalculateMovesForCheck()
{
	std::vector<int> firstEnemyPieceMoves;
	std::vector<int> secondEnemyPieceMoves;
	std::vector<int> thirdEnemyPieceMoves;
	std::vector<int> friendlyPieceMoves;
	Piece* tempPiece = nullptr;

	//sacrifice the piece to save our king
	if (Piece::isWhiteInCheck || Piece::isBlackInCheck)
	{
		//first loop checks all enemy pieces
		for (int i = 0; i < 64; i++)
		{
			//if enemy piece present
			if ((Piece::isWhiteInCheck && boardPosition[i] && !boardPosition[i]->GetPieceTeam() && this->GetPieceTeam()) || (Piece::isBlackInCheck && boardPosition[i] && boardPosition[i]->GetPieceTeam() && !this->GetPieceTeam()))
			{
				//second loop checks attacks of enemy pieces against our king
				firstEnemyPieceMoves.clear();
				boardPosition[i]->PossibleMovesVector().clear();
				firstEnemyPieceMoves = boardPosition[i]->CalculatePossibleMoves();
				for (int j = 0; j < firstEnemyPieceMoves.size(); j++)
				{
					if ((this->GetPieceTeam() && firstEnemyPieceMoves[j] == Piece::whiteKingPos) || (!this->GetPieceTeam() && firstEnemyPieceMoves[j] == Piece::blackKingPos))
					{
						friendlyPieceMoves.clear();
						//third loop will go through all of our piece's moves
						this->PossibleMovesVector().clear();
						this->CalculatePossibleMoves();
						for (int k = 0; k < this->PossibleMovesVector().size(); k++)
						{
							int x = static_cast<int>(this->GetPieceX());
							int y = static_cast<int>(this->GetPieceY());

							//if enemy piece is present
							//TDL: calculate moves when i take enemy pawn and need to make some workaround here
							if (boardPosition[this->PossibleMovesVector()[k]])
							{
								continue;
							}

							//temporarily save the position of our piece
							boardPosition[this->PossibleMovesVector()[k]] = boardPosition[x + (y * 8)];
							boardPosition[x + (y * 8)] = nullptr;

							//check if our king is still in check after making this move
							boardPosition[i]->PossibleMovesVector().clear();
							secondEnemyPieceMoves.clear();
							secondEnemyPieceMoves = boardPosition[i]->CalculatePossibleMoves();
							bool isLegelMove = true;
							
							//fourth loop will check through enemy piece's moves again and see if it can still check our king or not
							for (int l = 0; l < secondEnemyPieceMoves.size(); l++)
							{
								//if it can still check
								if (this->GetPieceType() != KING && ((this->GetPieceTeam() && secondEnemyPieceMoves[l] == Piece::whiteKingPos) || (!this->GetPieceTeam() && secondEnemyPieceMoves[l] == Piece::blackKingPos)))
								{
									isLegelMove = false;
									break;
								}

								//if we are king and our position is not in enemies' poss moves
								else if (this->GetPieceType() == KING && secondEnemyPieceMoves[l] == this->PossibleMovesVector()[k])
								{
									isLegelMove = false;
									break;
								}
								//TDL: fifth loop to check for other enemies that might be attacking my king
							}

							if (isLegelMove)
								friendlyPieceMoves.push_back(this->PossibleMovesVector()[k]);

							//restore the temp position that we set
							boardPosition[x + (y * 8)] = boardPosition[this->PossibleMovesVector()[k]];
							boardPosition[this->PossibleMovesVector()[k]] = nullptr;
						}
						//no need to search for further enemy attacks
						break;
					}
				}
			}
		}
		this->PossibleMovesVector().clear();
		this->PossibleMovesVector() = friendlyPieceMoves;
		firstEnemyPieceMoves.clear();
		secondEnemyPieceMoves.clear();
		friendlyPieceMoves.clear();
	}

	//can my move put my king under check?
	else
	{
		friendlyPieceMoves.clear();
		this->PossibleMovesVector().clear();
		this->CalculatePossibleMoves();
		for (int i = 0; i < this->PossibleMovesVector().size(); i++)
		{
			int x = static_cast<int>(this->GetPieceX());
			int y = static_cast<int>(this->GetPieceY());

			//if enemy piece is present
			if (boardPosition[this->PossibleMovesVector()[i]])
				tempPiece = boardPosition[this->PossibleMovesVector()[i]];

			//temporarily save the position of our piece
			boardPosition[this->PossibleMovesVector()[i]] = boardPosition[x + (y * 8)];
			boardPosition[x + (y * 8)] = nullptr;
			bool isLegalMove = true;

			//loop through enemy pieces and see if they can take our piece
			for (int j = 0; j < 64; j++)
			{
				if (boardPosition[j] && boardPosition[j]->GetPieceTeam() != this->GetPieceTeam())
				{
					//TDL:clearing this can create bugs maybe when being cornered by multiple pieces?
					boardPosition[j]->PossibleMovesVector().clear();
					boardPosition[j]->CalculatePossibleMoves();
					//chheck if enemy has our king under check
					for (int k = 0; k < boardPosition[j]->PossibleMovesVector().size(); k++)
					{
						if (this->GetPieceType() != KING && ((this->GetPieceTeam() && boardPosition[j]->PossibleMovesVector()[k] == Piece::whiteKingPos) || (!this->GetPieceTeam() && boardPosition[j]->PossibleMovesVector()[k] == Piece::blackKingPos))) 
						{
							isLegalMove = false;
							break;
						}
						//if we are king and our position is not in enemies' poss moves
						else if (this->GetPieceType() == KING && boardPosition[j]->PossibleMovesVector()[k] == this->PossibleMovesVector()[i])
						{
							isLegalMove = false;
							break;
						}
					}
					//delete moves calculated earlier
					boardPosition[j]->PossibleMovesVector().clear();
				}
			}

			//so our choice led to our king under check?
			if (isLegalMove)
			{
				friendlyPieceMoves.push_back(this->PossibleMovesVector()[i]);
			}

			//restored the temporary position stored
			boardPosition[x + (y * 8)] = boardPosition[this->PossibleMovesVector()[i]];
			if (tempPiece)
				boardPosition[this->PossibleMovesVector()[i]] = tempPiece;
			else
				boardPosition[this->PossibleMovesVector()[i]] = nullptr;
			tempPiece = nullptr;
		}
		this->PossibleMovesVector().clear();
		this->PossibleMovesVector() = friendlyPieceMoves;
		friendlyPieceMoves.clear();
	}
}

void Piece::SetKingVariables()
{
	//if "this" is king then save position
	if (this->GetPieceTeam() && this->GetPieceType() == KING)
		Piece::whiteKingPos = static_cast<int>(this->GetPieceX()) + static_cast<int>(this->GetPieceY() * 8);
	else if (!this->GetPieceTeam() && this->GetPieceType() == KING)
		Piece::blackKingPos = static_cast<int>(this->GetPieceX()) + static_cast<int>(this->GetPieceY() * 8);

	//save the vars as false after a move has been performed
	if (this->GetPieceTeam())
		Piece::isWhiteInCheck = false;
	else
		Piece::isBlackInCheck = false;

	//set check here by calculating enemy piece's next moves
	this->PossibleMovesVector().clear();
	this->CalculatePossibleMoves();
	for (int i = 0; i < this->PossibleMovesVector().size(); i++)
	{
		//white piece
		if (this->GetPieceTeam() && this->PossibleMovesVector()[i] == blackKingPos)
		{
			std::cout << "Black King Checked at: " << Piece::blackKingPos << std::endl;
			Piece::isBlackInCheck = true;
		}
		//black piece
		else if (!this->GetPieceTeam() && this->PossibleMovesVector()[i] == whiteKingPos)
		{
			std::cout << "White King Checked at: " << Piece::whiteKingPos << std::endl;
			Piece::isWhiteInCheck = true;
		}
	}
	this->PossibleMovesVector().clear();
}