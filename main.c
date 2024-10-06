#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#include "../include/student_op.h"
#include "../include/ui_op.h"

#define MAX_INPUT_LEN 100

int main() {
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Student Management System");
    SetTargetFPS(60);

    int current_action = 4;  // Start with "List All Students" as default
    bool confirm_button_pressed = false;
    bool edit_button_pressed = false;
    bool save_button_pressed = false;

    // Main loop
    while (!WindowShouldClose()) {
        // Start drawing the UI
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title
        DrawText("Student Management System", 10, 10, 20, DARKGRAY);

        // Action buttons laid out horizontally
        if (GuiButton((Rectangle){ 10, 50, 120, 30 }, "Add Student")) current_action = 0;
        if (GuiButton((Rectangle){ 140, 50, 120, 30 }, "Remove Student")) current_action = 1;
        if (GuiButton((Rectangle){ 270, 50, 120, 30 }, "Search Student")) current_action = 2;
        if (GuiButton((Rectangle){ 400, 50, 120, 30 }, "Edit Student")) current_action = 3;
        if (GuiButton((Rectangle){ 530, 50, 120, 30 }, "List Students")) current_action = 4;

        // Draw the appropriate UI based on the current action
        switch (current_action) {
            case 0: DrawAddStudentUI(&confirm_button_pressed); break;
            case 1: DrawRemoveStudentUI(&confirm_button_pressed); break;
            case 2: DrawSearchStudentUI(&confirm_button_pressed); break;
            case 3: DrawEditStudentUI(&edit_button_pressed, &save_button_pressed); break;
            case 4: DrawListStudentsUI(); break;
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
