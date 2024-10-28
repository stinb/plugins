// Custom

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function prototypes
int factorial(int n);
void printArray(int *arr, int size);
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
char *strrev(char *str);
void matrixOperations(int matrix[10][10], int size);
void fillArrayWithSquares(int *arr, int size);
void tokenizeString(char *str);

void veryLongFunction() { // UndCC_Violation
    // Part 1: Variable declarations and initializations
    int a = 5, b = 10, c = 0;
    float x = 3.14, y = 2.71, z = 0.0;
    char str1[100] = "Hello";
    char str2[100] = "World";
    int arr[50];
    char buffer[200];
    int matrix[10][10];
    int i, j;

    // Part 2: Basic arithmetic operations
    c = a + b;
    c *= 2;
    c -= a;
    c /= b;
    z = x * y;
    z /= 2.0;
    z += x;

    // Part 3: Array initialization and processing
    fillArrayWithSquares(arr, 50);

    // Part 4: String operations
    strcat(str1, str2);
    strcpy(buffer, str1);

    // Part 5: Conditional statements and loops
    int max = (a > b) ? a : b;
    if (max > 10) {
        max = 10;
    } else {
        max = 5;
    }

    // Nested loops to fill and process a matrix
    matrixOperations(matrix, 10);

    // Part 6: Factorial function call
    int factResult = factorial(5);

    // Part 7: Pointer operations
    int *p = &a;
    *p = 20;
    int **pp = &p;
    **pp = 30;

    // Part 8: Structure definition and usage
    struct Point {
        int x;
        int y;
    };

    struct Point point;
    point.x = 10;
    point.y = 20;

    // Part 9: Dynamic memory allocation
    int *dynamicArray = (int *)malloc(10 * sizeof(int));
    if (dynamicArray == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    for (i = 0; i < 10; i++) {
        dynamicArray[i] = i + 1;
    }
    free(dynamicArray);

    // Part 10: More complex string manipulation
    char str3[] = "C programming";
    char str4[100];
    strcpy(str4, str3);
    strcat(str4, " is fun!");

    // Part 11: Mathematical functions
    double result = sqrt(16.0);
    result = pow(2.0, 8.0);
    result = sin(M_PI / 2);

    // Part 12: File operations
    FILE *file = fopen("example.txt", "w");
    if (file != NULL) {
        fprintf(file, "This is an example file.\n");
        fprintf(file, "The factorial of 5 is %d.\n", factResult);
        fclose(file);
    }

    // Part 13: Additional string operations
    tokenizeString(buffer);

    // Part 14: Array processing with pointers
    int *ptrArr = arr;
    for (i = 0; i < 50; i++) {
        *(ptrArr + i) += 1;
    }

    // Part 15: Function pointers
    int (*operation)(int, int);
    operation = add;
    int opResult = operation(10, 5);
    operation = subtract;
    opResult = operation(10, 5);
    operation = multiply;
    opResult = operation(10, 5);
    operation = divide;
    opResult = operation(10, 5);

    // Part 16: Bitwise operations
    int bitwiseAnd = a & b;
    int bitwiseOr = a | b;
    int bitwiseXor = a ^ b;
    int bitwiseNot = ~a;
    int bitwiseShiftLeft = a << 2;
    int bitwiseShiftRight = a >> 2;

    // Part 17: More complex nested loops
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                arr[i * j * k % 50] += i + j + k;
            }
        }
    }

    // Part 18: Further mathematical operations
    double logResult = log(10.0);
    double expResult = exp(1.0);
    double floorResult = floor(3.14);
    double ceilResult = ceil(2.71);

    // Part 19: More string operations
    char *reverseStr = strrev(str2);

    // Part 20: Additional prints and operations to extend the function
    printf("c = %d\n", c);
    printf("z = %f\n", z);
    printf("Max = %d\n", max);
    printf("Factorial Result = %d\n", factResult);
    printf("Point: (%d, %d)\n", point.x, point.y);
    printf("String 4: %s\n", str4);
    printf("Operation Result: %d\n", opResult);
    printf("Bitwise AND: %d\n", bitwiseAnd);
    printf("Bitwise OR: %d\n", bitwiseOr);
    printf("Bitwise XOR: %d\n", bitwiseXor);
    printf("Bitwise NOT: %d\n", bitwiseNot);
    printf("Bitwise Shift Left: %d\n", bitwiseShiftLeft);
    printf("Bitwise Shift Right: %d\n", bitwiseShiftRight);
    printf("Log Result: %f\n", logResult);
    printf("Exp Result: %f\n", expResult);
    printf("Floor Result: %f\n", floorResult);
    printf("Ceil Result: %f\n", ceilResult);
    printf("Reverse String: %s\n", reverseStr);

    // Extra sections to extend the function length

    // Part 21: Additional nested loops
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            matrix[i][j] += i * j;
        }
    }

    // Part 22: More array processing
    for (i = 0; i < 50; i++) {
        arr[i] = arr[i] * 2;
    }

    // Part 23: Additional string concatenation
    strcat(str1, " ");
    strcat(str1, str2);

    // Part 24: Additional pointer manipulations
    int *newPtr = (int *)malloc(20 * sizeof(int));
    if (newPtr != NULL) {
        for (i = 0; i < 20; i++) {
            newPtr[i] = i + 5;
        }
        free(newPtr);
    }

    // Part 25: More mathematical function calls
    result = cos(M_PI / 4);
    result = tan(M_PI / 4);

    // Final prints to reach the line count requirement
    for (i = 0; i < 10; i++) {
        printf("Matrix[%d][%d] = %d\n", i, i, matrix[i][i]);
    }
    for (i = 0; i < 10; i++) {
        printf("Array[%d] = %d\n", i, arr[i]);
    }

    // Part 26: Additional sections to extend the function length
    // Part 26.1: More nested loops
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                arr[(i + j + k) % 50] -= i + j + k;
            }
        }
    }

    // Part 26.2: Further mathematical operations
    double atanResult = atan(1.0);
    double atan2Result = atan2(1.0, 1.0);
    double hypotResult = hypot(3.0, 4.0);

    // Part 26.3: More string manipulations
    char str5[100];
    strncpy(str5, str3, 5);
    str5[5] = '\0';
    strncat(str5, str4, 10);

    // Part 26.4: More pointer operations
    int **matrixPtr = (int **)malloc(10 * sizeof(int *));
    if (matrixPtr != NULL) {
        for (i = 0; i < 10; i++) {
            matrixPtr[i] = (int *)malloc(10 * sizeof(int));
            if (matrixPtr[i] != NULL) {
                for (j = 0; j < 10; j++) {
                    matrixPtr[i][j] = i + j;
                }
            }
        }
        for (i = 0; i < 10; i++) {
            free(matrixPtr[i]);
        }
        free(matrixPtr);
    }

    // Final additional prints
    printf("atanResult: %f\n", atanResult);
    printf("atan2Result: %f\n", atan2Result);
    printf("hypotResult: %f\n", hypotResult);
    printf("String 5: %s\n", str5);
}

// Function definitions

int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("Division by zero error\n");
        return 0;
    }
    return a / b;
}

char *strrev(char *str) {
    if (!str || !*str) {
        return str;
    }
    int i = strlen(str) - 1, j = 0;
    char ch;
    while (i > j) {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

void matrixOperations(int matrix[10][10], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = i * j;
        }
    }
}

void fillArrayWithSquares(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i * i;
    }
}

void tokenizeString(char *str) {
    char *token;
    char delim[] = " ";
    token = strtok(str, delim);
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, delim);
    }
}
