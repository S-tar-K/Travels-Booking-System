#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <windows.h> // system()
#include <conio.h>   // getch()

#define MAX_NAME 25
#define TABLE_SIZE 8

typedef struct person
{
    char name[MAX_NAME];
    char address[MAX_NAME];
    char ph[10];
    // char regDate[MAX_NAME];
    char seatNo[3];
    char details[MAX_NAME];

    struct person *next;
} Person;

Person *hashTable[TABLE_SIZE], *temp = NULL;
char name[MAX_NAME];
COORD coord = {0, 0};
char password[5] = {"skpdp"};

// Declarations
int hash(char *name);
void initTable();
Person *getDetails(char *name);
char *getTravelsName();
void insert(Person *p);
void displayPerson(Person *p);
void separateNames(char value[3][10], char *name);
void printTable();
Person *hashtableLookup(char *name);
int count(Person *p);
void availPersons();
int searchSeat(Person *tmp, int totSeat);
int availSeat(char *name);
void writeToFile(Person *p);
void insertFromFile(Person *p);
void assignValues(char value[5][MAX_NAME]);
void readFromFile();
void deleteFromFile(char *name);
void gotoxy(int x, int y);
void Password();
void displayBoard(char *name);

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Password() // for password option
{
    fflush(stdin);
    system("cls");
    char d[25] = "Password Protected";
    char ch, pass[5];
    int i = 0, j;
    gotoxy(90, 20);
    for (j = 0; j < 10; j++)
    {
        Sleep(50);
        printf("* ");
    }
    for (j = 0; j < 20; j++)
    {
        Sleep(50);
        printf("%c", d[j]);
    }
    for (j = 0; j < 10; j++)
    {
        Sleep(50);
        printf(" *");
    }
    gotoxy(90, 25);
    printf("Enter Password:");

    while (ch != 13)
    {
        ch = getch();

        if (ch != 13 && ch != 8)
        {
            putch('*');
            pass[i] = ch;
            i++;
        }
    }
    pass[i] = '\0';
    if (strcmp(pass, password) == 0)
    {

        gotoxy(90, 30);
        printf("\aPassword match\n");
        gotoxy(90, 35);
        printf("\aPress any key to countinue.....\n");
        getch();
        return;
    }
    else
    {
        system("cls");
        gotoxy(90, 30);
        printf("\aWarning!! Incorrect Password\n");
        getch();
        Password();
    }
}

void displayBoard(char *name)
{
    system("cls");
    int j;

    gotoxy(90, 30);
    for (j = 0; j < 8; j++)
    {
        Sleep(50);
        printf("* ");
    }
    for (j = 0; j < strlen(name); j++)
    {
        Sleep(50);
        printf("%c", name[j]);
    }
    for (j = 0; j < 8; j++)
    {
        Sleep(50);
        printf(" *");
    }
}

void writeToFile(Person *p)
{
    FILE *outfile;
    char write[100];

    // open file for writing
    outfile = fopen("travels.txt", "a");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        return;
    }

    strcpy(write, p->name);
    strcat(write, " ");
    strcat(write, p->address);
    strcat(write, " ");
    strcat(write, p->ph);
    strcat(write, " ");
    // strcat(write, p->regDate);
    // strcat(write, " ");
    strcat(write, p->seatNo);
    strcat(write, " ");
    strcat(write, p->details);
    strcat(write, " ");
    strcat(write, "\n");

    int results = fputs(write, outfile);
    if (results == EOF)
    {
        printf("\nFailed to write to file\n");
    }
    fclose(outfile);
}

void insertFromFile(Person *p)
{
    char seat[3];
    if (p == NULL)
        return;

    int index = hash(p->details);

    p->next = hashTable[index];
    hashTable[index] = p;
    return;
}

void insert(Person *p)
{
    char seat[3];
    if (p == NULL)
        return;

    int index = hash(p->details);

    if (p->details[0] == 'T')
        strcpy(seat, "15");
    else
        strcpy(seat, "5");

    if (hashTable[index] != NULL && !strcmp(hashTable[index]->seatNo, seat))
    {
        int avail = availSeat(hashTable[index]->details);

        if (avail == -1)
        {
            // printf("\nSorry no seat available\n");
            return;
        }
        sprintf(p->seatNo, "%d", avail);
    }
    else
        sprintf(p->seatNo, "%d", count(hashTable[index]) + 1);

    p->next = hashTable[index];
    hashTable[index] = p;
    return;
}

void assignValues(char value[5][MAX_NAME])
{
    temp = (Person *)(malloc(sizeof(Person)));
    strcpy(temp->name, value[0]);
    strcpy(temp->address, value[1]);
    strcpy(temp->ph, value[2]);
    // strcpy(temp->regDate, value[3]);
    strcpy(temp->seatNo, value[3]);
    strcpy(temp->details, value[4]);
    temp->next = NULL;

    insertFromFile(temp);
}

