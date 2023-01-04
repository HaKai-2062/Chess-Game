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
	std::vector<int> firstEnemyPieceMoves;		//to check if they can check our king
	std::vector<int> secondEnemyPieceMoves;		//to check if they can check our king if we made a move
	std::vector<int> thirdEnemyPieceMoves;		//to check if there are any other pieces that may check our king if we make a move
	
	std::vector<int> firstfriendlyPieceMoves;	//to check all the moves we can make
	
	std::vector<int> legitMoves;				//to push only the legit moves

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
				firstEnemyPieceMoves.clear();
				firstEnemyPieceMoves = boardPosition[i]->CalculatePossibleMoves();
				
				//second loop checks attacks of enemy pieces against our king
				for (int j = 0; j < firstEnemyPieceMoves.size(); j++)
				{
					if ((this->GetPieceTeam() && firstEnemyPieceMoves[j] == Piece::whiteKingPos) || (!this->GetPieceTeam() && firstEnemyPieceMoves[j] == Piece::blackKingPos))
					{
						legitMoves.clear();
						firstfriendlyPieceMoves.clear();
						firstfriendlyPieceMoves = this->CalculatePossibleMoves();
						
						//third loop will go through all of our piece's moves
						for (int k = 0; k < firstfriendlyPieceMoves.size(); k++)
						{
							int x = static_cast<int>(this->GetPieceX());
							int y = static_cast<int>(this->GetPieceY());
							bool isLegelMove = true;

							//if enemy piece is present
							//TDL: calculate moves when i take enemy pawn and need to make some workaround here
							if (boardPosition[firstfriendlyPieceMoves[k]])
							{
								continue;
							}
							else
							{
								//temporarily save the position of our piece
								boardPosition[firstfriendlyPieceMoves[k]] = boardPosition[x + (y * 8)];
								boardPosition[x + (y * 8)] = nullptr;
							}

							//check if our king is still in check after making this move
							secondEnemyPieceMoves.clear();
							secondEnemyPieceMoves = boardPosition[i]->CalculatePossibleMoves();

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
								else if (this->GetPieceType() == KING && secondEnemyPieceMoves[l] == firstfriendlyPieceMoves[k])
								{
									isLegelMove = false;
									break;
								}
								//TDL: fifth loop to check for other enemies that might be attacking my king
								for (int m = 0; m < 64; m++)
								{
									if (i != m && boardPosition[m] && ((Piece::isWhiteInCheck && boardPosition[m] && !boardPosition[m]->GetPieceTeam() && this->GetPieceTeam()) || (Piece::isBlackInCheck && boardPosition[m] && boardPosition[m]->GetPieceTeam() && !this->GetPieceTeam())))
									{
										thirdEnemyPieceMoves.clear();
										thirdEnemyPieceMoves = boardPosition[m]->CalculatePossibleMoves();
										for (int n = 0; n < thirdEnemyPieceMoves.size(); n++)
										{
											if ((this->GetPieceTeam() && thirdEnemyPieceMoves[n] == Piece::whiteKingPos) || (!this->GetPieceTeam() && thirdEnemyPieceMoves[n] == Piece::blackKingPos))
											{
												isLegelMove = false;
												m = 64;
												l = (int)secondEnemyPieceMoves.size();
												break;
											}
										}
									}
								}
							}

							if (isLegelMove)
								legitMoves.push_back(firstfriendlyPieceMoves[k]);

							//restore the temp position that we set
							boardPosition[x + (y * 8)] = boardPosition[firstfriendlyPieceMoves[k]];
							boardPosition[firstfriendlyPieceMoves[k]] = nullptr;
						}
						//no need to search for further enemy attacks
						break;
					}
				}
			}
		}
		this->PossibleMovesVector().clear();
		this->PossibleMovesVector() = legitMoves;
		firstEnemyPieceMoves.clear();
		secondEnemyPieceMoves.clear();
		thirdEnemyPieceMoves.clear();
		
		firstfriendlyPieceMoves.clear();

		legitMoves.clear();
	}
	
	//can my move put my king under check?
	else
	{
		legitMoves.clear();
		firstfriendlyPieceMoves.clear();
		firstfriendlyPieceMoves = this->CalculatePossibleMoves();
		for (int i = 0; i < firstfriendlyPieceMoves.size(); i++)
		{
			int x = static_cast<int>(this->GetPieceX());
			int y = static_cast<int>(this->GetPieceY());
			bool isLegalMove = true;

			//if enemy piece is present
			if (boardPosition[firstfriendlyPieceMoves[i]])
				tempPiece = boardPosition[firstfriendlyPieceMoves[i]];

			//temporarily save the position of our piece
			boardPosition[firstfriendlyPieceMoves[i]] = boardPosition[x + (y * 8)];
			boardPosition[x + (y * 8)] = nullptr;

			//loop through enemy pieces and see if they can take our piece
			for (int j = 0; j < 64; j++)
			{
				if (boardPosition[j] && boardPosition[j]->GetPieceTeam() != this->GetPieceTeam())
				{
					//TDL:clearing this can create bugs maybe when being cornered by multiple pieces?
					firstEnemyPieceMoves.clear();
					firstEnemyPieceMoves = boardPosition[j]->CalculatePossibleMoves();
					//chheck if enemy has our king under check
					for (int k = 0; k < firstEnemyPieceMoves.size(); k++)
					{
						if (this->GetPieceType() != KING && ((this->GetPieceTeam() && firstEnemyPieceMoves[k] == Piece::whiteKingPos) || (!this->GetPieceTeam() && firstEnemyPieceMoves[k] == Piece::blackKingPos)))
						{
							isLegalMove = false;
							break;
						}
						//if we are king and our position is not in enemies' poss moves
						else if (this->GetPieceType() == KING && firstEnemyPieceMoves[k] == firstfriendlyPieceMoves[i])
						{
							isLegalMove = false;
							break;
						}
					}
				}
			}

			//so our choice led to our king under check?
			if (isLegalMove)
			{
				legitMoves.push_back(firstfriendlyPieceMoves[i]);
			}

			//restored the temporary position stored
			boardPosition[x + (y * 8)] = boardPosition[firstfriendlyPieceMoves[i]];
			if (tempPiece)
				boardPosition[firstfriendlyPieceMoves[i]] = tempPiece;
			else
				boardPosition[firstfriendlyPieceMoves[i]] = nullptr;
			tempPiece = nullptr;
		}
		this->PossibleMovesVector().clear();
		this->PossibleMovesVector() = legitMoves;
		firstfriendlyPieceMoves.clear();
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