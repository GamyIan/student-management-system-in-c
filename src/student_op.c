#include <stdio.h>
#include <stdlib.h>
#include "../include/student_op.h"
#include "../include/csv_op.h"

// Function to add a student
void add_student(Student student) {
    save_student_to_csv(student);
    printf("Student with UID %d added.\n", student.uid);
}

// Function to remove a student by UID
void remove_student(int uid) {
    remove_student_from_csv(uid);
    printf("Student with UID %d removed.\n", uid);
}

// Function to search for a student by UID
Student* search_student(int uid) {
    int count = 0;
    Student* students = load_all_students(&count);

    for (int i = 0; i < count; i++) {
        if (students[i].uid == uid) {
            Student *found_student = malloc(sizeof(Student));
            *found_student = students[i];
            free(students);
            return found_student;
        }
    }

    free(students);
    printf("Student with UID %d not found.\n", uid);
    return NULL;
}

// Function to edit a student's data by UID
void edit_student(int uid, Student new_data) {
    update_student_in_csv(uid, new_data);
    printf("Student with UID %d edited.\n", uid);
}

// Function to list all students
void list_all_students() {
    int count = 0;
    Student* students = load_all_students(&count);

    if (count == 0) {
        printf("No students found.\n");
    } else {
        printf("Listing all students:\n");
        for (int i = 0; i < count; i++) {
            printf("UID: %d, Roll Number: %d, Name: %s, Major: %s, Minor: %s, OE1: %s, OE2: %s\n",
                   students[i].uid, students[i].roll_number, students[i].name,
                   students[i].major, students[i].minor, students[i].oe1, students[i].oe2);
        }
    }

    free(students);
}
