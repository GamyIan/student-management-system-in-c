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
static char uid_input[MAX_INPUT_LEN] = {0};  // Only this one is being used.
/*
static char roll_number_input[MAX_INPUT_LEN] = {0};
static char name[MAX_INPUT_LEN] = {0};
static char major[MAX_INPUT_LEN] = {0};
static char minor[MAX_INPUT_LEN] = {0};
static char oe1[MAX_INPUT_LEN] = {0};
static char oe2[MAX_INPUT_LEN] = {0};
*/

// Static variable for textbox focus
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
    DrawText("Add Student", 10, 100, 30, DARKGRAY); // Increased font size to 30

    // Local variables to store input data for the function
    static char uid_input[10] = {0};
    static char roll_number_input[10] = {0};
    static char name[100] = {0}; // Add name input field
    static int major_index = 0;  // Index for selected Major
    static int minor_index = 0;  // Index for selected Minor
    static int oe1_index = 0;    // Index for selected OE1
    static int oe2_index = 0;    // Index for selected OE2
    static bool uid_exists = false; // Flag to indicate if the UID already exists

    // Arrays to store valid options for subjects
    const char *major_options = "BSc IT;BAF;BMS;BSC;B.Com";
    const char *minor_options = "BMS;BAF (FS);BAF (A/c);BA MCJ";
    const char *oe1_options = "Accounting;Economics;Financial Literacy;Mass Communication;Nature & Conservation;Psch. Visual Art";
    const char *oe2_options = "Current Media;Demystifying Mental Illness;Economics;French;Investment Analysis;IPRS;Taxation;Tourism Mgmt";

    // Manage input fields for UID, Roll Number, and Name
    GuiLabel((Rectangle){ 10, 140, 150, 50 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 170, 140, 300, 50 }, uid_input, sizeof(uid_input), 0); // Increased TextBox size

    GuiLabel((Rectangle){ 10, 200, 150, 50 }, "Roll Number:");
    ManageTextBoxFocus((Rectangle){ 170, 200, 300, 50 }, roll_number_input, sizeof(roll_number_input), 1); // Increased TextBox size

    GuiLabel((Rectangle){ 10, 260, 150, 50 }, "Name:");
    ManageTextBoxFocus((Rectangle){ 170, 260, 300, 50 }, name, sizeof(name), 2); // New TextBox for Name

    // Subject Selection (Combo Boxes for autofill)
    GuiLabel((Rectangle){ 10, 320, 150, 50 }, "Major:");
    GuiComboBox((Rectangle){ 170, 320, 300, 50 }, major_options, &major_index);

    GuiLabel((Rectangle){ 10, 380, 150, 50 }, "Minor:");
    GuiComboBox((Rectangle){ 170, 380, 300, 50 }, minor_options, &minor_index);

    GuiLabel((Rectangle){ 10, 440, 150, 50 }, "OE1:");
    GuiComboBox((Rectangle){ 170, 440, 300, 50 }, oe1_options, &oe1_index);

    GuiLabel((Rectangle){ 10, 500, 150, 50 }, "OE2:");
    GuiComboBox((Rectangle){ 170, 500, 300, 50 }, oe2_options, &oe2_index);

    // Confirm Add button
    *confirm_button_pressed = GuiButton((Rectangle){ 10, 560, 150, 50 }, "Confirm Add");

    if (*confirm_button_pressed) {
        // Validate UID, Roll Number, and Name input
        if (strlen(uid_input) == 0 || strlen(roll_number_input) == 0 || strlen(name) == 0) {
            DrawText("UID, Roll Number, and Name must be filled!", 10, 620, 20, RED);
            return;  // Early exit if inputs are invalid
        }

        // Convert UID and Roll Number to integers
        int uid = atoi(uid_input);
        int roll_number = atoi(roll_number_input);

        // Check if UID already exists
        Student *existing_student = search_student(uid);
        if (existing_student != NULL) {
            uid_exists = true;
            free(existing_student);
            return;  // Early exit if UID already exists
        } else {
            uid_exists = false;
        }

        // Initialize student data
        Student student = {0};
        student.uid = uid;
        student.roll_number = roll_number;
        strncpy(student.name, name, sizeof(student.name) - 1);
        student.name[sizeof(student.name) - 1] = '\0';

        // Extract and assign Major
        char major[100] = {0};
        strncpy(major, major_options, sizeof(major) - 1);
        char *token = strtok(major, ";");
        for (int i = 0; i < major_index; i++) {
            token = strtok(NULL, ";");
        }
        strncpy(student.major, token, sizeof(student.major) - 1);
        student.major[sizeof(student.major) - 1] = '\0';

        // Extract and assign Minor
        char minor[100] = {0};
        strncpy(minor, minor_options, sizeof(minor) - 1);
        token = strtok(minor, ";");
        for (int i = 0; i < minor_index; i++) {
            token = strtok(NULL, ";");
        }
        strncpy(student.minor, token, sizeof(student.minor) - 1);
        student.minor[sizeof(student.minor) - 1] = '\0';

        // Extract and assign OE1
        char oe1[100] = {0};
        strncpy(oe1, oe1_options, sizeof(oe1) - 1);
        token = strtok(oe1, ";");
        for (int i = 0; i < oe1_index; i++) {
            token = strtok(NULL, ";");
        }
        strncpy(student.oe1, token, sizeof(student.oe1) - 1);
        student.oe1[sizeof(student.oe1) - 1] = '\0';

        // Extract and assign OE2
        char oe2[100] = {0};
        strncpy(oe2, oe2_options, sizeof(oe2) - 1);
        token = strtok(oe2, ";");
        for (int i = 0; i < oe2_index; i++) {
            token = strtok(NULL, ";");
        }
        strncpy(student.oe2, token, sizeof(student.oe2) - 1);
        student.oe2[sizeof(student.oe2) - 1] = '\0';

        // Add student to CSV
        add_student(student);

        // Clear input fields after successfully adding the student
        memset(uid_input, 0, sizeof(uid_input));
        memset(roll_number_input, 0, sizeof(roll_number_input));
        memset(name, 0, sizeof(name));
        major_index = 0;
        minor_index = 0;
        oe1_index = 0;
        oe2_index = 0;
    }

    // Display "UID already exists" message if applicable
    if (uid_exists) {
        DrawText("UID already exists.", 10, 620, 20, RED);
    }
}


