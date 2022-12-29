#include <iostream>
#include "Renderer.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

/*
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
|-|-|-|-|-|-|-|-|
*/

void MouseButtonPressed(SDL_Renderer*, bool& hasRenderedPossMove, const SDL_Event&, SDL_Texture*, SDL_Window*, const SDL_Rect&);
void FreeAllHeapMemory();

//must declare it here because cant really return it
Piece** boardPosition = new Piece * [64];

void Chess::MainRenderer()
{
	bool gameQuit = false;
	bool hasRenderedPossMoves = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	
	SDL_Surface* Icon = IMG_Load((DIRECTORY + "icon.ico").c_str());
	SDL_Window* Window = SDL_CreateWindow("Chess The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowIcon(Window, Icon);

	//ChessBoard
	SDL_Texture* Image = IMG_LoadTexture(Renderer, (DIRECTORY+"chessboard.png").c_str());
	if (!Image)
	{
		MissingTexture(gameQuit, "chessboard.png");
		gameQuit = true;
		return;
	}
	if (!Icon)
	{
		MissingTexture(gameQuit, "icon.ico");
		gameQuit = true;
		return;
	}

	//Centering Image
	SDL_Rect rectangle{};
	rectangle.w = 0;
	rectangle.h = 0;
	SDL_QueryTexture(Image, nullptr, nullptr, &rectangle.w, &rectangle.h);
	rectangle.x = (WIDTH - rectangle.w) / 2;
	rectangle.y = (HEIGHT - rectangle.h) / 2;

	bool initialRun = true;
	SDL_Event gameEvent;
	while (!gameQuit)
	{
		if (SDL_WaitEvent(&gameEvent))
		{
			if (gameEvent.type == SDL_QUIT)
				break;
			else if (gameEvent.key.keysym.sym == SDLK_ESCAPE)
				break;

			if (initialRun)
			{
				SDL_RenderClear(Renderer);
				SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
				initialRun = false;
				Chess::Init(Renderer);
				SDL_RenderPresent(Renderer);
				SDL_UpdateWindowSurface(Window);
			}

			if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
				MouseButtonPressed(Renderer, hasRenderedPossMoves, gameEvent, Image, Window, rectangle);
				SDL_RenderPresent(Renderer);
				SDL_UpdateWindowSurface(Window);
			}
			/*
			else
			{
				Chess::RenderAllPiece(Renderer, boardPosition);
			}*/

		}
		SDL_Delay(10);
	}

	//Freeing resources
	FreeAllHeapMemory();

	SDL_DestroyTexture(Image);	
	SDL_FreeSurface(Icon);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return;
}

void Chess::Init(SDL_Renderer* Renderer)
{
	//BLACK PAWN
	Pawn* pawn1 = new Pawn(Renderer, false, 0, 1);
	Pawn* pawn2 = new Pawn(Renderer, false, 1, 1);
	Pawn* pawn3 = new Pawn(Renderer, false, 2, 1);
	Pawn* pawn4 = new Pawn(Renderer, false, 3, 1);
	Pawn* pawn5 = new Pawn(Renderer, false, 4, 1);
	Pawn* pawn6 = new Pawn(Renderer, false, 5, 1);
	Pawn* pawn7 = new Pawn(Renderer, false, 6, 1);
	Pawn* pawn8 = new Pawn(Renderer, false, 7, 1);

	//WHITE PAWN
	Pawn* pawn9 = new Pawn(Renderer, true, 0, 6);
	Pawn* pawn10 = new Pawn(Renderer, true, 1, 6);
	Pawn* pawn11 = new Pawn(Renderer, true, 2, 6);
	Pawn* pawn12 = new Pawn(Renderer, true, 3, 6);
	Pawn* pawn13 = new Pawn(Renderer, true, 4, 6);
	Pawn* pawn14 = new Pawn(Renderer, true, 5, 6);
	Pawn* pawn15 = new Pawn(Renderer, true, 6, 6);
	Pawn* pawn16 = new Pawn(Renderer, true, 7, 6);

	//BLACK KNIGHT
	Knight* knight1 = new Knight(Renderer, false, 1, 0);
	Knight* knight2 = new Knight(Renderer, false, 6, 0);

	//WHITE KNIGHT		  
	Knight* knight3 = new Knight(Renderer, true, 1, 7);
	Knight* knight4 = new Knight(Renderer, true, 6, 7);

	//BLACK BISHOP
	Bishop* bishop1 = new Bishop(Renderer, false, 2, 0);
	Bishop* bishop2 = new Bishop(Renderer, false, 5, 0);

	//WHITE BISHOP
	Bishop* bishop3 = new Bishop(Renderer, true, 2, 7);
	Bishop* bishop4 = new Bishop(Renderer, true, 5, 7);

	//BLACK ROOK
	Rook* rook1 = new Rook(Renderer, false, 0, 0);
	Rook* rook2 = new Rook(Renderer, false, 7, 0);

	//WHITE ROOK
	Rook* rook3 = new Rook(Renderer, true, 0, 7);
	Rook* rook4 = new Rook(Renderer, true, 7, 7);

	//BLACK QUEEN
	Queen* queen1 = new Queen(Renderer, false, 3, 0);

	//WHITE QUEEN
	Queen* queen2 = new Queen(Renderer, true, 3, 7);

	//BLACK KING 
	King* king1 = new King(Renderer, false, 4,0);

	//WHITE KING
	King* king2 = new King(Renderer, true, 4,7);

	//set 16-47 to nullptr
	for (int i = 0; i < 64; i++)
	{
		boardPosition[i] = nullptr;
	}


	//------------X + (Y*W)------------//
	boardPosition[0 + (1*8)] = pawn1;
	boardPosition[1 + (1*8)] = pawn2;
	boardPosition[2 + (1*8)] = pawn3;
	boardPosition[3 + (1*8)] = pawn4;
	boardPosition[4 + (1*8)] = pawn5;
	boardPosition[5 + (1*8)] = pawn6;
	boardPosition[6 + (1*8)] = pawn7;
	boardPosition[7 + (1*8)] = pawn8;
	
	boardPosition[0 + (6*8)] = pawn9;
	boardPosition[1 + (6*8)] = pawn10;
	boardPosition[2 + (6*8)] = pawn11;
	boardPosition[3 + (6*8)] = pawn12;
	boardPosition[4 + (6*8)] = pawn13;
	boardPosition[5 + (6*8)] = pawn14;
	boardPosition[6 + (6*8)] = pawn15;
	boardPosition[7 + (6*8)] = pawn16;

	boardPosition[1 + (0*8)] = knight1;
	boardPosition[6 + (0*8)] = knight2;

	boardPosition[1 + (7*8)] = knight3;
	boardPosition[6 + (7*8)] = knight4;

	boardPosition[2 + (0*8)] = bishop1;
	boardPosition[5 + (0*8)] = bishop2;

	boardPosition[2 + (7*8)] = bishop3;
	boardPosition[5 + (7*8)] = bishop4;

	boardPosition[0 + (0*8)] = rook1;
	boardPosition[7 + (0*8)] = rook2;

	boardPosition[0 + (7*8)] = rook3;
	boardPosition[7 + (7*8)] = rook4;

	boardPosition[3 + (0*8)] = queen1;

	boardPosition[3 + (7*8)] = queen2;

	boardPosition[4 + (0*8)] = king1;

	boardPosition[4 + (7*8)] = king2;

}

