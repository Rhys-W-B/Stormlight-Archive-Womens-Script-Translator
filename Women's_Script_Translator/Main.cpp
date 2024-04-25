#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <Windows.h>

void renderSVG(const std::string& filename, SDL_Renderer* renderer,int x,int y, float scale) {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (!surface) {
		std::cerr << "Failed to load SVG image: " << IMG_GetError() << std::endl;
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create texture from SVG image: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return;
	}
	//SDL_SetTextureColorMod(texture, 255, 0, 0);
	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_Rect destRect = { x, y, surface->w*scale, surface->h*scale };
	
	SDL_RenderCopy(renderer, texture, NULL, &destRect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

int getCharWidth(const std::string& filename, float scale) {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (!surface) {
		std::cerr << "Failed to load SVG image: " << IMG_GetError() << std::endl;
		return 0;
	}
	return surface->w * scale;
}

void backSpace(int width, SDL_Renderer* renderer,int x,int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = 80;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	auto _SCREENWIDTH = DM.w/2;
	auto _SCREENHEIGHT = DM.h/2;

	SDL_Window* window = SDL_CreateWindow("Women's Script Translator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _SCREENWIDTH, _SCREENHEIGHT, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		std::cout << "SDL Window could not be created!!!" <<std::endl;
		exit(0);
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		std::cout << "SDL_GL context could not be created!!!" << std::endl;
		exit(0);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}


	bool run = true;

	std::string svgFile = "../Women's Script/A.svg"; // Replace with your SVG file path

	std::map<int, std::string> svgFilesMap;
	
	svgFilesMap[97] = "../Women's Script/A.svg";
	svgFilesMap[98] = "../Women's Script/B.svg";
	svgFilesMap[99] = "../Women's Script/CH.svg";
	svgFilesMap[100] = "../Women's Script/D.svg";
	svgFilesMap[101] = "../Women's Script/E.svg";
	svgFilesMap[102] = "../Women's Script/F.svg";
	svgFilesMap[103] = "../Women's Script/G.svg";
	svgFilesMap[104] = "../Women's Script/H.svg";
	svgFilesMap[105] = "../Women's Script/I.svg";
	svgFilesMap[106] = "../Women's Script/J.svg";
	svgFilesMap[107] = "../Women's Script/K.svg";
	svgFilesMap[108] = "../Women's Script/L.svg";
	svgFilesMap[109] = "../Women's Script/M.svg";
	svgFilesMap[110] = "../Women's Script/N.svg";
	svgFilesMap[111] = "../Women's Script/O.svg";
	svgFilesMap[112] = "../Women's Script/P.svg";
	svgFilesMap[114] = "../Women's Script/R.svg";
	svgFilesMap[115] = "../Women's Script/S.svg";
	svgFilesMap[200] = "../Women's Script/SH.svg";
	svgFilesMap[116] = "../Women's Script/T.svg";
	svgFilesMap[201] = "../Women's Script/TH.svg";
	svgFilesMap[117] = "../Women's Script/U.svg";
	svgFilesMap[118] = "../Women's Script/V.svg";
	svgFilesMap[121] = "../Women's Script/Y.svg";
	svgFilesMap[122] = "../Women's Script/Z.svg";
	svgFilesMap[91] = "../Women's Script/][.svg";

	//complex Characters: CH, SH, TH
	int x = 0;
	int y = 0;

	int prevChar = 0;
	int curchar = 0;

	int pos = -1;
	std::vector<int> widths;
	std::vector<int> xValues;

	bool compound = false;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	std::cout << "welcome to my women's script translator!\ncurrently, this only has very basic text editor support.\nall that means is no mouse or cursor interaction.\nyou can see what you're typing in english below vv\n";
	
	while(run){
		SDL_RenderPresent(renderer);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:																//stops program when X is clicked in window
				run = false;
				break;
			case SDL_KEYDOWN:															//keyboard input detection
				if (svgFilesMap.count(event.key.keysym.sym)) {							//ensures data only updated on valid character input
					pos++;
					svgFile = svgFilesMap[event.key.keysym.sym];						//grab SVG file path from map using keycode of keyboard input 
					curchar = event.key.keysym.sym;										//store keycode value in curchar
					float scale = 1;													//determines scale of text
					
					widths.push_back(getCharWidth(svgFilesMap[curchar], scale));		//add width of character to vector
					if (compound && event.key.keysym.sym == 104 && prevChar == 116) {	//TH compound character
						pos--;
						x -= getCharWidth(svgFilesMap[116], scale);
						widths.pop_back();
						widths.at(widths.size() - 1) = getCharWidth(svgFilesMap[201], scale);
						backSpace(getCharWidth(svgFilesMap[116], scale),renderer, x, y);
						svgFile = svgFilesMap[201];
						prevChar = 201;
						curchar = 201;
						compound = false;												//git test
					}
					if (compound && event.key.keysym.sym == 104 && prevChar == 115) {	//SH compound character
						pos--;
						x -= getCharWidth(svgFilesMap[115], scale);
						widths.pop_back();
						widths.at(widths.size() - 1) = getCharWidth(svgFilesMap[200], scale);
						backSpace(getCharWidth(svgFilesMap[115], scale), renderer, x, y);
						svgFile = svgFilesMap[200];
						prevChar = 200;
						curchar = 200;
						compound = false;
					}
					if (event.key.keysym.sym == 115 || event.key.keysym.sym == 116) {	//compound character elegibility detector
						compound = true;
					}
					renderSVG(svgFile, renderer, x, y, scale);							//renders character
					x += getCharWidth(svgFile, scale);									//modify's "cursor" placement
					if (curchar != 201 && curchar != 200) {								//ensures prevChar = curchar only when curchar is not a compound character
						prevChar = curchar;
					}
				}
				if (event.key.keysym.sym == 8 && widths.size()>0) {						//handles backspace
					if (x <= 0 && y>=0) {
						x = xValues.back();
						y -= 80;
						xValues.pop_back();
					}
					//std::cout << "backspaced width: " << widths.at(pos) << std::endl;	//comment for build
					std::cout << "\b \b";
					x -= widths.at(pos);
					backSpace(widths.at(pos), renderer, x, y);
					widths.pop_back();
					pos--;
				}
				if (event.key.keysym.sym == 13 || x >= _SCREENWIDTH - 50) {				//handles line feed
					xValues.push_back(x);
					y += 80;
					x = 0;
					std::cout << std::endl;												//uncomment for build
				}
				if (event.key.keysym.sym == 32) {
					backSpace(10, renderer, x, y);
					pos++;
					x += 10;														//FIX THIS
					widths.push_back(10);
				}
				//std::cout << event.key.keysym.sym << std::endl;
				//std::cout << "widths vector size: " << widths.size() << std::endl;	//debugging information vvv (comment for build)
				//std::cout << "widths: ";
				//for (int i = 0; i < widths.size(); i++) {
				//	std::cout << widths.at(i) << ", ";
				//}
				//std::cout << std::endl;
				//std::cout << "position: " << pos << std::endl;
				if (event.key.keysym.sym != 8) {
					std::cout << (char)event.key.keysym.sym;							//uncomment for build
				}
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}