// Function to draw UI for removing a student
void DrawRemoveStudentUI(bool *remove_button_pressed) {
    DrawText("Remove Student", 10, 100, 20, DARKGRAY);

    // Static variables to hold input and status
    static bool uid_not_found = false; // Flag to indicate if the UID does not exist
    static bool student_removed = false; // Flag to indicate if the student was removed successfully

    // Search input field for UID
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, sizeof(uid_input), 0);

    *remove_button_pressed = GuiButton((Rectangle){ 10, 180, 100, 30 }, "Remove");

    if (*remove_button_pressed) {
        int uid = atoi(uid_input);

        // Check if the UID exists
        Student *existing_student = search_student(uid);
        if (existing_student == NULL) {
            // UID not found
            uid_not_found = true;
            student_removed = false;
        } else {
            // UID exists, proceed to remove
            remove_student(uid);
            uid_not_found = false;
            student_removed = true;
            free(existing_student);
        }
    }

    // Display messages based on the operation result
    if (uid_not_found) {
        DrawText("UID not found.", 10, 220, 20, RED);
    }
    if (student_removed) {
        DrawText("Student removed successfully.", 10, 220, 20, GREEN);
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
void DrawEditStudentUI(bool *edit_button_pressed) {
    DrawText("Edit Student", 10, 100, 20, DARKGRAY);

    static char uid_input[MAX_INPUT_LEN] = {0};  // Input for the UID to be searched
    static Student found_student;                // Holds the student data to be edited
    static bool student_found = false;           // Tracks if a student was found

    // Input field for the UID to search for the student
    GuiLabel((Rectangle){ 10, 140, 100, 30 }, "UID:");
    ManageTextBoxFocus((Rectangle){ 120, 140, 200, 30 }, uid_input, MAX_INPUT_LEN, 0);

    // Search button
    *edit_button_pressed = GuiButton((Rectangle){ 10, 180, 100, 30 }, "Search");

    if (*edit_button_pressed) {
        // Convert UID input to integer
        if (strlen(uid_input) > 0) {
            int uid = atoi(uid_input);
            Student *student = search_student(uid);

            if (student != NULL) {
                // Store the found student in a static variable for persistent display and editing
                found_student = *student;
                student_found = true;

                // Free the memory allocated by `search_student()`
                free(student);
            } else {
                student_found = false;
            }
        } else {
            student_found = false;
        }
    }

    if (student_found) {
        // Editable textboxes for all student details
        static char roll_number_input[MAX_INPUT_LEN] = {0};
        static char name_input[MAX_NAME_LEN] = {0};
        static char major_input[MAX_NAME_LEN] = {0};
        static char minor_input[MAX_NAME_LEN] = {0};
        static char oe1_input[MAX_NAME_LEN] = {0};
        static char oe2_input[MAX_NAME_LEN] = {0};

        // Initialize the fields with existing data if not already initialized
        if (*edit_button_pressed) {
            snprintf(roll_number_input, MAX_INPUT_LEN, "%d", found_student.roll_number);
            strncpy(name_input, found_student.name, MAX_NAME_LEN);
            strncpy(major_input, found_student.major, MAX_NAME_LEN);
            strncpy(minor_input, found_student.minor, MAX_NAME_LEN);
            strncpy(oe1_input, found_student.oe1, MAX_NAME_LEN);
            strncpy(oe2_input, found_student.oe2, MAX_NAME_LEN);
        }

        // Textboxes for editing details
        GuiLabel((Rectangle){ 10, 220, 100, 30 }, "Roll Number:");
        ManageTextBoxFocus((Rectangle){ 120, 220, 200, 30 }, roll_number_input, MAX_INPUT_LEN, 1);

        GuiLabel((Rectangle){ 10, 260, 100, 30 }, "Name:");
        ManageTextBoxFocus((Rectangle){ 120, 260, 200, 30 }, name_input, MAX_NAME_LEN, 2);

        GuiLabel((Rectangle){ 10, 300, 100, 30 }, "Major:");
        ManageTextBoxFocus((Rectangle){ 120, 300, 200, 30 }, major_input, MAX_NAME_LEN, 3);

        GuiLabel((Rectangle){ 10, 340, 100, 30 }, "Minor:");
        ManageTextBoxFocus((Rectangle){ 120, 340, 200, 30 }, minor_input, MAX_NAME_LEN, 4);

        GuiLabel((Rectangle){ 10, 380, 100, 30 }, "OE1:");
        ManageTextBoxFocus((Rectangle){ 120, 380, 200, 30 }, oe1_input, MAX_NAME_LEN, 5);

        GuiLabel((Rectangle){ 10, 420, 100, 30 }, "OE2:");
        ManageTextBoxFocus((Rectangle){ 120, 420, 200, 30 }, oe2_input, MAX_NAME_LEN, 6);

        // Save button to apply changes
        bool save_button_pressed = GuiButton((Rectangle){ 10, 460, 100, 30 }, "Save");

        if (save_button_pressed) {
            // Convert edited data and save it back to the student
            found_student.roll_number = atoi(roll_number_input);
            strncpy(found_student.name, name_input, MAX_NAME_LEN - 1);
            found_student.name[MAX_NAME_LEN - 1] = '\0';

            strncpy(found_student.major, major_input, MAX_NAME_LEN - 1);
            found_student.major[MAX_NAME_LEN - 1] = '\0';

            strncpy(found_student.minor, minor_input, MAX_NAME_LEN - 1);
            found_student.minor[MAX_NAME_LEN - 1] = '\0';

            strncpy(found_student.oe1, oe1_input, MAX_NAME_LEN - 1);
            found_student.oe1[MAX_NAME_LEN - 1] = '\0';

            strncpy(found_student.oe2, oe2_input, MAX_NAME_LEN - 1);
            found_student.oe2[MAX_NAME_LEN - 1] = '\0';

            // Update the student data
            edit_student(found_student.uid, found_student);
        }
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