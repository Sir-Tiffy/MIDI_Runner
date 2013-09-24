#pragma once
#include "StdAfx.h"
#include "State.h"

namespace Game{
	void ChangeState(State* state, bool destroyOld = true);
	//void OpenWindow(int width=800, int height=600, const char* title="SORT DISPLASY", const int FSAA=0);
	void Init();
	void Begin();
	void Exit();
	void TryLoadSong();
	//float mousePosX, mousePosY;

	
	const string InitialSong("menuSong.mid");
}