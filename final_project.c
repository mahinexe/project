#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_CREDITS 14
#define MAX 50
int totalCredits = 0;

typedef struct
{
    char name[100];
    char id[14];
    char password[100];
    int creditsCompleted;
} Student;

typedef struct
{
    char code[10];
    char title[100];
    int credits;
    char prototype[20];
    char day[10];
    char time[20];
} Course;

typedef struct
{
    Course course;
    char type[7];
} operation;

typedef struct
{
    operation a[MAX];
    int top;
} stck;

stck *undostack;
stck *redostack;

typedef struct RegisteredCourse
{
    Course course;
    struct RegisteredCourse *next;
} regCourse;
regCourse *head = NULL;

void ini(stck *s)
{
    s->top = -1;
}

int isEMPTY(stck *s)
{
    return s->top == -1;
}

int isFULL(stck *s)
{
    return s->top == MAX - 1;
}

void push(stck *s, const char *type, Course course)
{
    if (isFULL(s))
    {
        printf("Stack Overflow\n");
        return;
    }
    s->top++;
    strncpy(s->a[s->top].type, type, 6); // Use strncpy for safe string copy
    // s->a[s->top].type[6] = '\0';         // Ensure null-termination
    s->a[s->top].course = course;
}

operation pop(stck *s)
{
    operation default_op = {0};
    if (isEMPTY(s))
    {
        printf("Stack Underflow\n");
        return default_op;
    }
    return s->a[s->top--];
}

operation peek(stck *s)
{
    operation default_op = {0};
    if (isEMPTY(s))
    {
        printf("Stack Underflow\n");
        return default_op;
    }
    return s->a[s->top];
}

void convert_upper(char *code)
{
    int i = 0;
    while (code[i] != '\0')
    {
        code[i] = toupper(code[i]);
        i++;
    }
}

