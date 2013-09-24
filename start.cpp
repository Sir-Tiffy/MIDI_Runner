#include "stdafx.h"
#include "Game.h"
#include "PopupMsg.h"

int main(int argc, char* argv){
	try{
		Game::Init();
		Game::Begin();
	} catch (exception &e){
		PopupError(e.what());
		return 1;
	}
	return 0;
}