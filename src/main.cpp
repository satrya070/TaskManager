#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <any>
#include <memory>
#include <format>
#include "Manager.h"
#include "Command.h"
#include "Command.cpp"
#include "Task.h"
#include <fstream>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"


bool initTables(sqlite3* db, const std::string& filePath) {
    // creates the database tables if they don't exist yet
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: file doesn't exist." << std::endl;
        return false;
    }

    // read content into string buffer
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sql = buffer.str();

    // execute sql
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Failed to create tables with: " << filePath << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

static std::vector<Task> showTasksPreview(sqlite3* db) {
    std::string selectQuery = "SELECT name, deadline, done FROM tasks LIMIT 5;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    std::vector<Task> tasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string task_name;

        const unsigned char* text;
        for (int i = 0; i < 3; i++) {
            int columnType = sqlite3_column_type(stmt, i);

            switch (columnType) {
            case SQLITE_TEXT:
                text = sqlite3_column_text(stmt, i);
                task_name = text ? reinterpret_cast<const char*>(text) : "";
                std::cout << task_name << " | ";
                break;
            case SQLITE_INTEGER:
                std::cout << sqlite3_column_int(stmt, i) << " | ";
                break;
            }
        }
        tasks.emplace_back(task_name);
        //tasks.push_back(std::make_unique<Task>(task_name));
        std::cout << std::endl;
    }
    sqlite3_finalize(stmt);

    return tasks;
}


int main() {
    sqlite3* db;
    int returnCode = sqlite3_open_v2(
        "tasks_database.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr
    );
    if (returnCode != SQLITE_OK) {
        std::cout << returnCode << ": Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    initTables(db, "setup.sql");

    // always the show the first 5 tasks
    std::vector<Task> tasks = showTasksPreview(db);
    std::cout << tasks.size() << std::endl;
    for (const Task& task : tasks) {
        std::cout << task.getTaskName() << std::endl;
    }

    int input;
    //std::cin >> choice;
    input = 0;

    Manager taskManager;

    // TODO ---- refactor to gui

    // set and execute command
    //taskManager.setCommand(std::make_unique<addCommand>(db, "buy eggs", "2025-06-14"));
    //taskManager.executeCommand();

    // delete command
    //taskManager.setCommand(std::make_unique<deleteCommand>(db, 10));
    //taskManager.executeCommand();

    // TODO
    //update command


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

        ImGui::TextWrapped("asdfsafasdfdasfdsaf");
        static char buff[32];
        ImGui::InputText("default", buff, 32);
        ImGui::BulletText("bullet 1");

        static int clicked = 0;
        if (ImGui::Button("Delete task")) {
            std::cout << "delete task!" << std::endl;
            clicked++;
        }
        if (clicked) {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
        }
        ImGui::Separator();

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
        ImGui::BeginTable("table", 3, tableFlags);
        ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthFixed, 200.f); // , ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("date", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableSetupColumn("done", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableHeadersRow();

        for (const Task& task : tasks) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("Task name");
            ImGui::TableSetColumnIndex(1); ImGui::Text(task.getTaskName().c_str());
        }

        ImGui::EndTable();

        ImGui::End();

        // archived window
        if (showArchiveWindow) {
            ImGui::Begin("archive", &showArchiveWindow);
            ImGui::Text("asdf");
            ImGui::End();

            //if(ImGui::Button("Close"))
        }

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "done";

    return 0;
}