Piece** Chess::GetBoardPos()
{
	return boardPosition;
}


void Chess::RenderAllPiece(SDL_Renderer* Renderer)
{
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (boardPosition[i + (j * 8)])
			{
				boardPosition[i + (j * 8)]->RenderThePiece(Renderer, boardPosition[i + (j * 8)]->GetPieceType(), boardPosition[i + (j * 8)]->GetPieceTeam(), boardPosition[i + (j * 8)]->GetPieceX(), boardPosition[i + (j * 8)]->GetPieceY());
			}
		}
	}
}


void MouseButtonPressed(SDL_Renderer* Renderer, bool& hasRenderedPossMoves, const SDL_Event& gameEvent, SDL_Texture* Image, SDL_Window* Window, const SDL_Rect& rectangle)
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int xBlockStart = (WIDTH / 8) * x;
			//start of next block is end of first
			int xBlockEnd = (WIDTH / 8) * (x + 1);

			int yBlockStart = (HEIGHT / 8) * y;
			int yBlockEnd = (HEIGHT / 8) * (y + 1);

			//if Moves havent been rendered yet then render them
			if (gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
			{
				if (boardPosition[x + (y * 8)])
				{
					//Clear and Rerender pieces if they had any renderedPossMoves before
					SDL_RenderClear(Renderer);
					SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
					Chess::RenderAllPiece(Renderer);
					SDL_RenderPresent(Renderer);
					SDL_UpdateWindowSurface(Window);

					//This is set to Pawn only to avoid bugs atm
					if (boardPosition[x + (y * 8)]->GetPieceType() == 0)
						hasRenderedPossMoves = true;

					//White Turn
					if (currentTurn == boardPosition[x + (y * 8)]->GetPieceTeam())
						boardPosition[x + (y * 8)]->RenderPossibleMoves(Renderer, 1, x, y);
					//Black Turn
					else
						boardPosition[x + (y * 8)]->RenderPossibleMoves(Renderer, 0, x, y);

					//break out of both loops
					y = 8;
					break;
				}
				else if (hasRenderedPossMoves)
				{
					//Clear and Rerender pieces if they had any renderedPossMoves before
					SDL_RenderClear(Renderer);
					SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
					Chess::RenderAllPiece(Renderer);
					SDL_RenderPresent(Renderer);
					SDL_UpdateWindowSurface(Window);
					hasRenderedPossMoves = false;

					//break out of both loops
					y = 8;
					break;
				}
				//std::cout << "MY NAME IS " << boardPosition[x][y]->GetPieceType()<<" from " << boardPosition[x][y]->GetPieceX()<<"," << boardPosition[x][y]->GetPieceY()<< std::endl;
			}
		}
	}
}

void FreeAllHeapMemory()
{
	for (int i = 0; i < 64; i++)
	{
		//piece object deleted
		delete boardPosition[i];
	}
	delete[] boardPosition;
}

void Chess::MissingTexture(bool gameQuit, std::string filename)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing Textures", ("Could not find " + filename + "\nMake sure you have \"Resource Files\" folder\nand all the required textures").c_str(), NULL);
	gameQuit = true;
	return;
}