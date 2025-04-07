#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <any>
#include <memory>
#include <format>
#include "Manager.h"
#include "Command.h"
#include "Task.h"
#include <fstream>
#include <sstream>
#include "SqliteDatabase.h"
#include "Renderer.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"


int main() {
    SqliteDatabase taskRepositoryDB("tasks_database.db");
    TaskRepository taskRepository(taskRepositoryDB);
    Manager taskManager(taskRepository);
    
    // renders GUI and all related functions
    Renderer renderer;
    if (!renderer.init()) {
        std::cout << "Renderer initialization failed." << std::endl;
        return -1;
    }

    renderer.run(taskManager);
    
    return 0;
}