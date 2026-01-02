#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#define MAX_TERMS 1024


typedef struct {
	int row, column, value; 
}Term;


void readMatrix(FILE* fp, Term x[], int*R, int*C) {
    int temp;
    fscanf(fp, "%d %d", R, C); 
    int count = 0;
    for (int i = 0; i < *R; i++) {
        for (int j = 0; j < *C; j++) {
            fscanf(fp, "%d", &temp);
            x[count].row = i;
            x[count].column = j;
            x[count].value = temp;
            count++;
        }
    }
}

void printMatrix(Term c[], int valid, int R, int C) {
    int Total_Term = R * C;
    int count = 0;
    printf("%d %d %d\n", R, C, valid);
    while (count < Total_Term) {
        if (c[count].value != 0) {
            printf("%d %d %d\n", c[count].row, c[count].column, c[count].value);
        }
        count++;
    }
    printf("\n");
}

void matrixAdd(Term a[], Term b[], Term c[], int*valid, int R, int C) {
    int count = 0;
    *valid = 0;
    int Total_Term = R * C;
    while (count < Total_Term) {
        if (a[count].value != 0 || b[count].value != 0) {
            c[count].row = a[count].row;
            c[count].column = a[count].column;
            c[count].value = a[count].value + b[count].value;
            (*valid)++;
        }
        else {
            c[count].value = 0;
        }
        count++;
    }
}

void matrixSubtract(Term a[], Term b[], Term c[], int*valid, int R, int C) {
    int count = 0;
    *valid = 0;
    int Total_Term = R * C;
    while (count < Total_Term) {
        if (a[count].value != 0 || b[count].value != 0) {
            c[count].row = a[count].row;
            c[count].column = a[count].column;
            c[count].value = a[count].value - b[count].value;
            (*valid)++;
        }
        else {
            c[count].value = 0;
        }
        count++;
    }
}

int main() {
	Term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];

    clock_t start, end;
    double duration;
    int R1, C1, R2, C2;
    int valid;

	FILE* fp = fopen("matrix1.txt", "r");
    if (fp == NULL) {
        printf("NO FILE OPEN!\n");
        return 1;
    }
	readMatrix(fp, a, &R1, &C1);
	fclose(fp);
	
    fp = fopen("matrix2.txt", "r");
    if (fp == NULL) {
        printf("NO FILE OPEN!\n");
        return 1;
    }
    readMatrix(fp, b, &R2, &C2);
    fclose(fp);

    if (R1 != R2 || C1 != C2) {
        printf("The sizes of the two matrices do not match.\n");
        return 1;
    }

    printf("matrix1 + matrix2\n");
    start = clock();
    for (int i = 0; i < 1000; i++) {
        matrixAdd(a, b, c, &valid, R1,C1);
    }
    duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
    printMatrix(c, valid, R1, C1);
    printf("평균 연산 시간(1000회 반복): %.6f\n\n",duration/1000);


    printf("matrix1 - matrix2\n");
    start = clock();
    for (int i = 0; i < 1000; i++) {
        matrixSubtract(a, b, c, &valid, R1, C1);
    }
    duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
    printMatrix(c, valid, R1,C1);
    printf("평균 연산 시간(1000회 반복): %.6f\n",duration/1000);

}
