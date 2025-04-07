#pragma once
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "Manager.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	bool init();
	void run(Manager& taskManager);
	void cleanup();

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	bool isRunning;
	void processEvents();
	void render(Manager& taskManager);
};