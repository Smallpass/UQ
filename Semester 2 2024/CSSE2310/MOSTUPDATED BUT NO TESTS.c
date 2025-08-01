#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char* getattempt();
int validattempt(char*);
float entropy1calc(char*);
void entropyprogram1();
void entropyclass(float);

void entropyprogram2(int, char**);
int entropy2calc(int, int, char**, char**, char*);
int argumentcheck(int, char**, char*);
int passwordfilepresent(int, char**);
int validarguments(int, char**);
char** populatefiles(int, char**, int);
char** passwordlist(int, char**, int);
int validatefiles(int, char**, int);
int count_entries(char**);
int strcasecmp_custom(char*, char*);
int lettercount(char*);
float min(float, float);
char* add_digits(char*, int, int);

int main(int argc, char** argv)
{
    if (argc == 1) {
        entropyprogram1();
    } else {
        if (validarguments(argc, argv) == 1) {
            entropyprogram2(argc, argv);
        } else {
            fprintf(stderr,
                    "Usage: ./uqentropy [--casecheck] [--leet] [--double] "
                    "[--add-digits 1..8] [listfilename ...]\n");
            exit(3);
        }
    }
}

void entropyprogram1()
{
    printf("Welcome to UQEntropy\nWritten by s4803050.\n");
    printf("Enter password candidate to check its strength.\n");
    int strongdetect = 0;
    while (1) {
        char* attempt = getattempt();
        if (attempt == NULL) {
            break;
        }
        if (validattempt(attempt) == 0) {
            fprintf(stderr, "Candidate password is invalid\n");
        } else {
            printf("Password entropy: %.1f\n",
                    floor(entropy1calc(attempt) * 10) / 10);

            entropyclass(entropy1calc(attempt));

            if (entropy1calc(attempt) >= 60) {
                strongdetect = 1;
            }
            free(attempt);
        }
    }

    if (strongdetect == 0) {
        printf("No strong password(s) have been identified\n");
        exit(20);
    }
    exit(0);
}

char* getattempt()
{
    int length = 0;
    char c;
    char* attempt = (char*)malloc(sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF) {
        length++;
        char* temp = (char*)realloc(attempt, length + 1);
        if (temp == NULL) {
            printf("Memory reallocation failed\n");
            free(attempt);
            return NULL;
        }
        attempt = temp;
        attempt[length - 1] = c;
        attempt[length] = '\0';
    }

    if (c == EOF && length == 0) {
        free(attempt);
        return NULL;
    }

    return attempt;
}

int validattempt(char* attempt)
{
    if (strlen(attempt) > 0) {
        for (int i = 0; i < (int)strlen(attempt); i++) {
            if ((isprint(attempt[i]) == 0) || (attempt[i] == ' ')) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

float entropy1calc(char* attempt)
{
    float integers = 0;
    float lowercase = 0;
    float uppercase = 0;
    float otherchar = 0;

    for (int i = 0; i < (int)strlen(attempt); i++) {
        int c = attempt[i];

        if ((c >= 48) && (c <= 57)) {
            integers = 10;
        }

        if ((c >= 97) && (c <= 122)) {
            lowercase = 26;
        }

        if ((c >= 65) && (c <= 90)) {
            uppercase = 26;
        }
        if (isprint(c)
                && !((c >= 48 && c <= 57) || (c >= 97 && c <= 122)
                        || (c >= 65 && c <= 90))) {
            otherchar = 32;
        }
    }
    return strlen(attempt)
            * log2f(integers + lowercase + uppercase + otherchar);
}

void entropyclass(float entropy)
{
    if (entropy < 35) {
        printf("Password strength classification: very weak\n");
    } else if (30 <= entropy && entropy < 60) {
        printf("Password strength classification: weak\n");
    } else if (60 <= entropy && entropy < 120) {
        printf("Password strength classification: strong\n");
    } else if (entropy >= 120) {
        printf("Password strength classification: very strong\n");
    }
}

int firstfilearg(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        if ((argv[i][0] != '-') && (argv[i][0] != NULL)) {
            return i;
        }
    }
    return -1;
}

int validarguments(int argc, char** argv)
{
    int casecheck = 0;
    int leet = 0;
    int digits = 0;
    int sdouble = 0;

    int firstfile = firstfilearg(argc, argv);
    for (int i = 1; i < firstfile; i++) {
        char* arg = argv[i];

        if (strcmp(arg, "--casecheck") == 0) {
            casecheck++;
        } else if (strcmp(arg, "--leet") == 0) {
            leet++;
        } else if (strcmp(arg, "--add-digits 1") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 2") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 3") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 4") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 5") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 6") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 7") == 0) {
            digits++;
        } else if (strcmp(arg, "--add-digits 8") == 0) {
            digits++;
        } else if (strcmp(arg, "--double") == 0) {
            sdouble++;
        } else {
            return 0;
        }
    }

    if ((casecheck > 1) || (leet > 1) || (digits > 1) || (sdouble > 1)) {
        return 0;
    }
    return 1;
}

