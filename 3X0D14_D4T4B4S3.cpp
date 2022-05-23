#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct Grade
{
    float gradeValue;
    char disciplineName[30];
    struct Grade* nextGrade;
} Grade;


typedef struct Student
{
    char facultyNumber[7];
    char firstName[15];
    char middleName[15];
    char lastName[15];
    Grade* gradeArray;
    int gradeCount;
    float averageGrade;
    struct Student* nextStudent;
} Student;


void quit(void);
void printMenu(void);
void printErrorMessage(void);
char choiceInput(char* choice);
bool isValidGrade(float grade);
bool isValidName(char* string);
void menu(Student** studentArray);
void readFromFile(Student** group);
void enterValidGrade(float* grade);
Grade* findGrade(Student* student);
void createGradeNode(Grade** head);
void findAvgGrade(Student* student);
void enterNames(Student* newStudent);
void editGrade(Student* studentArray);
void createStudentNode(Student** head);
void sortStudentsByGPA(Student** head);
void deleteGrade(Student* studentArray);
void createGrade(Student* studentArray);
void deleteAllGrades(Grade** gradeArray);
void printStudent(Student* studentArray);
void printStudents(Student* studentArray);
void createStudent(Student** studentArray);
void deleteStudent(Student** studentArray);
Student* findStudent(Student* studentArray);
void nameInput(const char* nameType, char* temp);
void writeToFile(Student* group, int studentCount);
void addGrade(Grade** grade, Student* currStudent);
void changeGrade(Student* currStudent, char* temp);
void studentDeletionSwitch(Student** currentStudent);
float printGroupGPA(Student* studentArray, bool print);
void printAllGradesByDiscipline(Student* studentArray);
void enterDisciplineName(const char* message, char* temp);
void gradeDeletionSwitch(Grade** currentGrade, bool print);
void readStudentNames(FILE* filePointer, char* destination);
bool isValidFacultyNumber(char* string, Student* studentArray);
bool isValidDisciplineName(Student* student, char* disciplineName);
void enterFacultyNumber(Student* newStudent, Student* studentArray);
Student* sortedInsert(Student* head, Student* sorted_head, Student* new_node);
void splitDisciplineAndGrade(FILE* string, char* disciplineName, float* grade);


int studentCount = 0;
const char* pathName = "3X0D14_D4T4B4S3.json";


int main(void)
{
    Student* studentArray = NULL;

    readFromFile(&studentArray); // DONE

    for (; ; )
    {
        menu(&studentArray);
    }

    return 0;
}


void menu(Student** studentArray)
{
    writeToFile(*studentArray, studentCount);  //DONE

    char choice[50];
    choiceInput(choice);

    switch (choice[0]) {
    case '1':
        createStudent(studentArray); //DONE
        break;
    case '2':
        printStudent(*studentArray); //DONE
        break;
    case '3':
        deleteStudent(studentArray); //DONE
        break;
    case '4':
        createGrade(*studentArray);  //DONE
        break;
    case '5':
        editGrade(*studentArray);    //DONE
        break;
    case '6':
        deleteGrade(*studentArray);  //DONE
        break;
    case '7':
        printStudents(*studentArray);// TODO SORTING
        break;
    case '8':
        printAllGradesByDiscipline(*studentArray); //DONE
        break;
    case '9':
        printGroupGPA(*studentArray, true); //DONE
        break;
    case '0':
        quit(); //DONE
        break;
    }

    return;
}


void printMenu(void)
{
    printf(" _________________________________ \n");
    printf("|         3X0D14 D4T4B4S3         |\n");
    printf("|_________________________________|\n");
    printf("|  1. |     Add a new student.    |\n");
    printf("|_____|___________________________|\n");
    printf("|  2. |      Find a student.      |\n");
    printf("|_____|___________________________|\n");
    printf("|  3. |     Remove a student.     |\n");
    printf("|_____|___________________________|\n");
    printf("|  4. |   Enter a student grade.  |\n");
    printf("|_____|___________________________|\n");
    printf("|  5. |   Edit a student grade.   |\n");
    printf("|_____|___________________________|\n");
    printf("|  6. |  Remove a student grade.  |\n");
    printf("|_____|___________________________|\n");
    printf("|  7. |    Print all students.    |\n");
    printf("|_____|___________________________|\n");
    printf("|  8. |   Search by discipline.   |\n");
    printf("|_____|___________________________|\n");
    printf("|  9. |     Get group's GPA.      |\n");
    printf("|_____|___________________________|\n");
    printf("|  0. |          Quit.            |\n");
    printf("|_____|___________________________|\n\n");
    printf("              Made by 3X0D14 C0RP.\n\n");
    printf("Enter a choice: ");
    return;
}


