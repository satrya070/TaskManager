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

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"


bool showAddTaskWindow = false;
void addTaskWindow() {
    if (showAddTaskWindow) {
        ImGui::Begin("Add task", &showAddTaskWindow);
        ImGui::Button("add task");
        ImGui::End();
    }
}


int main() {
    SqliteDatabase taskRepositoryDB("tasks_database.db");
    TaskRepository taskRepository(taskRepositoryDB);
    Manager taskManager(taskRepository);

    std::vector<Task> tasks = taskManager.taskRepository.fetchTasks();
    
    //----- GUI -----------------------------
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }
    
    // setup window
    SDL_Window* window = SDL_CreateWindow("TaskManager", 800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    
    if (!window) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // setup platform render backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    // initial window status
    bool showArchiveWindow = false;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                running = false;
        }

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // confire to fill screen
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse;

        // GUI
        ImGui::Begin("Task Manager", nullptr, window_flags);

        static char buff[200];
        ImGui::InputText("default", buff, 200);
        ImGui::SameLine();
        static char buff2[32];
        ImGui::InputText("default##2", buff2, 32);

        static int clicked = 0;
        if (ImGui::Button("Delete task")) {
            std::cout << "delete task!" << std::endl;
            clicked++;
        }
        if (clicked) {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
        }
        if (ImGui::Button("add task")) {
            // TODO implement real add
            /*taskManager.setCommand(std::make_unique<AddCommand>(db, "refresh database", "2025-04-10"));
            taskManager.executeCommand();*/
            std::cout << "task was added!" << std::endl;
            // refresh view
            //tasks = showTasksPreview(db);
        }
        ImGui::Separator();

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
        ImGui::BeginTable("table", 3, tableFlags);
        ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthFixed, 200.f); // , ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("date", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableHeadersRow();

        for (const Task& task : tasks) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text(task.getTaskName().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text(task.getTaskName().c_str());
            ImGui::PushID(task.getTaskId());
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("done")) {
                /*taskManager.setCommand(std::make_unique<ArchiveCommand>(
                    db, task.getTaskId(), task.getTaskName(), "0000-00-00"
                ));
                taskManager.executeCommand();*/
            }
            ImGui::PopID();
        }

        ImGui::EndTable();

        if (ImGui::Button("Tasks archive"))
            showArchiveWindow = true;

        ImGui::End();

        // archived window
        if (showArchiveWindow) {
            ImGui::Begin("archive", &showArchiveWindow);
            ImGui::Text("asdf");
            ImGui::End();
        }

        // add task
        addTaskWindow();

        // DEMO
        // ImGui::ShowDemoWindow();

        // render
        ImGui::Render();
        //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // cleanup
    //sqlite3_close(db);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "done";

    return 0;
}