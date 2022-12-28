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

void Chess::MainRenderer()
{
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
	while (!gameQuit)
	{
		SDL_Event gameEvent;
		if (SDL_WaitEvent(&gameEvent))
		{
			if (gameEvent.type == SDL_QUIT)
				break;
			else if (gameEvent.key.keysym.sym == SDLK_ESCAPE)
				break;
			else if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
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


						if (gameEvent.button.x > xBlockStart && gameEvent.button.x <= xBlockEnd && gameEvent.button.y > yBlockStart && gameEvent.button.y <= yBlockEnd)
						{ }
					}
				}
			}
			if (initialRun)
			{
				SDL_RenderClear(Renderer);
				SDL_RenderCopy(Renderer, Image, nullptr, &rectangle);
				initialRun = false;
				Chess::Init(Renderer);
				SDL_RenderPresent(Renderer);
			}
		}
		SDL_Delay(10);
	}
	//Freeing resources
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

}


void Chess::MissingTexture(bool gameQuit, std::string filename)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing Textures", ("Could not find "+filename+"\nMake sure you have \"Resource Files\" folder\nand all the required textures").c_str(), NULL);
	gameQuit = true;
	return;
}