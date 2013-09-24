#include "stdafx.h"
#include "GameState.h"
#include "MeshHandler.h"
#include "ChestNoteBlock.h"

void GameState::Render(){
	glm::mat4 temp = glm::translate(pv,glm::vec3(-2*songPos/songSpeed,0,0));
	for (auto i = vector<NoteBlock*>::iterator(visibleNoteIterator); i != noteBlocks.end() && (*i)->notePos < songPos+songSpeed*rightBoundary; i++){
		//if ((*i)->playing)
		(*i)->Render(temp, songPos, songSpeed);
	}
	NoteBlock::loaded = false;
	for (auto i = vector<NoteBlock*>::iterator(visibleNoteIterator); i != noteBlocks.end() && (*i)->notePos < songPos+songSpeed*rightBoundary; i++){
		if ((*i)->hasCoin && (*i)->coinScore == 1) (*i)->RenderCoin(temp, songPos);
	}
	NoteBlock::loaded = false;
	for (auto i = ChestNoteBlock::chestBlocksList.begin(); i != ChestNoteBlock::chestBlocksList.end(); i++){
		if ((*i)->hasCoin) ((ChestNoteBlock*)(*i))->RenderCoin(temp, songPos);
	}
	
	glUseProgram(0);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf((float*)&(projectionMatrix*glm::lookAt(glm::vec3(0,0,0),cameraLookVector,glm::vec3(0,1,0))));
	glScalef(512,512,512);
	glPushMatrix();
	glTranslatef(0,-cameraPosition.y/256,0);
	mountainLayer1.Render(&glm::vec3(3/32.0f,3/16.0f,1/16.0f),&glm::vec3(1/16.0f,128.0f/255,0), true);
	glPopMatrix();
	glScalef(1.1f,1.1f,1.1f);
	glTranslatef(0,-cameraPosition.y/384,0);
	mountainLayer2.Render(&glm::vec3(3/64.0f,3/32.0f,1/32.0f),&glm::vec3(1/32.0f,128.0f/512,0), false);

	glLoadIdentity();
	glMultMatrixf((float*)&projectionMatrix);
	glScalef(768,768,768);
	glBegin(GL_QUADS);
	glColor3f(0,0,53.0f/255);
	glVertex3f(-1,-1,-1);
	glVertex3f(1,-1,-1);

	glColor3f(0,0,0);
	glVertex3f(1,1,-1);
	glVertex3f(-1,1,-1);
	glEnd();

	glScalef(.9f,.9f,.9f);
	glTranslatef(0,-cameraPosition.y/512,0);
	glColor3f(1,1,1);
	const unsigned int NUM_STARS = 256;
	default_random_engine randGenerator;
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < NUM_STARS; i++){
		//float angle = (i/(NUM_STARS-1.0f)-.5f)*PI * 5/12 + PI;
		const float angle = (float)(((float)randGenerator()/randGenerator.max()-.5f)*PI * 5/12 + PI);
		glVertex3f((float)sin(angle),(float)randGenerator()/randGenerator.max(), (float)cos(angle));
	}
	glEnd();

	glPopMatrix();
}

enum moveDir{
	FORWARD = 0x80,
	BACK = 0x40,
	LEFT = 0x20,
	RIGHT = 0x10,
	UP = 0x8,
	DOWN = 0x4,
	BOOST = 0x2
};

void GameState::SongDone(double currentTime){
	//if (loop){
	//	Begin(currentTime);
	//} else {
		done = true;
	//}
}

void GameState::Update(double currentTime, double deltaTime){
	if (!running) return;

	songDelta = song->UpdatePlaySong(currentTime, deltaTime, paused);
	songPos += songDelta;
	#ifdef DEBUGCAMERA
		if (moveDir != 0 && debugCamera){
			glm::vec3 movement(0,0,0);
			if ((moveDir & FORWARD) == FORWARD) movement += cameraLookVector;
			if ((moveDir & BACK) == BACK) movement -= cameraLookVector;
			if ((moveDir & UP) == UP) movement.y += 1;
			if ((moveDir & DOWN) == DOWN) movement.y -= 1;
			if ((moveDir & LEFT) == LEFT) movement -= glm::cross(cameraLookVector,glm::vec3(0,1,0));
			if ((moveDir & RIGHT) == RIGHT) movement += glm::cross(cameraLookVector,glm::vec3(0,1,0));
			if (movement.x == 0 && movement.y == 0 && movement.z == 0) return;
			cameraPosition += glm::normalize(movement)*(float)(cameraSpeed*deltaTime*((moveDir&BOOST)==BOOST?4:1));
			RebuildMatrix();
		}
	#endif
	while (visibleNoteIterator != noteBlocks.end() && (*visibleNoteIterator)->notePos + (*visibleNoteIterator)->length*songSpeed < songPos - songSpeed*leftBoundary) visibleNoteIterator++;
	if (!done && song->done) SongDone(currentTime);
	/*for (auto i = noteBlocks.begin(); i != noteBlocks.end(); i++)
		(*i)->Update(currentTime, deltaTime, songPos, songSpeed);*/
	//print(songPos);
}