void readFromFile(Student** group)
{
    int i, j;
    int trash;
    Student* temp;
    Grade* tempGrade;

    FILE* filePointer = fopen(pathName, "r");
    fseek(filePointer, 0, SEEK_SET);

    if (filePointer == NULL)
    {
        fclose(filePointer);
        printf("File does not exist!\nInitiating Program...\n");
        return;
    }

    fseek(filePointer, 22, SEEK_SET);
    trash = fscanf(filePointer, "%d", &studentCount);

    if (studentCount == 0)
    {
        fclose(filePointer);
        return;
    }

    for (i = 0; i < studentCount; i++)
    {
        createStudentNode(group);
    }


    fseek(filePointer, 87, SEEK_CUR);

    for (temp = (*group), i = 0; i < studentCount; i++, temp = temp->nextStudent)
    {
        readStudentNames(filePointer, temp->facultyNumber);

        fseek(filePointer, 24, SEEK_CUR);
        readStudentNames(filePointer, temp->firstName);

        fseek(filePointer, 25, SEEK_CUR);
        readStudentNames(filePointer, temp->middleName);

        fseek(filePointer, 23, SEEK_CUR);
        readStudentNames(filePointer, temp->lastName);

        fseek(filePointer, 27, SEEK_CUR);
        trash = fscanf(filePointer, "%f", &(temp->averageGrade));

        fseek(filePointer, 27, SEEK_CUR);
        trash = fscanf(filePointer, "%d", &(temp->gradeCount));

        if (temp->gradeCount == 0)
        {
            if (i + 1 < studentCount)
            {
                fseek(filePointer, 88, SEEK_CUR);
            }
            continue;
        }

        for (j = 0; j < temp->gradeCount; j++)
        {
            createGradeNode(&(temp->gradeArray));
        }

        fseek(filePointer, 38, SEEK_CUR);

        for (tempGrade = temp->gradeArray, j = 0; j < temp->gradeCount; j++, tempGrade = tempGrade->nextGrade)
        {
            splitDisciplineAndGrade(filePointer, tempGrade->disciplineName, &tempGrade->gradeValue);
            fseek(filePointer, 14, SEEK_CUR);

            if (j + 1 < temp->gradeCount)
            {
                fseek(filePointer, 1, SEEK_CUR);
            }
        }

        if (i + 1 < studentCount)
        {
            fseek(filePointer, 49, SEEK_CUR);
        }
    }

    fclose(filePointer);

    return;
}


void writeToFile(Student* group, int studentCount)
{
    int i, j;
    Student* temp;
    Grade* tempGrade;
    FILE* filePointer = fopen(pathName, "w+");
    if (filePointer == NULL)
    {
        fclose(filePointer);
        printf("File does not exist!\n");
        quit();
    }

    fseek(filePointer, 0, SEEK_SET);
    fprintf(filePointer, "{\n");
    fprintf(filePointer, "   \"studentCount\":\"%d\",\n", studentCount);
    fprintf(filePointer, "   \"groupGPA\":\"%.2f\",\n", printGroupGPA(group, false));
    fprintf(filePointer, "   \"studentGroup\":{\n");

    if (!group)
    {
        fprintf(filePointer, "   }\n}");
        fclose(filePointer);
        return;
    }

    for (temp = group, i = 0; temp != NULL && i < studentCount; i++, temp = temp->nextStudent)
    {
        fprintf(filePointer, "      \"%d\":{\n", i);
        fprintf(filePointer, "         \"facultyNumber\":\"%s\",\n", temp->facultyNumber);
        fprintf(filePointer, "         \"firstName\":\"%s\",\n", temp->firstName);
        fprintf(filePointer, "         \"middleName\":\"%s\",\n", temp->middleName);
        fprintf(filePointer, "         \"lastName\":\"%s\",\n", temp->lastName);
        fprintf(filePointer, "         \"averageGrade\":\"%.2f\",\n", temp->averageGrade);
        fprintf(filePointer, "         \"gradeCount\":\"%d\",\n", temp->gradeCount);
        fprintf(filePointer, "         \"grades\":{\n");

        if (temp->gradeCount == 0)
        {
            fprintf(filePointer, "\n         }\n");
        }
        else if (temp->gradeCount == 1)
        {
            fprintf(filePointer, "            \"%s\":\"%.2f\"\n         }\n", temp->gradeArray->disciplineName, temp->gradeArray->gradeValue);
        }
        else
        {
            for (tempGrade = temp->gradeArray, j = 0; tempGrade->nextGrade != NULL && j < temp->gradeCount - 1; j++, tempGrade = tempGrade->nextGrade)
            {
                fprintf(filePointer, "            \"%s\":\"%.2f\",\n", tempGrade->disciplineName, tempGrade->gradeValue);
            }

            fprintf(filePointer, "            \"%s\":\"%.2f\"\n", tempGrade->disciplineName, tempGrade->gradeValue);
            fprintf(filePointer, "         }\n");
        }

        fprintf(filePointer, "      }\n");
        fprintf(filePointer, "   }\n");
        fprintf(filePointer, "}");

        if (i < studentCount - 1)
        {
            fseek(filePointer, -9, SEEK_CUR);
            fprintf(filePointer, ",\n");
        }
    }

    fclose(filePointer);
}


