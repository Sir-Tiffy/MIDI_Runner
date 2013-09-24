#include "stdafx.h"
#include "Game.h"
#include "MenuState.h"
#include "CheckGLError.h"
#include "PlayableGameState.h"
#include "NumberRenderer.h"
#include "MIDI_Objects.h"
#include "TextRenderer.h"
#include "ScoreClass.h"


namespace Game{
	bool running;
	bool tryLoadSong = false;
	bool changeState = false;
	int
		fov = 60,
		width,
		height,
		FSAA = 2;
	float
		mousePosX,
		mousePosY,
		aspectRatio = 4.0f/3;
	const double updateFreq = 1.0/60;
	GLFWmutex stateMutex;
	GLFWthread updateThread;

	glm::vec3 clearColour(0,0,0);
	State* currentState = 0;

	void KeyPressed(int key, int pressed);
	void CharPressed(int c, int pressed);
	void MouseButton(int button, int pressed);
	void MouseMove(int x, int y);
	void Update(double deltaTime);
	void Render();

	void Exit(){
		running = false;
	}

	void TryLoadSong(){
		tryLoadSong = true;
	}

	void InitWindow(int w, int h){
		width = w;
		height = h;
		glClearColor(clearColour.r,clearColour.g,clearColour.b,1);
		glfwSetKeyCallback(KeyPressed);
		glfwSetCharCallback(CharPressed);
		glfwSetMousePosCallback(MouseMove);
		glfwSetMouseButtonCallback(MouseButton);
		//glfwDisable(GLFW_AUTO_POLL_EVENTS);
		glfwSwapInterval(1);
		glViewport(0,0,width,height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-1,4.0/3,-1,1,-1,1);
		//gluPerspective(60.0f,4.0f/3.0f,0.01,1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		//glOrtho(-2,2,-2,2,2,-2);
		//gluPerspective(fov,aspectRatio,0.01,100);
		//glMatrixMode(GL_MODELVIEW);
		//glfwEnable(GLFW_MOUSE_CURSOR);*/
	}

	void OpenWindow(int w = 800, int h = 600, const char* title = "MIDI_Runner"){
		if (glfwGetWindowParam(GLFW_OPENED)) glfwCloseWindow();
	
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES,FSAA);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,2);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,0);
		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,true);
		if (!glfwOpenWindow(w,h,8,8,8,0,24,0,GLFW_WINDOW)) throw runtime_error("Failed to open GLFW window!");
		glfwSetWindowTitle(title);
		InitWindow(w,h);
	}

	void KeyPressed(int key, int pressed){
		glfwLockMutex(stateMutex);
		currentState->KeyPress(key,pressed);
		glfwUnlockMutex(stateMutex);
	}

	void CharPressed(int c, int pressed){
		glfwLockMutex(stateMutex);
		currentState->CharPress(c, pressed);
		glfwUnlockMutex(stateMutex);
	}

	void MouseButton(int button, int pressed){
		glfwLockMutex(stateMutex);
		currentState->MouseButton(button,pressed);
		glfwUnlockMutex(stateMutex);
	}

	void MouseMove(int x, int y){
		bool move = false;
		if (x < 0){
			x = 0;
			move = true;
		} else if (x > width){
			x = width;
			move = true;
		}
		if (y < 0){
			y = 0;
			move = true;
		} else if (y > height){
			y = height;
			move = true;
		}
		if (move) glfwSetMousePos(x,y);

		mousePosX = (float)x/width;
		mousePosY = 1-(float)y/height;
		glfwLockMutex(stateMutex);
		if (currentState != 0) currentState->MouseMove(mousePosX,mousePosY);
		glfwUnlockMutex(stateMutex);
	}

	void Update(double currentTime, double deltaTime){
		glfwLockMutex(stateMutex);
		currentState->Update(currentTime, deltaTime);
		glfwUnlockMutex(stateMutex);
	}

	void Render(){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glfwLockMutex(stateMutex);
		currentState->Render();
		glfwUnlockMutex(stateMutex);

		/*glLoadIdentity();
		glColor3f(1,0,0);
		glBegin(GL_QUADS);
		glVertex2f(-.5,-.5);
		glVertex2f(.5,-.5);
		glVertex2f(.5,.5);
		glVertex2f(-.5,.5);
		glEnd();*/

		CheckError("Error in main render loop!");
		glfwSwapBuffers();
	}
	
	void ChangeState(State* state, bool destroyOld){
		State* oldState = currentState;
		glfwLockMutex(stateMutex);
		changeState = true;
		currentState = state;
		if (destroyOld) delete oldState;
		state->Begin(glfwGetTime());
		glfwUnlockMutex(stateMutex);
	}

	void Init(){
		currentState = 0;
		if (!glfwInit()) throw runtime_error("Failed to init GLFW!");
		stateMutex = glfwCreateMutex();

		Game::OpenWindow(800,600,"MIDI_Runner");
		int status = glewInit();
		if (status != GLEW_OK) throw runtime_error(string("Failed to init GLEW!\n").append((const char*)glewGetErrorString(status)));
		TextRenderer::Init();
		NumberRenderer::Init();
		ScoreClass::LoadScores();
		currentState = new MenuState(InitialSong);
		midiOutOpen(&MIDI::Song::device,0,0,0,0);
		srand((unsigned int)time(NULL));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Cleanup(){
		glfwSetKeyCallback(0);
		glfwSetMousePosCallback(0);
		glfwSetMouseButtonCallback(0);
		glfwLockMutex(stateMutex);
		delete currentState;
		glfwDestroyThread(updateThread);
		glfwUnlockMutex(stateMutex);
		glfwDestroyMutex(stateMutex);
		glfwTerminate();
		midiOutClose(MIDI::Song::device);
		MIDI::Song::device = 0;
	}

	void UpdateThread(void*){
		double lastTime = 0;
		double currentTime;
		double deltaTime;
		glfwSetTime(0);
		currentState->Begin(glfwGetTime());
		while (running){
			currentTime = glfwGetTime();
			if (changeState){
				deltaTime = 0;
				changeState = false;
			}
			else deltaTime = currentTime-lastTime;
			lastTime = currentTime;
		
			Update(currentTime,deltaTime);
			glfwSleep(updateFreq);
		}
	}
	void LoadSong(){
		OPENFILENAME ofn = {0};
		char filename[MAX_PATH] = "";

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFilter = "MIDI Files (*.mid)\0*.mid\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
		ofn.lpstrInitialDir = ".\\midi\\";
		//ofn.lpstrDefExt = "";
		tryLoadSong = false;
		if (GetOpenFileName(&ofn)){
			ChangeState(new PlayableGameState(filename),true);
		}
	}

	void Begin(){
		try{
			running = true;
			updateThread = glfwCreateThread(UpdateThread,0); //begin update thread
			while (running){ //render
				Render();
				if (tryLoadSong) LoadSong();
				if (!glfwGetWindowParam(GLFW_OPENED)) running = false;
			}
		} catch (exception &e){
			Cleanup();
			throw e;
		}
		Cleanup();
	}
};