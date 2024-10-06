#ifndef UI_OP_H
#define UI_OP_H

#include "student_op.h"

// Function prototypes for UI operations
void DrawAddStudentUI(bool *add_button_pressed);
void DrawRemoveStudentUI(bool *remove_button_pressed);
void DrawSearchStudentUI(bool *search_button_pressed);
void DrawEditStudentUI(bool *edit_button_pressed, bool *save_button_pressed);
void DrawListStudentsUI();

#endif // UI_OP_H
