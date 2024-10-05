#include "raylib.h"
#include "raygui.h"
#include "../include/ui_op.h"
#include "../include/student_op.h"
#include "../include/csv_op.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_INPUT_LEN 100

// Static variables to store input data across frames
static char uid_input[MAX_INPUT_LEN] = {0};
/*
static char roll_number_input[MAX_INPUT_LEN] = {0};
static char name[MAX_INPUT_LEN] = {0};
static char major[MAX_INPUT_LEN] = {0};
static char minor[MAX_INPUT_LEN] = {0};
static char oe1[MAX_INPUT_LEN] = {0};
static char oe2[MAX_INPUT_LEN] = {0};
*/

// Static variables for textbox focus
static int current_focused_box = -1;  // -1 means no box is focused

// Function to manage focus and input handling for textboxes
bool ManageTextBoxFocus(Rectangle bounds, char *input, int maxLength, int boxIndex) {
    bool result = GuiTextBox(bounds, input, maxLength, current_focused_box == boxIndex);
    
    // If a textbox is clicked, it becomes the focused box
    if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        current_focused_box = boxIndex;
    }

    // If Enter is pressed, unfocus the textbox
    if (IsKeyPressed(KEY_ENTER) && current_focused_box == boxIndex) {
        current_focused_box = -1;
    }

    return result;
}

// Function to draw UI for adding a student
void DrawAddStudentUI(bool *confirm_button_pressed) {
    DrawText("Add Student", 10, 100, 20, DARKGRAY);

    // Static variables to store input data across frames
    static char uid_input[MAX_INPUT_LEN] = {0};
    static char roll_number_input[MAX_INPUT_LEN] = {0};
    static char name[MAX_NAME_LEN] = {0};
    static char major[MAX_NAME_LEN] = {0};
    static char minor[MAX_NAME_LEN] = {0};
    static char oe1[MAX_NAME_LEN] = {0};
    static char oe2[MAX_NAME_LEN] = {0};

    // Manage input fields for UID and Roll Number
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, MAX_INPUT_LEN, 0);

    GuiLabel((Rectangle){ 10, 180, 100, 30 }, "Roll Number:");
    ManageTextBoxFocus((Rectangle){ 120, 180, 200, 30 }, roll_number_input, MAX_INPUT_LEN, 1);

    // Manage input fields for Name, Major, Minor, OE1, OE2
    GuiLabel((Rectangle){ 10, 220, 100, 30 }, "Name:");
    ManageTextBoxFocus((Rectangle){ 120, 220, 200, 30 }, name, MAX_NAME_LEN, 2);

    GuiLabel((Rectangle){ 10, 260, 100, 30 }, "Major:");
    ManageTextBoxFocus((Rectangle){ 120, 260, 200, 30 }, major, MAX_NAME_LEN, 3);

    GuiLabel((Rectangle){ 10, 300, 100, 30 }, "Minor:");
    ManageTextBoxFocus((Rectangle){ 120, 300, 200, 30 }, minor, MAX_NAME_LEN, 4);

    GuiLabel((Rectangle){ 10, 340, 100, 30 }, "OE1:");
    ManageTextBoxFocus((Rectangle){ 120, 340, 200, 30 }, oe1, MAX_NAME_LEN, 5);

    GuiLabel((Rectangle){ 10, 380, 100, 30 }, "OE2:");
    ManageTextBoxFocus((Rectangle){ 120, 380, 200, 30 }, oe2, MAX_NAME_LEN, 6);

    *confirm_button_pressed = GuiButton((Rectangle){ 10, 420, 100, 30 }, "Confirm Add");

    if (*confirm_button_pressed) {
        // Validate UID and Roll Number input
        if (strlen(uid_input) == 0 || strlen(roll_number_input) == 0) {
            DrawText("UID and Roll Number must be filled!", 10, 460, 20, RED);
            return;  // Early exit if inputs are invalid
        }

        // Convert UID and Roll Number to integers
        int uid = atoi(uid_input);
        int roll_number = atoi(roll_number_input);

        // Zero-initialize the Student struct to avoid undefined behavior
        Student student = {0};
        student.uid = uid;
        student.roll_number = roll_number;

        // Safely copy strings, ensuring null-termination and preventing overflows
        strncpy(student.name, name, MAX_NAME_LEN - 1);
        student.name[MAX_NAME_LEN - 1] = '\0';

        strncpy(student.major, major, MAX_NAME_LEN - 1);
        student.major[MAX_NAME_LEN - 1] = '\0';

        strncpy(student.minor, minor, MAX_NAME_LEN - 1);
        student.minor[MAX_NAME_LEN - 1] = '\0';

        strncpy(student.oe1, oe1, MAX_NAME_LEN - 1);
        student.oe1[MAX_NAME_LEN - 1] = '\0';

        strncpy(student.oe2, oe2, MAX_NAME_LEN - 1);
        student.oe2[MAX_NAME_LEN - 1] = '\0';

        // Add student to CSV
        add_student(student);

        // Clear input fields after successfully adding the student
        memset(uid_input, 0, sizeof(uid_input));
        memset(roll_number_input, 0, sizeof(roll_number_input));
        memset(name, 0, sizeof(name));
        memset(major, 0, sizeof(major));
        memset(minor, 0, sizeof(minor));
        memset(oe1, 0, sizeof(oe1));
        memset(oe2, 0, sizeof(oe2));
    }
}


