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
    std::vector<Task> archived_tasks = taskManager.taskRepository.fetchArchivedTasks();
    
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

        static char buffName[256];
        static char buffDeadline[11];

        //
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));           // transparent background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));    // no hover color
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));     // no click color
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        float windowWidth = ImGui::GetContentRegionAvail().x;
        ImGui::Button("new task..", ImVec2(windowWidth * 0.6, 0));
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(4);
        
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::Text("Deadline date");
        ImGui::PopStyleColor();

        // add task input fields
        ImGui::PushItemWidth(windowWidth * 0.6);
        ImGui::InputText("##taskName", buffName, IM_ARRAYSIZE(buffName));
        ImGui::PopItemWidth();
        ImGui::SameLine();
  
        ImGui::PushItemWidth(100);
        ImGui::InputText("##taskDeadline", buffDeadline, 11);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("add task")) {
            taskManager.taskRepository.addTask(buffName, buffDeadline);

            // clear input
            buffName[0] = '\0';
            buffDeadline[0] = '\0';

            // refresh tasks view
            tasks = taskManager.taskRepository.fetchTasks();
        }
        //ImGui::Separator();
        ImGui::Spacing();

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
        ImGui::BeginTable("table", 3, tableFlags);
        ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthFixed, 200.f); // , ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("date", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableHeadersRow();

        for (const Task& task : tasks) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text(task.getTaskName().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text(task.getDeadlineDate().c_str());
            ImGui::PushID(task.getTaskId());
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("done")) {
                taskManager.taskRepository.archiveTask(
                    task.getTaskId(), task.getTaskName(), task.getDeadlineDate()
                );

                //refresh views
                tasks = taskManager.taskRepository.fetchTasks();
                archived_tasks = taskManager.taskRepository.fetchArchivedTasks();

                //abort the current loop with old version of tasks
                ImGui::PopID();
                break;
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
            
            ImGui::BeginTable("asdf", 3, tableFlags);
            ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthFixed, 200.f);
            ImGui::TableSetupColumn("dateFinished", ImGuiTableColumnFlags_WidthFixed, 200.f);
            ImGui::TableHeadersRow();
            
            for (const auto& task : archived_tasks) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text(task.getTaskName().c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text(task.getDeadlineDate().c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::PushID(task.getTaskId());
                if (ImGui::Button("Delete")) {
                    taskManager.taskRepository.deleteTask(task.getTaskId());
                    // refresh archive view
                    archived_tasks = taskManager.taskRepository.fetchArchivedTasks();

                    //abort the current loop with old version of tasks
                    ImGui::PopID();
                    break;
                }
                ImGui::PopID();
            }
            ImGui::EndTable();

            ImGui::End();
        }

        // add task
        addTaskWindow();

        // render
        ImGui::Render();
        //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "done";

    return 0;
}