void splitDisciplineAndGrade(FILE* string, char* disciplineName, float* grade)
{
    int i, trash;
    char buffer[30];
    char buffer2[30];
    char gradeString[5] = { '\0' };

    trash = fscanf(string, "%s", buffer);
    buffer[strlen(buffer) - 1] = '\0';
    strcpy(buffer2, buffer);

    for (i = 0; buffer[i] != '"'; i++);
    buffer[i] = '\0';
    strcpy(disciplineName, buffer);

    for (i = 0; buffer2[i] != ':' && buffer2[i] != '\0'; i++);
    i += 2;

    for (int j = 0; j < 4; j++)
    {
        gradeString[j] = buffer[i++];
    }
    gradeString[4] = '\0';
    (*grade) = (float)atof(gradeString);

    return;
}


void createStudentNode(Student** head)
{
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent)
    {
        printf("readFromFile could not allocate memory!\n");
        exit(1);
    }

    newStudent->gradeArray = NULL;
    newStudent->gradeCount = 0;
    newStudent->averageGrade = 0;
    newStudent->nextStudent = (*head);
    (*head) = newStudent;

    return;
}


void createGradeNode(Grade** head)
{
    Grade* newGrade = (Grade*)malloc(sizeof(Grade));
    if (!newGrade)
    {
        printf("readFromFile could not allocate memory!\n");
        exit(1);
    }

    newGrade->nextGrade = (*head);
    (*head) = newGrade;

    return;
}


void sortStudentsByGPA(Student** head)
{
    Student* curr = (*head);
    Student* sorted_head = NULL;
    while (curr != NULL)
    {
        Student* currNext = curr->nextStudent;
        sorted_head = sortedInsert((*head), sorted_head, curr);
        curr = currNext;
    }

    (*head) = sorted_head;
    return;
}


Student* sortedInsert(Student* head, Student* sorted_head, Student* new_node)
{
    if (sorted_head == NULL || head->averageGrade <= new_node->averageGrade)
    {
        new_node->nextStudent = sorted_head;
        return new_node;
    }
    else
    {
        Student* curr = sorted_head;
        while (curr->nextStudent != NULL && curr->nextStudent->averageGrade > new_node->averageGrade)
        {
            curr = curr->nextStudent;
        }
        new_node->nextStudent = curr->nextStudent;
        curr->nextStudent = new_node;
    }
    return sorted_head;
}


char choiceInput(char* choice)
{
    printMenu();
    fflush(stdin);
    fgets(choice, 50, stdin);
    fflush(stdin);
    choice[strlen(choice) - 1] = '\0';
    if (strlen(choice) != 1 || (choice[0] < '0' || choice[0] > '9'))
    {
        printErrorMessage();
        choiceInput(choice);
    }

    return choice[0];
}


void readStudentNames(FILE* filePointer, char* destination)
{
    int trash;
    char buffer[30];
    trash = fscanf(filePointer, "%s", buffer);
    buffer[strlen(buffer) - 2] = '\0';
    strcpy(destination, buffer);
}


