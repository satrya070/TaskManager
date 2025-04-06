#include "Renderer.h"
#include "Task.h"
#include <iostream>

Renderer::Renderer() : window(nullptr), glContext(nullptr), isRunning(false) {}

Renderer::~Renderer() {
	cleanup();
}

bool Renderer::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("TaskManager", 800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cout << "openGL context could not be created." << SDL_GetError() << std::endl;
        return false;
    }
    SDL_GL_MakeCurrent(window, glContext);

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // setup platform render backend(openGL)
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    isRunning = true;
    return true;
}

void Renderer::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
            isRunning = false;
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
            isRunning = false;
    }
}

static bool showArchiveWindow = false;
void Renderer::render(Manager& taskManager) {
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

    // add task header elements
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
    }
    ImGui::Spacing();

    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
    ImGui::BeginTable("table", 4, tableFlags);
    ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthFixed, 200.f);
    ImGui::TableSetupColumn("Created", ImGuiTableColumnFlags_WidthFixed, 100.f);
    ImGui::TableSetupColumn("Deadline", ImGuiTableColumnFlags_WidthFixed, 100.f);
    ImGui::TableHeadersRow();

    for (const Task& task : taskManager.taskRepository.fetchTasks()) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text(task.getTaskName().c_str());
        ImGui::TableSetColumnIndex(1); ImGui::Text(task.getCreateDate().c_str());
        ImGui::TableSetColumnIndex(2); ImGui::Text(task.getDeadlineDate().c_str());
        ImGui::PushID(task.getTaskId());
        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("done")) {
            taskManager.taskRepository.archiveTask(
                task.getTaskId(), task.getTaskName(), task.getDeadlineDate()
            );

            //abort loop with load updated tasks
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

        for (const auto& task : taskManager.taskRepository.fetchArchivedTasks()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text(task.getTaskName().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text(task.getDeadlineDate().c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::PushID(task.getTaskId());
            if (ImGui::Button("Delete")) {
                taskManager.taskRepository.deleteTask(task.getTaskId());

                //abort the current loop to load updated archive
                ImGui::PopID();
                break;
            }
            ImGui::PopID();
        }
        ImGui::EndTable();

        ImGui::End();
    }

    // render frame
    ImGui::Render();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void Renderer::run(Manager& taskManager) {
    while (isRunning) {
        processEvents();
        render(taskManager);
    }
}

void Renderer::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}