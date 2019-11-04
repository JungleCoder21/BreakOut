#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <SDL2/SDL.h>
#include "textRenderer.h"
#include "Globals.h"
class Score{
	public:
		Score(textRenderer *renderText);
		textRenderer* renderText = NULL;
		int scoreX = Globals::SCREEN_WIDTH/2 - Globals::txtMsgW/2;
		int scoreY = 5;
		SDL_Color black = {0,0,0};
		int score = 0;
		std::string scoreString[6];
		std::string scor = "";
		const char* scoreChar;  //why can we modify if it is const?
		void showScore();
		void setScore(int score);
		int getScore();
		void update();
		void draw();
};


#endif