void printAllGradesByDiscipline(Student* studentArray)
{
    char temp[30];
    bool found = false;
    enterDisciplineName("Enter a discipline to search for: ", temp);
    printf("\n");

    for (Student* tempStudent = studentArray; tempStudent != NULL; tempStudent = tempStudent->nextStudent)
    {
        for (Grade* tempGrade = tempStudent->gradeArray; tempGrade != NULL; tempGrade = tempGrade->nextGrade)
        {
            if (strcmp(temp, tempGrade->disciplineName) == 0)
            {
                found = true;
                printf("\tName: %s %s\n", tempStudent->firstName, tempStudent->lastName);
                printf("\tFaculty Number: %s\n", tempStudent->facultyNumber);
                printf("\tGrade: %.2f\n\n", tempGrade->gradeValue);
                break;
            }
        }
    }

    if (!found)
    {
        printf("\tInvalid discipline!\n");
    }

    return;
}


float printGroupGPA(Student* studentArray, bool print)
{
    int i = 0;
    float result;
    Student* temp;
    float total = 0;

    for (temp = studentArray; temp != NULL; temp = temp->nextStudent)
    {
        if (temp->averageGrade && !isnan(temp->averageGrade))
        {
            total += temp->averageGrade;
            i++;
        }
    }

    if (i == 0)
    {
        if (print)
        {
            printf("\nGroup has no grades yet!\n\n");
        }
        return 0;
    }

    result = total / i;

    if (print)
    {
        printf("\nGroup GPA: %.2f\n\n", result);
    }

    return result;
}


void deleteGrade(Student* studentArray)
{
    Student* currStudent = findStudent(studentArray);
    if (!currStudent)
    {
        printf("Student does not exist!\n");
        return;
    }

    Grade* gradeToRemove = findGrade(currStudent);

    if (!gradeToRemove)
    {
        printf("Student does not study this discipline!\n");
        return;
    }

    if (gradeToRemove == currStudent->gradeArray)
    {
        gradeDeletionSwitch(&(currStudent->gradeArray), true);
        currStudent->gradeCount--;
        findAvgGrade(currStudent);
        return;
    }

    Grade* currentGrade;
    for (currentGrade = currStudent->gradeArray; currentGrade->nextGrade != gradeToRemove;
        currentGrade = currentGrade->nextGrade);

    gradeDeletionSwitch(&(currentGrade->nextGrade), true);
    currStudent->gradeCount--;
    findAvgGrade(currStudent);
    return;
}


void studentDeletionSwitch(Student** currentStudent)
{
    Student* temp = (*currentStudent);
    (*currentStudent) = (*currentStudent)->nextStudent;

    deleteAllGrades(&(temp->gradeArray));

    printf("Student \"%s %s\" removed.\n", temp->firstName, temp->lastName);
    free(temp);

    return;
}


void deleteAllGrades(Grade** gradeArray)
{
    Grade* temp = (*gradeArray);

    if (!temp)
    {
        return;
    }

    while (temp)
    {
        temp = temp->nextGrade;
        free((*gradeArray));
        (*gradeArray) = temp;
    }

    return;
}


void gradeDeletionSwitch(Grade** currentGrade, bool print)
{
    Grade* temp = (*currentGrade);
    (*currentGrade) = (*currentGrade)->nextGrade;

    if (print)
    {
        printf("Grade for discipline \"%s\" removed.\n", temp->disciplineName);
    }

    free(temp);

    return;
}


void deleteStudent(Student** studentArray)
{
    Student* foundStudent = findStudent(*studentArray);

    if (!foundStudent)
    {
        printf("Student does not exist!\n");
        return;
    }

    if (foundStudent == (*studentArray))
    {
        studentDeletionSwitch(studentArray);
        studentCount--;
        return;
    }

    Student* currentStudent;
    for (currentStudent = (*studentArray); currentStudent->nextStudent != foundStudent;
        currentStudent = currentStudent->nextStudent);

    studentDeletionSwitch(&(currentStudent->nextStudent));
    studentCount--;

    return;
}


void createGrade(Student* studentArray)
{
    Student* currStudent = findStudent(studentArray);

    if (!currStudent)
    {
        printf("Student does not exist!\n");
        return;
    }


    if (!(currStudent->gradeArray))
    {
        addGrade(&(currStudent->gradeArray), currStudent);
        return;
    }

    Grade* i;
    for (i = currStudent->gradeArray; i->nextGrade != NULL; i = i->nextGrade);

    addGrade(&(i->nextGrade), currStudent);

    return;
}