// Function to draw UI for removing a student
void DrawRemoveStudentUI(bool *confirm_button_pressed) {
    DrawText("Remove Student", 10, 100, 20, DARKGRAY);
    
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, MAX_INPUT_LEN, 0);

    *confirm_button_pressed = GuiButton((Rectangle){ 10, 180, 100, 30 }, "Confirm Remove");

    if (*confirm_button_pressed) {
        int uid = atoi(uid_input);
        remove_student(uid);
    }
}

// Function to draw UI for searching a student
void DrawSearchStudentUI(bool *search_button_pressed) {
    DrawText("Search Student", 10, 100, 20, DARKGRAY);

    // Static variables to hold input and status
    static char uid_input[10] = {0};
    static bool student_not_found = false;
    static Student found_student;
    static bool student_found = false;

    // Search input field
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, sizeof(uid_input), 0);

    *search_button_pressed = GuiButton((Rectangle){ 10, 180, 100, 30 }, "Search");

    if (*search_button_pressed) {
        int uid = atoi(uid_input);
        Student *student = search_student(uid);

        if (student != NULL) {
            found_student = *student; // Copy found student details to static storage
            student_found = true;
            student_not_found = false;
            free(student);
        } else {
            student_not_found = true;
            student_found = false;
        }
    }

    // Display student details if found
    if (student_found) {
        int startY = 220;

        DrawText(TextFormat("UID: %07d", found_student.uid), 10, startY, 20, DARKGRAY);
        startY += 30;
        DrawText(TextFormat("Name: %s", found_student.name), 10, startY, 20, DARKGRAY);
        startY += 30;
        DrawText(TextFormat("Major: %s", found_student.major), 10, startY, 20, DARKGRAY);
        startY += 30;
        DrawText(TextFormat("Minor: %s", found_student.minor), 10, startY, 20, DARKGRAY);
        startY += 30;
        DrawText(TextFormat("OE1: %s", found_student.oe1), 10, startY, 20, DARKGRAY);
        startY += 30;
        DrawText(TextFormat("OE2: %s", found_student.oe2), 10, startY, 20, DARKGRAY);
    }

    // Display "Student Not Found" message if applicable
    if (student_not_found) {
        DrawText("Student not found.", 10, 220, 20, RED);
    }
}