Course allCourses[] =
{
    {"CSE161", "Programming Language I", 3, "Theory", "Monday", "10:00 AM - 11:30 AM"},
    {"CSE162", "Programming Language I Lab", 1, "Practical", "Wednesday", "2:00 PM - 4:00 PM"},
    {"CSE181", "Discrete Mathematics", 3, "Theory", "Tuesday", "9:00 AM - 10:30 AM"},
    {"CSE241", "Data Structures", 3, "Theory", "Thursday", "11:00 AM - 12:30 PM"},
    {"CSE242", "Data Structures Lab", 1, "Practical", "Friday", "3:00 PM - 5:00 PM"},
    {"CSE261", "Numerical Methods", 3, "Theory", "Monday", "3:00 PM - 4:30 PM"},
    {"CSE263", "Digital Logic Design", 3, "Theory", "Tuesday", "10:00 AM - 11:30 AM"},
    {"CSE264", "Digital Logic Design Lab", 1, "Practical", "Wednesday", "4:00 PM - 6:00 PM"},
    {"CSE265", "Algorithm", 3, "Theory", "Thursday", "2:00 PM - 3:30 PM"},
    {"CSE266", "Algorithm Lab", 1, "Practical", "Friday", "9:00 AM - 11:00 AM"},
    {"CSE282", "Programming Language II Lab", 1, "Practical", "Tuesday", "3:00 PM - 5:00 PM"},
    {"CSE341", "Computer Networking", 3, "Theory", "Monday", "1:00 PM - 2:30 PM"},
    {"CSE342", "Computer Networking Lab", 1, "Practical", "Wednesday", "10:00 AM - 12:00 PM"},
    {"CSE343", "Computer Architecture", 3, "Theory", "Thursday", "3:00 PM - 4:30 PM"},
    {"CSE345", "Information & Software Engineering", 3, "Theory", "Friday", "1:00 PM - 2:30 PM"},
    {"CSE346", "Information & Software Engineering Lab", 1, "Practical", "Tuesday", "9:00 AM - 11:00 AM"},
    {"CSE347", "Advanced Networking", 3, "Theory", "Monday", "11:00 AM - 12:30 PM"},
    {"CSE348", "Advanced Networking Lab", 1, "Practical", "Wednesday", "1:00 PM - 3:00 PM"},
    {"CSE349", "Mathematical Analysis for Computer Science", 3, "Theory", "Thursday", "9:00 AM - 10:30 AM"},
    {"CSE351", "Advanced Java", 3, "Theory", "Friday", "11:00 AM - 12:30 PM"},
    {"CSE352", "Advanced Java Lab", 1, "Practical", "Monday", "2:00 PM - 4:00 PM"},
    {"CSE353", "Introduction to Data Mining", 3, "Theory", "Tuesday", "11:00 AM - 12:30 PM"},
    {"CSE355", "Image Processing", 3, "Theory", "Thursday", "2:00 PM - 3:30 PM"},
    {"CSE359", "Graph Theory", 3, "Theory", "Wednesday", "10:00 AM - 11:30 AM"},
    {"CSE361", "Operating Systems", 3, "Theory", "Monday", "4:00 PM - 5:30 PM"},
    {"CSE362", "Operating Systems Lab", 1, "Practical", "Tuesday", "1:00 PM - 3:00 PM"},
    {"CSE363", "Microprocessor Design", 3, "Theory", "Wednesday", "2:00 PM - 3:30 PM"},
    {"CSE364", "Microprocessor Design Lab", 1, "Practical", "Friday", "9:00 AM - 11:00 AM"},
    {"CSE365", "Artificial Intelligence", 3, "Theory", "Monday", "9:00 AM - 10:30 AM"},
    {"CSE375", "Advanced Algorithm", 3, "Theory", "Tuesday", "4:00 PM - 5:30 PM"},
    {"CSE376", "Advanced Algorithm Lab", 1, "Practical", "Wednesday", "3:00 PM - 5:00 PM"},
    {"CSE381", "Introduction to Embedded Systems", 3, "Theory", "Thursday", "11:00 AM - 12:30 PM"},
    {"CSE382", "Introduction to Embedded Systems Lab", 1, "Practical", "Friday", "1:00 PM - 3:00 PM"},
    {"CSE383", "Database Design", 3, "Theory", "Monday", "2:00 PM - 3:30 PM"},
    {"CSE384", "Database Design Lab", 1, "Practical", "Wednesday", "9:00 AM - 11:00 AM"},
    {"CSE441", "Theory of Computing", 3, "Theory", "Tuesday", "10:00 AM - 11:30 AM"},
    {"CSE443", "Computer Graphics & Animation", 3, "Theory", "Thursday", "1:00 PM - 2:30 PM"},
    {"CSE444", "Computer Graphics & Animation Lab", 1, "Practical", "Friday", "10:00 AM - 12:00 PM"},
    {"CSE445", "Smart Device App Development", 3, "Theory", "Monday", "11:00 AM - 12:30 PM"},
    {"CSE457", "Advanced Embedded Systems", 3, "Theory", "Tuesday", "1:00 PM - 2:30 PM"},
    {"CSE458", "Advanced Embedded Systems Lab", 1, "Practical", "Wednesday", "11:00 AM - 1:00 PM"},
    {"CSE459", "Research Methodology", 3, "Theory", "Thursday", "3:00 PM - 4:30 PM"},
    {"CSE465", "Parallel and Distributed Computing", 3, "Theory", "Friday", "2:00 PM - 3:30 PM"},
    {"CSE466", "Parallel and Distributed Computing Lab", 1, "Practical", "Tuesday", "9:00 AM - 11:00 AM"},
    {"CSE467", "E-Commerce & E-Governance", 3, "Theory", "Wednesday", "1:00 PM - 2:30 PM"},
    {"CSE469", "Compiler Construction", 3, "Theory", "Thursday", "10:00 AM - 11:30 AM"},
    {"CSE471", "Web and Internet Programming", 3, "Theory", "Monday", "3:00 PM - 4:30 PM"},
    {"CSE472", "Web and Internet Programming Lab", 1, "Practical", "Wednesday", "2:00 PM - 4:00 PM"},
    {"CSE473", "Software Development and Project Management", 3, "Theory", "Tuesday", "11:00 AM - 12:30 PM"},
    {"CSE474", "Software Development and Project Management Lab", 1, "Practical", "Friday", "9:00 AM - 11:00 AM"},
    {"CSE475", "Management Information System", 3, "Theory", "Thursday", "2:00 PM - 3:30 PM"},
    {"CSE477", "Cloud Computing", 3, "Theory", "Monday", "10:00 AM - 11:30 AM"},
    {"CSE479", "Cryptography & Network Security", 3, "Theory", "Tuesday", "4:00 PM - 5:30 PM"},
    {"CSE489", "Internship", 3, "Theory", "Wednesday", "11:00 AM - 12:30 PM"},
    {"CSE491", "Project", 3, "Theory", "Thursday", "9:00 AM - 10:30 AM"},
    {"ECO461", "Introduction to Economics", 3, "Theory", "Friday", "2:00 PM - 3:30 PM"},
    {"EEE181", "Electrical Circuits Design I", 3, "Theory", "Monday", "1:00 PM - 2:30 PM"},
    {"EEE182", "Electrical Circuits Design I Lab", 1, "Practical", "Wednesday", "4:00 PM - 6:00 PM"},
    {"EEE241", "Electronic Devices & Circuits I", 3, "Theory", "Tuesday", "9:00 AM - 10:30 AM"},
    {"EEE242", "Electronic Devices & Circuits I Lab", 1, "Practical", "Friday", "10:00 AM - 12:00 PM"},
    {"ENG102", "Intermediate English Skills", 3, "English", "Tuesday", "9:00 AM - 11:00 AM"},
    {"ENG103", "Advanced English Skills", 3, "English", "Wednesday", "9:00 AM - 11:00 AM"},
    {"ENG105", "Public Speaking", 3, "English", "Thursday", "10:00 AM - 12:00 PM"},
    {"ETE281", "Communication Theory", 3, "Theory", "Monday", "1:00 PM - 3:00 PM"},
    {"ETE282", "Communication Lab", 1, "Practical", "Tuesday", "1:00 PM - 3:00 PM"},
    {"ETE357", "Telecommunication Engineering", 3, "Theory", "Wednesday", "1:00 PM - 3:00 PM"},
    {"ETE358", "Telecommunication Engineering Lab", 1, "Practical", "Thursday", "1:00 PM - 3:00 PM"},
    {"ETE399", "Digital Communication", 3, "Theory", "Monday", "3:00 PM - 5:00 PM"},
    {"ETE451", "Digital Signal Processing", 3, "Theory", "Tuesday", "3:00 PM - 5:00 PM"},
    {"ETE452", "Digital Signal Processing Lab", 1, "Practical", "Thursday", "3:00 PM - 5:00 PM"},
    {"MAT141", "Differential & Integral Calculus", 3, "Theory", "Monday", "9:00 AM - 11:00 AM"},
    {"MAT161", "Coordinate Geometry and Vector Analysis", 3, "Theory", "Wednesday", "9:00 AM - 11:00 AM"},
    {"MAT241", "Complex Variables and Transforms", 3, "Theory", "Thursday", "9:00 AM - 11:00 AM"},
    {"MAT261", "Linear Algebra and Matrices", 3, "Theory", "Friday", "9:00 AM - 11:00 AM"},
    {"MGT281", "Introduction to Business & Management", 3, "Theory", "Monday", "3:00 PM - 5:00 PM"},
    {"PHY161", "Physics I", 3, "Theory", "Tuesday", "9:00 AM - 11:00 AM"},
    {"PHY181", "Physics II", 3, "Theory", "Wednesday", "1:00 PM - 3:00 PM"},
    {"PHY182", "Physics II Lab", 1, "Practical", "Thursday", "1:00 PM - 3:00 PM"},
    {"SOC341", "Engineering Ethics", 3, "Theory", "Friday", "1:00 PM - 3:00 PM"},
    {"STA281", "Statistical Methods & Probability", 3, "Theory", "Friday", "3:00 PM - 5:00 PM"}
};