void addGrade(Grade** grade, Student* currStudent)
{
    char temp[30];
    float gradeValue;

    (*grade) = (Grade*)malloc(sizeof(Grade));
    if (!(*grade))
    {
        printf("createGrade could not allocate memory!\n");
        exit(1);
    }

    (*grade)->nextGrade = NULL;

    do
    {
        enterDisciplineName("Enter a discpline name: ", temp);
    } while (!isValidDisciplineName(currStudent, temp));

    enterValidGrade(&gradeValue);
    strcpy((*grade)->disciplineName, temp);
    (*grade)->gradeValue = gradeValue;
    currStudent->gradeCount++;
    findAvgGrade(currStudent);

    printf("Grade for discipline \"%s\" added.\n\n", (*grade)->disciplineName);
    return;
}


bool isValidDisciplineName(Student* student, char* disciplineName)
{

    if (strlen(disciplineName) == 1 && disciplineName[0] == ' ')
    {
        return false;
    }

    for (int i = 0; i < strlen(disciplineName); i++)
    {
        if (!isalpha(disciplineName[i]) && !isdigit(disciplineName[i]) && disciplineName[i] != ' ')
        {
            return false;
        }
    }

    for (Grade* temp = student->gradeArray; temp != NULL; temp = temp->nextGrade)
    {
        if (strcmp(temp->disciplineName, disciplineName) == 0)
        {
            return false;
        }
    }

    return true;
}


void printErrorMessage(void)
{
    printf("\nInvalid choice! Please try again!\n");
    return;
}


void findAvgGrade(Student* student)
{
    float total = 0;

    if (!(student->gradeArray))
    {
        student->averageGrade = 0;
        return;
    }
    for (Grade* i = student->gradeArray; i != NULL; i = i->nextGrade)
    {
        total += i->gradeValue;
    }

    student->averageGrade = total / student->gradeCount;

    return;
}


Grade* findGrade(Student* student)
{
    char temp[30];

    enterDisciplineName("Enter discipline to remove: ", temp);

    for (Grade* i = student->gradeArray; i != NULL; i = i->nextGrade)
    {
        if (strcmp(temp, i->disciplineName) == 0)
        {
            return i;
        }
    }

    return NULL;
}


void editGrade(Student* studentArray)
{
    char temp[30];
    Student* currStudent = findStudent(studentArray);

    if (!currStudent)
    {
        printf("Student does not exist!\n");
        return;
    }

    enterDisciplineName("Enter discipline name: ", temp);
    changeGrade(currStudent, temp);

    return;
}


void changeGrade(Student* currStudent, char* temp)
{
    float newGrade;

    for (Grade* i = currStudent->gradeArray; i != NULL; i = i->nextGrade)
    {
        if (strcmp(i->disciplineName, temp) == 0)
        {
            enterValidGrade(&newGrade);
            i->gradeValue = newGrade;
            findAvgGrade(currStudent);
            printf("\n\tGrade edited.\n");
            return;
        }
    }

    printf("Student does not study \"%s\".\n", temp);

    return;
}


void enterValidGrade(float* grade)
{
    char temp[50];

    do
    {
        printf("Enter valid grade [2-6]: ");
        fflush(stdin);
        fgets(temp, 50, stdin);
        temp[strlen(temp) - 1] = '\0';
        fflush(stdin);
        (*grade) = (float)atof(temp);
    } while (!isValidGrade(*grade));

    return;
}


bool isValidGrade(float grade)
{
    if (grade < 2 || grade > 6)
    {
        return false;
    }

    return true;
}


void enterDisciplineName(const char* message, char* temp)
{
    printf("%s", message);
    fflush(stdin);
    fgets(temp, 30, stdin);
    temp[strlen(temp) - 1] = '\0';
    fflush(stdin);
    return;
}


void quit(void)
{
    printf("Exiting...\n");
    exit(0);
}


