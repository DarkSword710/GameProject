#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>	//Needs to be included to use images
#include <SDL_ttf.h>	//Needs to be included to use text and fonts
#include <SDL_mixer.h>	//Needs to be included to use audio tracks and SFX
#include <iostream>
#include <time.h> //Needed for deltaTime

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

int main(int, char*[]) {
#pragma region Inizialización
	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect bgRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	/*SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect playerRect{ 0, 0, 350, 189 };*/
	SDL_Rect playerTarget{ 0, 0, 100, 100 };

		// --- Animated Sprite ---
	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../../res/img/sp01.png") };
	SDL_Rect playerRect, playerPosition;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 6;
	frameHeight = textHeight / 1;
	playerPosition.x = playerPosition.y = 0;
	playerRect.x = playerRect.y = 0;
	playerPosition.h = playerRect.h = frameHeight;
	playerPosition.w = playerRect.w = frameWidth;
	int frameTime = 0;

	// --- TEXT ---
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "My first SDL game", SDL_Color{ 255, 150, 0, 255 }) };
	if (tmpSurf == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text surface";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 100, 50, tmpSurf->w, tmpSurf->h };
	SDL_FreeSurface(tmpSurf);

	SDL_Surface *tmpSurfHover{ TTF_RenderText_Blended(font, "My first SDL game", SDL_Color{ 216, 255, 202, 255 }) };
	SDL_Texture *textTextureHover{ SDL_CreateTextureFromSurface(renderer, tmpSurfHover) };
	SDL_FreeSurface(tmpSurf);
	SDL_FreeSurface(tmpSurfHover);
	TTF_CloseFont(font);

	// --- AUDIO ---
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags) & mixFlags)) throw "Error: SDL_mixer init";
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Unable to initialize SDL_mixer audio systems";
	}
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!soundtrack) throw "Unable to load the Mix_Music soundtrack";
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_PlayMusic(soundtrack, -1);

	// --- TIME ---
	clock_t lastTime = clock();
	float timeDown = 10.;
	float deltaTime = 0;

	/*
	Mix_PlayingMusic();Boolean
	Mix_PauseMusic();Pause
	Mix_PausedMusic();Boolean
	Mix_HaltMusic();
	Mix_ResumeMusic();

	StarUML;
	*/
#pragma endregion

#pragma region GameLoop
	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	bool mouseClicked = false;
	bool hover = false;
	while (isRunning){

	#pragma region HandleEvents
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			case SDL_MOUSEMOTION:
				playerTarget.x = event.motion.x;
				playerTarget.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseClicked = true;
				break;
			default:;
			}
		}
	#pragma endregion

	#pragma region Update
		// UPDATE
		deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;
		std::cout << timeDown << std::endl;

		frameTime++;
		if (FPS / frameTime <= 9) {
			frameTime = 0;
			playerRect.x += frameWidth;
			if (playerRect.x >= textWidth) {
				playerRect.x = 0;
			}
		}
		//playerRect.x += (playerTarget.x - playerRect.x) / 10;
		//playerRect.y += (playerTarget.y - playerRect.y) / 10;
		if (playerTarget.x >= textRect.x && playerTarget.y >= textRect.y && playerTarget.x <= textRect.x + textRect.w && playerTarget.y <= textRect.y + textRect.h) {
			hover = true;
			if (mouseClicked) {
				if (Mix_PlayingMusic()) {
					Mix_HaltMusic();
				}
				else {
					Mix_PlayMusic(soundtrack, -1);
				}
			}
		}
		else {
			hover = false;
		}
		mouseClicked = false;
	#pragma endregion

	#pragma region Draw
		// DRAW
			//Background
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		//SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
		if (hover) {
			SDL_RenderCopy(renderer, textTextureHover, nullptr, &textRect);
		}
		else {
			SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		}
		
		
			//Animated Sprite
		SDL_RenderCopy(renderer, playerTexture, &playerRect, &playerPosition);
		SDL_RenderPresent(renderer);
	#pragma endregion
	}
#pragma endregion

#pragma region Destroy
	// --- DESTROY ---
	Mix_CloseAudio();
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
#pragma endregion

#pragma region Quit
	// --- QUIT ---
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
#pragma endregion
}

/*
enum class GameState { PLAY, EXIT, MENU };
// -- GAME LOOP ---
SDL_event event;
GameState gamestat = GameState::MENU;
while (gamestat != GameState::EXIT) {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			gamestat = GameState::EXIT;
			break;
		}
	}
}

//Game_Loop//
switch (gamestat)
{
	case GameState::PLAY:
		//EventHandlePlay();
		//UpdatePlay();
		//DrawPlay();
		break;
	case GameState::EXIT:
		break;
	case GameState::MENU:
		//EventHandleMenu();
		//UpdateMenu();
		//DrawMenu();
	break;
}
//Game_Loop//
*/