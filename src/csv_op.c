#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/csv_op.h"

#define CSV_FILE "students.csv"

// Function to save a student to the CSV file
void save_student_to_csv(Student student) {
    FILE *file = fopen(CSV_FILE, "a");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    fprintf(file, "%d,%d,%s,%s,%s,%s,%s\n", student.uid, student.roll_number,
            student.name, student.major, student.minor, student.oe1, student.oe2);
    fclose(file);
}

// Function to remove a student from the CSV file by UID
void remove_student_from_csv(int uid) {
    FILE *file = fopen(CSV_FILE, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (file == NULL || temp == NULL) {
        perror("Could not open file");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%d,%99[^,],%99[^,],%99[^,],%99[^,],%99[^,\n]",
               &student.uid, &student.roll_number, student.name,
               student.major, student.minor, student.oe1, student.oe2);

        if (student.uid != uid) {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);
    remove(CSV_FILE);
    rename("temp.csv", CSV_FILE);
}

// Function to load all students from the CSV file
Student* load_all_students(int *count) {
    FILE *file = fopen(CSV_FILE, "r");
    if (file == NULL) {
        perror("Could not open file");
        return NULL;
    }

    Student *students = malloc(sizeof(Student) * 100); // Allocate memory for up to 100 students initially
    *count = 0;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%d,%99[^,],%99[^,],%99[^,],%99[^,],%99[^,\n]",
               &students[*count].uid, &students[*count].roll_number, students[*count].name,
               students[*count].major, students[*count].minor, students[*count].oe1, students[*count].oe2);
        (*count)++;
    }

    fclose(file);
    return students;
}

// Function to update a student's data in the CSV file
void update_student_in_csv(int uid, Student new_data) {
    FILE *file = fopen(CSV_FILE, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (file == NULL || temp == NULL) {
        perror("Could not open file");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%d,%99[^,],%99[^,],%99[^,],%99[^,],%99[^,\n]",
               &student.uid, &student.roll_number, student.name,
               student.major, student.minor, student.oe1, student.oe2);

        if (student.uid == uid) {
            fprintf(temp, "%d,%d,%s,%s,%s,%s,%s\n", new_data.uid, new_data.roll_number,
                    new_data.name, new_data.major, new_data.minor, new_data.oe1, new_data.oe2);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);
    remove(CSV_FILE);
    rename("temp.csv", CSV_FILE);
}
