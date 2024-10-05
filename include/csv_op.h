#ifndef CSV_OP_H
#define CSV_OP_H

#include "student_op.h"

// Function prototypes for CSV operations
void save_student_to_csv(Student student);
void remove_student_from_csv(int uid);
Student* load_all_students(int *count);
void update_student_in_csv(int uid, Student new_data);

#endif // CSV_OP_H