// Linked list to store registered courses

int checkConflict(char *day, char *time)
{
    regCourse *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->course.day, day) == 0 && strcmp(temp->course.time, time) == 0)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void addCourseByCode(char *code)
{

    if (totalCredits >= MAX_CREDITS)
    {
        printf("You have reached the maximum credit limit.\n");
        return;
    }

    // Find the course details by the code
    convert_upper(code);
    int found = 0;
    Course selectedCourse;
    for (int i = 0; i < sizeof(allCourses) / sizeof(allCourses[0]); i++)
    {
        if (strcmp(allCourses[i].code, code) == 0)
        {
            selectedCourse = allCourses[i];
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Course code not found!\n");
        return;
    }

    if (totalCredits + selectedCourse.credits > MAX_CREDITS)
    {
        printf("Adding this course will exceed the maximum credit limit.\n");
        return;
    }

    if (checkConflict(selectedCourse.day, selectedCourse.time))
    {
        printf("Conflict detected! Cannot add course %s on %s at %s.\n", code, selectedCourse.day, selectedCourse.time);
        return;
    }

    // Create a new registered course node
    regCourse *newCourse = (regCourse *)malloc(sizeof(regCourse));
    newCourse->course = selectedCourse;
    newCourse->next = NULL;

    // Add the course to the linked list
    if (head == NULL)
    {
        head = newCourse;
    }
    else
    {
        regCourse *temp = head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newCourse;
    }
    totalCredits += selectedCourse.credits;
    printf("Course %s (%s) added successfully on %s at %s.\n", code, selectedCourse.title, selectedCourse.day, selectedCourse.time);
    push(undostack, "add", selectedCourse);
}

// Function to remove a course by its code
void removeCourse(char *code)
{
    if (head == NULL)
    {
        printf("You haven't registered to any course yet.\n");
        return;
    }

    convert_upper(code);

    regCourse *temp = head;
    regCourse *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->course.code, code) == 0)
        {
            if (prev == NULL)
            {
                head = temp->next; // Course to be removed is at the head
            }
            else
            {
                prev->next = temp->next; // Remove the course from the linked list
            }

            totalCredits -= temp->course.credits;

            printf("Course %s (%s) removed successfully from %s at %s.\n", temp->course.code, temp->course.title, temp->course.day, temp->course.time);

            // Push the removed course to the undo stack
            push(undostack, "remove", temp->course);

            free(temp);
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Course code not found in your registered courses!\n");
}
void undo()
{
    if (isEMPTY(undostack))
    {
        printf("Nothing to undo.\n");
        return;
    }

    operation lastOp = pop(undostack); // Get the last operation

    // If the operation was to add a course, remove it
    if (strcmp(lastOp.type, "add") == 0)
    {
        regCourse *temp = head;
        regCourse *prev = NULL;
        while (temp != NULL)
        {
            if (strcmp(temp->course.code, lastOp.course.code) == 0)
            {
                if (prev == NULL)
                    head = temp->next;
                else
                    prev->next = temp->next;
                totalCredits -= temp->course.credits;
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        printf("Undo: Course %s removed.\n", lastOp.course.code);
        push(redostack, "remove", lastOp.course); // Push to redo stack
    }
    // If the operation was to remove a course, add it back
    else if (strcmp(lastOp.type, "remove") == 0)
    {
        regCourse *newCourse = (regCourse *)malloc(sizeof(regCourse));
        newCourse->course = lastOp.course;
        newCourse->next = head;
        head = newCourse;
        totalCredits += lastOp.course.credits;
        printf("Undo: Course %s added.\n", lastOp.course.code);
        push(redostack, "add", lastOp.course); // Push to redo stack
    }
}

void redo()
{
    if (isEMPTY(redostack))
    {
        printf("Nothing to redo.\n");
        return;
    }

    operation lastOp = pop(redostack); // Get the last undone operation

    // If the operation was to add a course, add it back
    if (strcmp(lastOp.type, "remove") == 0)
    {
        regCourse *newCourse = (regCourse *)malloc(sizeof(regCourse));
        newCourse->course = lastOp.course;
        newCourse->next = head;
        head = newCourse;
        totalCredits += lastOp.course.credits;
        printf("Redo: Course %s added.\n", lastOp.course.code);
        push(undostack, "add", lastOp.course); // Push back to undo stack
    }
    // If the operation was to remove a course, remove it again
    else if (strcmp(lastOp.type, "add") == 0)
    {
        regCourse *temp = head;
        regCourse *prev = NULL;
        while (temp != NULL)
        {
            if (strcmp(temp->course.code, lastOp.course.code) == 0)
            {
                if (prev == NULL)
                    head = temp->next;
                else
                    prev->next = temp->next;
                totalCredits -= temp->course.credits;
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        printf("Redo: Course %s removed.\n", lastOp.course.code);
        push(undostack, "remove", lastOp.course); // Push back to undo stack
    }
}

// Function to display all registered courses
void displayRegisteredCourses()
{
    if (head == NULL)
    {
        printf("No courses registered.\n");
        return;
    }
    printf("\nRegistered Courses:\n\n");
    printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
    printf("| Course Code  | Course Title                                       | Credits | Course Prototype   | Day                    | Time                     |\n");
    printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
    regCourse *temp = head;
    while (temp != NULL)
    {
        printf("| %-12s | %-50s | %-7d | %-18s | %-22s | %-24s |\n", temp->course.code, temp->course.title, temp->course.credits, temp->course.prototype, temp->course.day, temp->course.time);
        temp = temp->next;
    }
    printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
}

void displayAllCourses(Course *courses, int size)
{
    printf("\nAll Courses:\n\n");
    printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
    printf("| Course Code  | Course Title                                       | Credits | Course Prototype   | Day                    | Time                     |\n");
    printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
    for (int i = 0; i < size; i++)
    {
        printf("| %-12s | %-50s | %-7d | %-18s | %-22s | %-24s |\n",
               courses[i].code, courses[i].title, courses[i].credits, courses[i].prototype, courses[i].day, courses[i].time);
    }
    printf("+------------+------------------------------------------------------+---------+--------------------+------------------------+--------------------------+\n");
}

int searchCourseByCode(Course *courses, int size, char *code)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(courses[i].code, code) == 0)
            return i;
    }
    return -1;
}

void searchAndDisplayCourse(Course *courses, int size, char *code)
{
    convert_upper(code);

    int i = searchCourseByCode(courses, size, code);
    if (i != -1)
    {
        printf("Course found...\n");
        printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
        printf("| Course Code  | Course Title                                       | Credits | Course Prototype   | Day                    | Time                   |\n");
        printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
        printf("| %-12s | %-50s | %-7d | %-18s | %-22s | %-24s |\n",
               courses[i].code, courses[i].title, courses[i].credits, courses[i].prototype,
               courses[i].day, courses[i].time);
        printf("+------------+------------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
    }
    else
    {
        printf("No course found with the code: %s\n", code);
    }
}

int searchCourseByTitle(Course *courses, int size, char *code)
{
    for (int i = 0; i < size; i++)
    {
        if (strcasecmp(allCourses[i].title, code) == 0)
            return i;
    }
    return -1;
}

void searchTitleAndDisplayCourse(Course *courses, int size, char *code)
{
    // convert_upper(code);

    int i = searchCourseByTitle(courses, size, code);
    if (i != -1)
    {
        printf("Course found...\n");
        printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
        printf("| Course Code  | Course Title                                       | Credits | Course Prototype   | Day                    | Time                   |\n");
        printf("+--------------+----------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
        printf("| %-12s | %-50s | %-7d | %-18s | %-22s | %-24s |\n",
               courses[i].code, courses[i].title, courses[i].credits, courses[i].prototype,
               courses[i].day, courses[i].time);
        printf("+------------+------------------------------------------------------+---------+--------------------+------------------------+------------------------+\n");
    }
    else
    {
        printf("No course found with the code: %s\n", code);
    }
}

void displaySuggestedCourses(int semester)
{
    printf("\nSuggested Courses for Semester %d:\n", semester);
    printf("+------------+----------------------------------------------------+---------+--------------------+---------------------+---------------------+\n");
    printf("| Course Code| Course Title                                       | Credits | Course Prototype   | Day                 | Time                |\n");
    printf("+------------+----------------------------------------------------+---------+--------------------+---------------------+---------------------+\n");

    if (semester == 2)
    {
        printf("| ENG102     | Intermediate Composition                           | 3       | Theory             | Monday              | 10:00 AM - 11:30 AM |\n");
        printf("| PHY161     | Physics I                                          | 3       | Theory             | Tuesday             | 9:00 AM - 10:30 AM  |\n");
        printf("| CSE161     | Programming Language I                             | 3       | Theory             | Monday              | 10:00 AM - 11:30 AM |\n");
        printf("| CSE162     | Programming Language I Lab                         | 1       | Practical          | Wednesday           | 2:00 PM - 4:00 PM   |\n");
        printf("| MAT161     | Coordinate Geometry and Vector Analysis            | 3       | Theory             | Tuesday             | 3:00 PM - 4:30 PM   |\n");
        printf("\n  Total Credits: 13\n");
    }
    else if (semester == 3)
    {
        printf("| CSE181     | Discrete Mathematics                               | 3       | Theory             | Tuesday             | 9:00 AM - 10:30 AM  |\n");
        printf("| ENG103     | Advanced English Skills                            | 3       | Theory             | Thursday            | 10:00 AM - 11:30 AM |\n");
        printf("| EEE181     | Electrical Circuits Design I                       | 3       | Theory             | Monday              | 2:00 PM - 3:30 PM   |\n");
        printf("| EEE182     | Electrical Circuits Design I Lab                   | 1       | Practical          | Wednesday           | 3:00 PM - 5:00 PM   |\n");
        printf("| PHY181     | Physics II                                         | 3       | Theory             | Tuesday             | 10:00 AM - 11:30 AM |\n");
        printf("| PHY182     | Physics II Lab                                     | 1       | Practical          | Thursday            | 1:00 PM - 3:00 PM   |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 4)
    {
        printf("| CSE241     | Data Structures                                    | 3       | Theory             | Thursday            | 11:00 AM - 12:30 PM |\n");
        printf("| CSE242     | Data Structures Lab                                | 1       | Practical          | Friday              | 3:00 PM - 5:00 PM   |\n");
        printf("| ENG105     | Public Speaking                                    | 3       | Theory             | Wednesday           | 10:00 AM - 11:30 AM |\n");
        printf("| MAT241     | Complex Variables and Transforms                   | 3       | Theory             | Tuesday             | 3:00 PM - 4:30 PM   |\n");
        printf("| EEE241     | Electronic Devices & Circuits I                    | 3       | Theory             | Monday              | 1:00 PM - 2:30 PM   |\n");
        printf("| EEE242     | Electronic Devices & Circuits I Lab                | 1       | Practical          | Friday              | 10:00 AM - 12:00 PM |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 5)
    {
        printf("| CSE261     | Numerical Methods                                  | 3       | Theory             | Monday              | 3:00 PM - 4:30 PM   |\n");
        printf("| CSE263     | Digital Logic Design                               | 3       | Theory             | Tuesday             | 10:00 AM - 11:30 AM |\n");
        printf("| CSE264     | Digital Logic Design Lab                           | 1       | Practical          | Wednesday           | 4:00 PM - 6:00 PM   |\n");
        printf("| MAT261     | Linear Algebra and Matrices                        | 3       | Theory             | Thursday            | 2:00 PM - 3:30 PM   |\n");
        printf("| CSE265     | Algorithm                                          | 3       | Theory             | Thursday            | 11:00 AM - 12:30 PM |\n");
        printf("| CSE266     | Algorithm Lab                                      | 1       | Practical          | Friday              | 9:00 AM - 11:00 AM  |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 6)
    {
        printf("| MGT281     | Introduction to Business & Management              | 3       | Theory             | Tuesday             | 4:00 PM - 5:30 PM   |\n");
        printf("| CSE281     | Introduction to Programming Language II (Java)     | 3       | Theory             | Monday              | 10:00 AM - 11:30 AM |\n");
        printf("| CSE282     | Introduction to Programming Language II (Java) Lab | 1       | Practical          | Wednesday           | 2:00 PM - 4:00 PM   |\n");
        printf("| ETE281     | Communication Theory                               | 3       | Theory             | Thursday            | 3:00 PM - 4:30 PM   |\n");
        printf("| ETE282     | Communication Lab                                  | 1       | Practical          | Friday              | 11:00 AM - 1:00 PM  |\n");
        printf("| STA281     | Statistical Methods & Probability                  | 3       | Theory             | Monday              | 3:00 PM - 4:30 PM   |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 7)
    {
        printf("| CSE341     | Computer Networking                                | 3       | Theory             | Monday              | 11:00 AM - 12:30 PM |\n");
        printf("| CSE342     | Computer Networking Lab                            | 1       | Practical          | Wednesday           | 10:00 AM - 12:00 PM |\n");
        printf("| CSE343     | Computer Architecture                              | 3       | Theory             | Thursday            | 3:00 PM - 4:30 PM   |\n");
        printf("| CSE345     | Information System & Software Engineering          | 3       | Theory             | Tuesday             | 2:00 PM - 3:30 PM   |\n");
        printf("| CSE346     | Information System & Software Engineering Lab      | 1       | Practical          | Friday              | 9:00 AM - 11:00 AM  |\n");
        printf("| SOC341     | Engineering Ethics                                 | 3       | Theory             | Monday              | 1:00 PM - 2:30 PM   |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 8)
    {
        printf("| CSE441     | Theory of Computing                                | 3       | Theory             | Tuesday             | 10:00 AM - 11:30 AM |\n");
        printf("| CSE383     | Database Design                                    | 3       | Theory             | Wednesday           | 2:00 PM - 3:30 PM   |\n");
        printf("| CSE384     | Database Design Lab                                | 1       | Practical          | Friday              | 3:00 PM - 5:00 PM   |\n");
        printf("| CSE363     | Microprocessor Design & Assembly Language          | 3       | Theory             | Thursday            | 1:00 PM - 2:30 PM   |\n");
        printf("| CSE364     | Microprocessor Design & Assembly Language Lab      | 1       | Practical          | Monday              | 2:00 PM - 4:00 PM   |\n");
        printf("| CSE365     | Artificial Intelligence                            | 3       | Theory             | Friday              | 11:00 AM - 12:30 PM |\n");
        printf("\n  Total Credits: 14\n");
    }
    else if (semester == 9)
    {
        printf("| CSE381     | Introduction to Embedded Systems                   | 3       | Theory             | Thursday            | 9:00 AM - 10:30 AM  |\n");
        printf("| CSE382     | Introduction to Embedded Systems Lab               | 1       | Practical          | Friday              | 2:00 PM - 4:00 PM   |\n");
        printf("| CSE361     | Operating Systems                                  | 3       | Theory             | Monday              | 3:00 PM - 4:30 PM   |\n");
        printf("| CSE362     | Operating Systems Lab                              | 1       | Practical          | Tuesday             | 4:00 PM - 6:00 PM   |\n");
        printf("| CSE443     | Computer Graphics & Animation                      | 3       | Theory             | Wednesday           | 10:00 AM - 11:30 AM |\n");
        printf("\n  Total Credits: 11\n");
    }

    printf("+------------+----------------------------------------------------+---------+--------------------+---------------------+---------------------+\n");
}

int isValidName(char *name)
{
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (isdigit(name[i]))
        {
            return 0;
        }
    }
    return 1;
}

int isValidID(char *id)
{
    if (strlen(id) != 13)
        return 0;
    for (int i = 0; i < 13; i++)
    {
        if (!isdigit(id[i]))
        {
            return 0;
        }
    }
    return 1;
}

int findStudentByID(Student *students, int size, char *id)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(students[i].id, id) == 0)
        {
            return i;
        }
    }
    return -1;
}

void sort(Course *a, int n)
{
    Course title;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcmp(a[j].title, a[j + 1].title) > 0)
            {
                title = a[j];
                a[j] = a[j + 1];
                a[j + 1] = title;
            }
        }
    }
}

int main()
{
    Student students[] =
    {
        {"Al-Mahin", "2023200000334", "1234", 38},
        {"Nahid Hasan Emon", "2023200000153", "1234", 38},
        {"Mst. Ratna", "2023200000144", "1234", 38},
        {"Md. Imdadul Hossain Ratul", "2023100000541", "1234", 51},
        {"Munia Muzumder", "2023200000123", "1234", 38}
    };
    int studentCount = sizeof(students) / sizeof(students[0]);

    char id[14];
    int StudentID;

    printf("\n");

    printf("WELCOME TO THE COURSE REGISTRATION SYSTEM\n");
    printf("\nLog in with your Student ID & Password\n");

    while (1)
    {
        printf("Enter your Student ID: ");
        scanf("%s", id);

        if (isValidID(id))
        {
            StudentID = findStudentByID(students, studentCount, id);
            if (StudentID != -1)
            {
                char enteredPassword[5];
                int attempts = 3;

                while (attempts > 0)
                {
                    printf("Enter your password: ");
                    scanf("%s", enteredPassword);

                    if (strcmp(enteredPassword, students[StudentID].password) == 0)
                    {

                        break;
                    }
                    else
                    {
                        printf("Incorrect password. %d attempts remaining.\n", --attempts);
                    }
                }

                if (attempts == 0)
                {
                    printf("Too many incorrect password attempts. Exiting.\n");
                    return 1;
                }
                else
                {

                    break;
                }
            }
            else
            {
                printf("Error: Invalid ID! Your ID is not in our server.\n\n");
            }
        }
        else
        {
            printf("Error: Invalid ID! ID must be 13 digits and contain only digits.\n\n");
        }
    }
    printf("\nStudent Information:\n");
    printf("Name: %s\n", students[StudentID].name);
    printf("Student ID: %s\n", students[StudentID].id);
    printf("Credits Completed: %d\n", students[StudentID].creditsCompleted);

    int semester;
    while (1)
    {
        printf("\nEnter the semester you are registering for: ");
        scanf("%d", &semester);
        if (semester >= 2 && semester <= 9)
        {
            break;
        }
        else
        {
            printf("Error: Semester must be between 2 and 9.\n");
        }
    }

    undostack = (stck *)malloc(sizeof(stck));
    redostack = (stck *)malloc(sizeof(stck));

    while (1)
    {
        printf("\nCourse Registration System\n");
        printf("1. View all courses\n");
        printf("2. Suggested courses for semester %d\n", semester);
        printf("3. Add Course\n");
        printf("4. Remove Course\n");
        printf("5. Display Registered Courses\n");
        printf("6. Search for a course by code\n");
        printf("7. Search for a course by Title\n");
        printf("8. Sort all courses in dictionary order\n");
        printf("9. Undo last action\n");
        printf("10. Redo last action\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);
        char code[10];
        if (choice == 1)
        {
            displayAllCourses(allCourses, sizeof(allCourses) / sizeof(allCourses[0]));
        }
        else if (choice == 2)
        {
            displaySuggestedCourses(semester);
        }
        else if (choice == 3)
        {
            printf("Enter course code to add: ");
            scanf("%s", code);
            addCourseByCode(code);
        }
        else if (choice == 4)
        {
            printf("Enter course code to remove: ");
            scanf("%s", code);

            removeCourse(code);
        }
        else if (choice == 5)
        {
            displayRegisteredCourses();
        }
        else if (choice == 6)
        {
            printf("Enter course code to search: ");
            scanf("%s", code);
            searchAndDisplayCourse(allCourses, sizeof(allCourses) / sizeof(allCourses[0]), code);
        }
        else if (choice == 7)
        {
            printf("Enter course title to search: ");
            scanf(" %[^\n]", code);
            searchTitleAndDisplayCourse(allCourses, sizeof(allCourses) / sizeof(allCourses[0]), code);
        }
        else if (choice == 8)
        {
            sort(allCourses, sizeof(allCourses) / sizeof(allCourses[0]));
            displayAllCourses(allCourses, sizeof(allCourses) / sizeof(allCourses[0]));
        }
        else if (choice == 9)
        {
            undo();
        }
        else if (choice == 10)
        {
            redo();
        }
        else if (choice == 11)
        {
            printf("Exiting the program.....");
            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }


    return 0;
}