// Function to draw UI for editing a student
void DrawEditStudentUI(bool *edit_button_pressed, bool *save_button_pressed) {
    DrawText("Edit Student", 10, 100, 20, DARKGRAY);

    // Static variables to hold input and status
    static char uid_input[10] = {0};
    static bool student_not_found = false;
    static Student found_student;
    static bool student_found = false;

    // Search input field for UID
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, sizeof(uid_input), 0);

    *edit_button_pressed = GuiButton((Rectangle){ 10, 180, 100, 30 }, "Search");

    if (*edit_button_pressed) {
        int uid = atoi(uid_input);
        Student *student = search_student(uid);

        if (student != NULL) {
            found_student = *student; // Copy found student details to static storage
            student_found = true;
            student_not_found = false;
            free(student);
        } else {
            student_not_found = true;
            student_found = false;
        }
    }

    // Display student details if found, with editable fields
    if (student_found) {
        int startY = 220;

        GuiLabel((Rectangle){ 10, startY, 100, 30 }, "Name:");
        ManageTextBoxFocus((Rectangle){ 120, startY, 200, 30 }, found_student.name, sizeof(found_student.name), 1);
        startY += 40;

        GuiLabel((Rectangle){ 10, startY, 100, 30 }, "Major:");
        ManageTextBoxFocus((Rectangle){ 120, startY, 200, 30 }, found_student.major, sizeof(found_student.major), 2);
        startY += 40;

        GuiLabel((Rectangle){ 10, startY, 100, 30 }, "Minor:");
        ManageTextBoxFocus((Rectangle){ 120, startY, 200, 30 }, found_student.minor, sizeof(found_student.minor), 3);
        startY += 40;

        GuiLabel((Rectangle){ 10, startY, 100, 30 }, "OE1:");
        ManageTextBoxFocus((Rectangle){ 120, startY, 200, 30 }, found_student.oe1, sizeof(found_student.oe1), 4);
        startY += 40;

        GuiLabel((Rectangle){ 10, startY, 100, 30 }, "OE2:");
        ManageTextBoxFocus((Rectangle){ 120, startY, 200, 30 }, found_student.oe2, sizeof(found_student.oe2), 5);
        startY += 40;

        *save_button_pressed = GuiButton((Rectangle){ 10, startY, 100, 30 }, "Save");
        if (*save_button_pressed) {
            edit_student(found_student.uid, found_student);
            student_found = false; // Reset after saving
        }
    }

    // Display "Student Not Found" message if applicable and there is no ongoing search
    if (student_not_found && !student_found) {
        DrawText("Student not found.", 10, 220, 20, RED);
    }
}

// Function to draw UI for listing all students
void DrawListStudentsUI() {
    DrawText("List of Students", 10, 100, 20, DARKGRAY);

    int count = 0;
    Student *students = load_all_students(&count);

    if (students == NULL || count <= 1) {
        DrawText("No students found.", 10, 140, 20, RED);
    } else {
        // Constants for pagination
        const int students_per_page = 10;
        static int current_page = 0;
        int total_pages = ((count - 1) + students_per_page - 1) / students_per_page;

        // Define start position for student list display
        int startX = 10;
        int startY = 140;

        // Adjusted column spacings based on your preferences
        DrawText("UID", startX, startY, 20, DARKBLUE);
        DrawText("Name", startX + 100, startY, 20, DARKBLUE);
        DrawText("Major", startX + 400, startY, 20, DARKBLUE);
        DrawText("Minor", startX + 500, startY, 20, DARKBLUE);
        DrawText("OE1", startX + 650, startY, 20, DARKBLUE);
        DrawText("OE2", startX + 920, startY, 20, DARKBLUE);

        // Calculate start and end indices for the current page, starting from the second student
        int start_index = 1 + (current_page * students_per_page);
        int end_index = start_index + students_per_page;
        if (end_index > count) {
            end_index = count;
        }

        // Iterate through students and display relevant information for the current page
        startY += 30;  // Move down to start displaying students
        for (int i = start_index; i < end_index; i++) {
            DrawText(TextFormat("%07d", students[i].uid), startX, startY, 20, DARKGRAY); // UID with leading zeros if needed, 7 digits
            DrawText(students[i].name, startX + 100, startY, 20, DARKGRAY); // Name column
            DrawText(students[i].major, startX + 400, startY, 20, DARKGRAY); // Major (e.g., "BSc IT")
            DrawText(students[i].minor, startX + 500, startY, 20, DARKGRAY); // Minor (e.g., "BAF - Financial Markets")
            DrawText(students[i].oe1, startX + 650, startY, 20, DARKGRAY); // OE1 (e.g., "Demystifying Mental Illness")
            DrawText(students[i].oe2, startX + 920, startY, 20, DARKGRAY); // OE2 (e.g., "Demystifying Mental Illness")

            startY += 30;  // Move down for each student entry
        }

        // Draw Previous and Next buttons
        bool prev_button_pressed = GuiButton((Rectangle){ 10, startY + 10, 100, 30 }, "Previous");
        bool next_button_pressed = GuiButton((Rectangle){ 120, startY + 10, 100, 30 }, "Next");

        // Update current page based on button presses
        if (prev_button_pressed && current_page > 0) {
            current_page--;
        }
        if (next_button_pressed && current_page < total_pages - 1) {
            current_page++;
        }

        // Draw current page indicator
        DrawText(TextFormat("Page %d of %d", current_page + 1, total_pages), 250, startY + 10, 20, DARKGRAY);
    }

    free(students);
}