void createStudent(Student** studentArray)
{

    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("createStudent failed!");
        exit(1);
    }

    enterFacultyNumber(newStudent, (*studentArray));
    enterNames(newStudent);

    newStudent->gradeCount = 0;
    newStudent->averageGrade = 0;
    newStudent->gradeArray = NULL;
    newStudent->nextStudent = NULL;

    if (!(*studentArray))
    {
        (*studentArray) = newStudent;
        studentCount++;
        return;
    }

    Student* i;
    for (i = (*studentArray); i->nextStudent != NULL; i = i->nextStudent);

    i->nextStudent = newStudent;
    studentCount++;

    return;
}


Student* findStudent(Student* studentArray)
{
    char temp[50];
    printf("Enter a faculty number: ");
    fflush(stdin);
    fgets(temp, 50, stdin);
    temp[strlen(temp) - 1] = '\0';
    fflush(stdin);

    for (Student* i = studentArray; i != NULL; i = i->nextStudent)
    {
        if (strcmp(temp, i->facultyNumber) == 0)
        {
            return i;
        }
    }
    return NULL;
}


void printStudent(Student* studentArray)
{
    Student* foundStudent = findStudent(studentArray);

    if (foundStudent)
    {
        printf("Student: \n");
        printf("\tFaculty Number: %s\n", foundStudent->facultyNumber);
        printf("\tName: %s %s %s\n", foundStudent->firstName, foundStudent->middleName, foundStudent->lastName);

        if (foundStudent->gradeCount > 0)
        {
            printf("\tAverage grade: %.2f\n", foundStudent->averageGrade);
            printf("\tGrades:\n");

            for (Grade* i = foundStudent->gradeArray; i != NULL; i = i->nextGrade)
            {
                printf("\t\t%s - %.2f\n", i->disciplineName, i->gradeValue);
            }

            return;
        }

        printf("\tStudent has no grades!\n");
        return;
    };

    printf("Student does not exist!\n");
    return;
}


void printStudents(Student* studentArray)
{
    if (!studentArray)
    {
        printf("Database is empty!\n");
        return;
    }

    //sortStudentsByGPA(&studentArray);


    printf("\n\tAll students: \n\n");
    for (Student* i = studentArray; i != NULL; i = i->nextStudent)
    {
        printf("\tFaculty Number: %s\n", i->facultyNumber);
        printf("\tName: %s %s %s\n", i->firstName, i->middleName, i->lastName);
        if (i->gradeCount > 0)
        {
            printf("\tAverage grade: %.2f\n\n", i->averageGrade);
        }
        else
        {
            printf("\tStudent has no grades!\n\n");
        }
    }
    printf("\n");
    return;
}


void nameInput(const char* nameType, char* temp)
{
    printf("Enter student's %s name: ", nameType);
    fflush(stdin);
    fgets(temp, 50, stdin);
    temp[strlen(temp) - 1] = '\0';
    fflush(stdin);


    while (!isValidName(temp)) {
        printf("Invalid %s name! Try again: ", nameType);
        fflush(stdin);
        fgets(temp, 50, stdin);
        temp[strlen(temp) - 1] = '\0';
        fflush(stdin);
    }

    return;
}


void enterNames(Student* newStudent)
{
    char temp[50];

    nameInput("first", temp);
    strcpy(newStudent->firstName, temp);

    nameInput("middle", temp);
    strcpy(newStudent->middleName, temp);

    nameInput("last", temp);
    strcpy(newStudent->lastName, temp);

    return;
}


void enterFacultyNumber(Student* newStudent, Student* studentArray)
{
    char facultyNumber[50];

    do
    {
        printf("Enter a student faculty number: ");
        fflush(stdin);
        fgets(facultyNumber, 50, stdin);
        facultyNumber[strlen(facultyNumber) - 1] = '\0';
        fflush(stdin);
    } while (!isValidFacultyNumber(facultyNumber, studentArray));

    strcpy(newStudent->facultyNumber, facultyNumber);
    return;
}


bool isValidFacultyNumber(char* string, Student* studentArray)
{
    if (strlen(string) != 6)
    {
        return false;
    }

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (!isdigit(string[i]))
        {
            return false;
        }
    }

    for (Student* temp = studentArray; temp != NULL; temp = temp->nextStudent)
    {
        if (strcmp(temp->facultyNumber, string) == 0)
        {
            return false;
        }
    }

    return true;
}


bool isValidName(char* string)
{
    if (strlen(string) == 0)
    {
        return false;
    }

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (!isalpha(string[i]))
        {
            return false;
        }
    }

    return true;
}