int argumentcheck(int argc, char** argv, char* argument)
{
    if (argument == "--add-digits") {
        for (int i = 0; i < argc; i++) {
            for (int j = 1; j < 9; j++) {
                if (strcmp(add_digits(argument, j, 1), argv[i]) == 0) {
                    return j;
                }
            }

            if (strcmp(argument, argv[i]) == 0) {
                return -1;
            }
        }
    }
    return 0;
}

char* add_digits(char* string, int number, int num_spaces)
{
    // Convert the integer to a string
    char num_str[12]; // Enough space to store the number and '\0'
    snprintf(num_str, sizeof(num_str), "%d", number);

    // Calculate the new string length
    int original_length = strlen(string);
    int num_str_length = strlen(num_str);
    int total_length = original_length + num_spaces + num_str_length;

    // Allocate memory for the new string
    char* result
            = (char*)malloc(total_length + 1); // +1 for the null terminator
    if (result == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Copy the original string to the result
    strcpy(result, string);

    // Add the spaces
    for (int i = 0; i < num_spaces; i++) {
        result[original_length + i] = ' ';
    }

    // Append the number string
    strcpy(result + original_length + num_spaces, num_str);

    return result;
}

void entropyprogram2(int argc, char** argv)
{
    int firstfile = firstfilearg(argc, argv);
    if (firstfile == -1) {
        fprintf(stderr,
                "Usage: ./uqentropy [--casecheck] [--leet] [--double] "
                "[--add-digits 1..8] [listfilename ...]\n");
        exit(3);
    }
    char** passwords = passwordlist(argc, argv, firstfile);
    printf("Welcome to UQEntropy\nWritten by s4803050.\n");
    printf("Enter password candidate to check its strength.\n");
    int strongdetect = 0;
    while (1) {
        char* attempt = getattempt();
        if (attempt == NULL) {
            break;
        }
        if (validattempt(attempt) == 0) {
            fprintf(stderr, "Candidate password is invalid\n");
        } else {
            int matchcheck = entropy2calc(0, argc, argv, passwords, attempt);
            if (matchcheck == 1) {
                printf("Password found in list\n");
            } else {
                printf("Password entropy: %.1f\n",
                        floor(entropy1calc(attempt) * 10) / 10);

                entropyclass(entropy1calc(attempt));

                if (entropy1calc(attempt) >= 60) {
                    strongdetect = 1;
                }
            }
            free(attempt);
        }
    }

    if (strongdetect == 0) {
        printf("No strong password(s) have been identified\n");
        exit(20);
    }
    exit(0);
}

int entropy2calc(int count, int argc, char** argv, char** passwords, char* attempt)
{
    if (count >= argc - 1) {
        return 0;
    }

    int current = count + 1;
    if (strcasecmp_custom(attempt, passwords[current]) == 0) {
        return 1;
    }
    return entropy2calc(current, argc, argv, passwords, attempt);
}

int strcasecmp_custom(char* s1, char* s2)
{
    while (*s1 && *s2 && tolower(*s1) == tolower(*s2)) {
        s1++;
        s2++;
    }
    return tolower(*(unsigned char*)s1) - tolower(*(unsigned char*)s2);
}

int passwordfilepresent(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        if ((argv[i][0] != '-') && (argv[i][0] != NULL)) {
            return 1;
        }
    }
    return 0;
}

char** passwordlist(int argc, char** argv, int firstfile)
{
    if (passwordfilepresent(argc, argv) == 1) {
        char** passwords = (char**)malloc(10000 * sizeof(char*));
        if (passwords == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        int num_passwords = 0;

        for (int i = firstfile; i < argc; i++) {
            FILE* file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "Unable to open file %s\n", argv[i]);
                exit(1);
            }

            char line[256];
            while (fgets(line, sizeof(line), file)) {
                // Remove newline character if present
                line[strcspn(line, "\n")] = '\0';
                passwords[num_passwords] = strdup(line);
                if (passwords[num_passwords] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                num_passwords++;
            }
            fclose(file);
        }
        return passwords;
    }
    return NULL;
}

int validatefiles(int argc, char** argv, int firstfile)
{
    for (int i = firstfile; i < argc; i++) {
        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            fprintf(stderr, "Unable to open file %s\n", argv[i]);
            return 0;
        }
        fclose(file);
    }
    return 1;
}

int count_entries(char** list)
{
    int count = 0;
    while (list[count] != NULL) {
        count++;
    }
    return count;
}