void readFromFile()
{
    FILE *file = fopen("travels.txt", "r");

    if (!file)
    {
        printf("\n Unable to open file\n");
        return;
    }
    if (feof(file))
    {
        printf("Data file is emtpy, exiting the program");
        return;
    }
    char str[100];

    while (fgets(str, sizeof(str), file))
    {
        char arr[5][MAX_NAME];
        int i = 0;
        char *token = strtok(str, " ");
        while (token != NULL)
        {
            strcpy(arr[i++], token);
            token = strtok(NULL, " ");
        }
        assignValues(arr);
    }
    fclose(file);
}

void deleteFromFile(char *name)
{
    FILE *file = fopen("travels.txt", "r");
    FILE *file2 = fopen("copy.txt", "w");

    if (!file)
    {
        printf("\n Unable to open file\n");
        return;
    }

    if (feof(file))
    {
        printf("Data file is emtpy, exiting the program");
        return;
    }
    char str[100];
    int results;

    while (fgets(str, sizeof(str), file))
    {
        int i = 0;
        char newStr[100];
        strcpy(newStr, str);
        char *token = strtok(str, " ");

        if (strcmp(name, token))
        {
            results = fputs(newStr, file2);
            if (results == EOF)
                printf("\nFailed to write to file\n");
        }
    }
    fclose(file);
    fclose(file2);

    file = fopen("travels.txt", "w");
    file2 = fopen("copy.txt", "r");

    while (fgets(str, sizeof(str), file2))
    {
        results = fputs(str, file);
        if (results == EOF)
            printf("\nFailed to write to file\n");
    }
    fclose(file);
    fclose(file2);

    initTable();
    readFromFile();
}

int hash(char *name)
{
    int len = strnlen(name, MAX_NAME);
    long int hashVal = 1;

    for (int i = 0; i < len; i++)
        hashVal += name[i];

    return hashVal % TABLE_SIZE;
}

void initTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
        hashTable[i] = NULL;
}

Person *getDetails(char *name)
{
    Person *tmp = (Person *)malloc(sizeof(Person));

    // time_t t;
    // time(&t);
    // strcpy(tmp->regDate,ctime(&t));

    printf("\nEnter client name (Minimum 8 characters): ");
    scanf("%s", tmp->name);
    fflush(stdin);
    printf("Enter client address: ");
    scanf("%s", tmp->address);
    printf("Enter client phone number: ");
    scanf("%s", tmp->ph);
    strncpy(tmp->details, name, MAX_NAME);

    tmp->next = NULL;
    return tmp;
}

char *getTravelsName()
{
    strcpy(name, "");
    int opt;
    printf("\n\nGive your preferences:\n\n\t1.Travels\n\t2.Sumo\nEnter your choice: ");
    scanf("%d", &opt);

    if (opt == 1)
        strcat(name, "TRS");
    else if (opt == 2)
        strcat(name, "SUM");
    else if (opt < 1 && opt > 2)
    {
        printf("\nVehicle not available here\n");
        return NULL;
    }

    printf("\n\t1.One day\n\t2.Two days\nEnter your choice: ");
    scanf("%d", &opt);

    if (opt == 1)
        strcat(name, "1");
    else if (opt == 2)
        strcat(name, "2");
    else if (opt < 1 && opt > 2)
    {
        printf("\nTrip not available here\n");
        return NULL;
    }

    printf("\n\t1.AC\n\t2.NON AC\nEnter your choice: ");
    scanf("%d", &opt);

    if (opt == 1)
        strcat(name, "AC");
    else if (opt == 2)
        strcat(name, "NAC");
    else if (opt < 1 && opt > 2)
    {
        printf("\nTravels not available here\n");
        return NULL;
    }

    return name;
}

void displayPerson(Person *p)
{
    printf("\n\tSeat number: %s", p->seatNo);
    printf("\n\tClient name: %s", p->name);
    printf("\n\tClient address: %s", p->address);
    printf("\n\tClient phone number: %s\n", p->ph);
    // printf("\n\tRegistered date: %s\n", p->regDate);
}

void separateNames(char value[3][10], char *name)
{
    if (name[0] == 'T')
        strcpy(value[0], "Travels");
    else
        strcpy(value[0], "Sumo");

    if (name[3] == '1')
        strcpy(value[1], "One Day");
    else
        strcpy(value[1], "Two Day");

    if (name[4] == 'A')
        strcpy(value[2], "AC");
    else
        strcpy(value[2], "NON AC");
}

void printTable()
{
    int j;
    char ch[3][10];

    gotoxy(8, 4);
    for (j = 0; j < 110; j++)
    {
        Sleep(5);
        printf("=");
    }
    printf("\n\n\tVehicle\t\tNo of days\tAC / Non AC\tSeat number\tClient name\tClient ph no\tClient Address\n\n");

    gotoxy(8, 8);
    for (j = 0; j < 110; j++)
    {
        Sleep(5);
        printf("=");
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (hashTable[i] == NULL)
            continue;
        separateNames(&(*ch), hashTable[i]->details);

        temp = hashTable[i];
        while (temp != NULL)
        {
            printf("\n\t%s\t\t%s\t\t%s\t\t", ch[0], ch[1], ch[2]);
            printf("%s\t\t%s\t%s\t%s\n", temp->seatNo, temp->name, temp->ph, temp->address);
            temp = temp->next;
        }
        printf("\t");
        for (j = 0; j < 110; j++)
        {
            Sleep(5);
            printf("-");
        }
    }
}

