#ifndef STUDENT_OP_H
#define STUDENT_OP_H

#define MAX_NAME_LEN 100
#define MAX_SUBJECT_LEN 100

typedef struct {
    int uid;                   // Unique identifier for the student
    int roll_number;           // Roll number of the student
    char name[MAX_NAME_LEN];   // Name of the student
    char major[MAX_SUBJECT_LEN];  // Major subject
    char minor[MAX_SUBJECT_LEN];  // Minor subject
    char oe1[MAX_SUBJECT_LEN];    // Open elective 1
    char oe2[MAX_SUBJECT_LEN];    // Open elective 2
} Student;

// Function prototypes for student operations
void add_student(Student student);
void remove_student(int uid);
Student* search_student(int uid);
void edit_student(int uid, Student new_data);
void list_all_students();

#endif // STUDENT_OP_H
