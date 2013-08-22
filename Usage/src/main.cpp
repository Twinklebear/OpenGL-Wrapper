#if defined(_MSC_VER)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <oglw.h>

int main(int argc, char **argv){
	oglw::SDL sdl(SDL_INIT_EVERYTHING);
	oglw::Window window("OGLW Test", 640, 480);
	
	SDL_Delay(500);

	return 0;
}