Person *hashtableLookup(char *name)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Person *tmp = hashTable[i];
        while (tmp != NULL)
        {
            if (!strcmp(tmp->name, name))
                return tmp;
            tmp = tmp->next;
        }
    }
    return NULL;
}

int count(Person *p)
{
    int cnt = 0;
    if (p == NULL)
        return 0;

    for (; p != NULL; p = p->next, cnt++)
        ;
    return cnt;
}

void availPersons()
{
    int opt;
    char name[MAX_NAME];
    strcpy(name, getTravelsName());

    if (name == NULL)
    {
        printf("\nNot available\n");
        return;
    }

    opt = hash(name);
    if (hashTable[opt] == NULL)
    {
        printf("\nNo persons in this vehicle\n");
        return;
    }
    else
    {
        Person *tmp = hashTable[opt];
        while (tmp != NULL)
        {
            displayPerson(tmp);
            tmp = tmp->next;
        }
    }
}

int searchSeat(Person *tmp, int totSeat)
{
    int num = atoi(tmp->seatNo);
    for (int avail = 0; avail < totSeat; avail++)
    {

        if (tmp != NULL && num == atoi(tmp->seatNo))
        {
            tmp = tmp->next;
            num--;
        }
        else
        {
            printf("\nSeat available at %d\n", num);
            return num;
        }
    }
    printf("\nSeat not available\n");
    return -1;
}

int availSeat(char *name)
{
    int index = hash(name);
    Person *tmp = hashTable[index];

    if (name[0] == 'T')
    {
        return searchSeat(tmp, 15);
    }
    else if (name[0] == 'S')
    {
        return searchSeat(tmp, 5);
    }
    else
    {
        printf("\nError occured!\n");
        return -1;
    }
}

int main()
{
    char op;
    initTable();
    readFromFile();
    getch();
    Password();
    system("cls");
    displayBoard("WELCOME TO BITS AND BYTES");
    getch();
    do
    {
        fflush(stdin);
        system("cls");
        gotoxy(10, 4);
        printf("* * * * * * * * DASHBOARD * * * * * * * *\n");
        printf("\nChoose an option : "
               "\n\t1.Book ticket"
               "\n\t2.Cancel ticket "
               "\n\t3.Persons in travels"
               "\n\t4.Booked in which travels"
               "\n\t5.Display all travels"
               "\n\t6.Exit");
        printf("\nEnter your choice: ");
        scanf("%c", &op);
        fflush(stdin);

        switch (op)
        {
        case '1':
        {
            system("cls");
            char name[MAX_NAME];
            strcpy(name, getTravelsName());
            int cnt = count(hashTable[hash(name)]);
            if ((name[0] == 'T' && cnt == 15) || (name[0] == 'S' && cnt == 5))
            {
                printf("\nThere is no seat available.\n");
                getch();
                break;
            }
            Person *t = getDetails(name);
            insert(t);
            writeToFile(t);
            break;
        }
        case '2':
        {
            system("cls");
            char val[3][10];
            char name[MAX_NAME];
            Person *tmp;
            printf("\nEnter booked name: ");
            scanf("%s", name);
            tmp = hashtableLookup(name);
            if (tmp == NULL)
            {
                printf("\nNo one named %s booked ticket\n", name);
                getch();
                break;
            }
            deleteFromFile(tmp->name);
            separateNames(&(*val), tmp->details);
            printf("\n\nTicket with below details cancelled:\n\n");
            printf("Ticket booked in %s %s %s", val[0], val[1], val[2]);
            displayPerson(tmp);
            getch();
            break;
        }
        case '3':
        {
            system("cls");
            availPersons();
            getch();
            break;
        }
        case '4':
        {
            system("cls");
            char val[3][10];
            char name[MAX_NAME];
            Person *tmp;
            printf("\nEnter booked name: ");
            scanf("%s", name);
            tmp = hashtableLookup(name);
            if (tmp == NULL)
            {
                printf("\nTicket not available\n");
                getch();
                break;
            }
            separateNames(&(*val), tmp->details);
            printf("\n\nTicket Found\n\n");
            printf("Ticket booked in %s %s %s", val[0], val[1], val[2]);
            displayPerson(tmp);
            getch();
            break;
        }
        case '5':
        {
            system("cls");
            printTable();
            getch();
            break;
        }
        case '6':
        {
            displayBoard("THANK YOU FOR VISITING BITS AND BYTES");
            break;
        }
        default:
            printf("\nChoose correct option\n");
            getch();
        }
        fflush(stdin);
    } while (op != '6');

    scanf("%c", &op);
    return 0;
}