void GameState::MouseMove(float x, float y){
#ifdef DEBUGCAMERA
	if (debugCamera){
		double mouseSensitivity = 1.5;
		cameraLook += glm::vec2((.5-x)*mouseSensitivity,(y-.5)*mouseSensitivity);
		if (cameraLook.x < 0) cameraLook.x += (float)(PI*2);
		else if (cameraLook.x >= (float)2*PI) cameraLook.x -= (float)(2*PI);
		if (cameraLook.y > (float)PI/2*9/10) cameraLook.y = (float)(PI/2*9.0f/10);
		else if (cameraLook.y < (float)-PI/2*9/10) cameraLook.y = (float)(-PI/2*9/10);
		glfwSetMousePos(800/2,600/2);
		RebuildMatrix();
	}
#endif
	__super::MouseMove(x,y);
}

void GameState::Pause(){
	paused = true;
#ifdef DEBUGCAMERA
	if (!debugCamera)
#endif
	glfwEnable(GLFW_MOUSE_CURSOR);
}

void GameState::UnPause(){
	paused = false;
	glfwDisable(GLFW_MOUSE_CURSOR);
}

void GameState::KeyPress(int button, int pressed){
	if (pressed == GLFW_PRESS){
#ifdef DEBUGCAMERA
		if (debugCamera){
			switch(button){
				case 'W': moveDir |= FORWARD; break;
				case 'S': moveDir |= BACK; break;
				case 'A': moveDir |= LEFT; break;
				case 'D': moveDir |= RIGHT; break;
				case 'Q': moveDir |= UP; break;
				case 'E': moveDir |= DOWN; break;
				case 287: moveDir |= BOOST; break;
				case '\\': debugCamera = false; if (paused) glfwEnable(GLFW_MOUSE_CURSOR); break;
				case 'P':{
					if (paused) UnPause(); else Pause();
					break;
				}
				case 'T':{
					cameraPosition = glm::vec3(-48,144,96);
					cameraLook = glm::vec2(PI/2-PI/8,0);
					RebuildMatrix();
					break;
				 }
			}
		} else {
#endif
			switch(button){
				case 'P': if (paused) UnPause(); else Pause(); break;
				#ifdef DEBUGCAMERA
					case '\\': debugCamera = true;
						glfwDisable(GLFW_MOUSE_CURSOR);
			}
#endif
		}
	}
#ifdef DEBUGCAMERA
	else {
		if (debugCamera){
			switch(button){
				case 'W': moveDir &= ~FORWARD; break;
				case 'S': moveDir &= ~BACK; break;
				case 'A': moveDir &= ~LEFT; break;
				case 'D': moveDir &= ~RIGHT; break;
				case 'Q': moveDir &= ~UP; break;
				case 'E': moveDir &= ~DOWN; break;
				case 287: moveDir &= ~BOOST; break;
			}
		}
	}
#endif
}

void GameState::RebuildMatrix(){
	double xzLen = cos(cameraLook.y);
	cameraLookVector = glm::vec3(xzLen*cos(cameraLook.x),sin(cameraLook.y),xzLen*sin(-cameraLook.x));
	//print(lookVector.x<<','<<lookVector.y<<','<<lookVector.z);
	pv = projectionMatrix*glm::lookAt(cameraPosition,cameraPosition+cameraLookVector,glm::vec3(0,1,0));
}

void GameState::Begin(double currentTime){
	song->BeginPlaySong(currentTime);
	done = false;
	songPos = 0;
	running = true;
}

GameState::GameState(string songFilename, bool loop):mountainLayer1(256),mountainLayer2(256), loop(loop), running(false){
	done = false;
	paused = false;
	songPos = 0;
	songSpeed = 64;
	rightBoundary = 64;
	leftBoundary = 48;
	song = new MIDI::Song(songFilename);
	//glfwCreateThread((GLFWthreadfun)MIDI::Song::PlaySong,song);


	UINT64 notePos = 0;
	
	unordered_map<UINT16, NoteBlock*> playingNotes;

	const unsigned int coinChance = 32;

	for (auto i = song->notes.begin(); i != song->notes.end(); i++){
		MIDI::Note* note = *i;
		notePos += note->deltaTime;

		bool on = note->IsNoteOn();
		if ((note->code&0x0f) != 9 && on || note->IsNoteOff()){
			UINT16 id = ((note->code&0x0f)<<8)|(note->data1);
			auto playingNote = playingNotes.find(id);
			if (playingNote != playingNotes.end()){
				playingNote->second->UpdateLength((float)((notePos - playingNote->second->notePos)/songSpeed));
				//playingNote->second->RebuildMatrix();
				playingNotes.erase(playingNote);
			}

			if (on){
				NoteBlock* block;
				if ((rand()%(coinChance*2))==0) block = new ChestNoteBlock(this,song,note,notePos,songSpeed);
				else block = new NoteBlock(this, song, note, notePos, songSpeed, coinChance);
				noteBlocks.push_back(block);
				playingNotes[id] = block;
			}
		}
	}

	visibleNoteIterator = noteBlocks.begin();

	projectionMatrix = glm::perspective(60.0f,4.0f/3,0.01f,1000.0f);
	//pv = glm::translate(pv,glm::vec3(0,-52,-2*56));
	cameraPosition = glm::vec3(-48,144,96);
	cameraLook = glm::vec2(PI/2-PI/8,0);
#ifdef DEBUGCAMERA
	moveDir = 0;
	cameraSpeed = 32;
	debugCamera = false;
#endif
	
	RebuildMatrix();
	Pause();

}

GameState::~GameState(){
	Pause();
	delete song;
	for (auto i = noteBlocks.begin(); i != noteBlocks.end(); i++) delete *i;
}