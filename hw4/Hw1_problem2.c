#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#define MAX_TERMS 1024
#define COMPARE(x, y) ((x) < (y) ? -1 : (x) == (y) ? 0 : 1)

typedef struct {
    int row, column, value; 
}Term;



void readMatrix(FILE* fp, Term x[], int* R, int* C, int *valid) {
    int temp;
    fscanf(fp, "%d %d", R, C);
    int count = 1;
    for (int i = 0; i < *R; i++) {
        for (int j = 0; j < *C; j++) {
            fscanf(fp, "%d", &temp);
            if (temp) {
                x[count].row = i;
                x[count].column = j;
                x[count].value = temp;
                count++;
            }
        }
    }
    *valid = count-1;
    x[0].row = *R;
    x[0].column = *C;
    x[0].value = *valid;
}

void fast_transpose(Term a[], Term b[], int R, int C, int valid) {

    int row_terms[MAX_TERMS];
    int num_cols = a[0].column;
    int num_terms = a[0].value;

    b[0].row = num_cols; 
    b[0].column = a[0].row;
    b[0].value = num_terms;

    if (num_terms > 0) {
        for (int i = 0; i < num_cols; i++) {
            row_terms[i] = 0;
        }

        for (int i = 1; i <= num_terms; i++) {
            row_terms[a[i].column]++;
        }

        int temp = row_terms[0];
        row_terms[0] = 1;
        for (int i = 1; i < num_cols; i++) {
            int temp2 = row_terms[i];
            row_terms[i] = row_terms[i - 1] + temp;
            temp = temp2;
        }

        for (int i = 1; i <= num_terms; i++) {
            int j = row_terms[a[i].column]++;
            b[j].row = a[i].column;
            b[j].column = a[i].row;
            b[j].value = a[i].value;
        }

    }
}

void storesum(Term c[], int* totalc, int row, int column, int* sum) {
    if (*sum != 0) { 
        c[*totalc+1].row = row;
        c[*totalc+1].column = column;
        c[*totalc+1].value = *sum;  
        (*totalc)++;
        *sum = 0;  
    }
}


void mmult(Term a[], Term b[], Term c[]) {
    Term new_b[MAX_TERMS];
    int totala = a[0].value;
    int totalb = b[0].value;

    int rows_a = a[0].row;
    int cols_a = a[0].column;

    int cols_b = b[0].column;

    int row_begin = 1;
    int totalc = 0;
    int row = a[1].row;
    int sum=0;

    if (cols_a != b[0].row) {
        printf("Incompatible matrices\n");
        return;
    }

    fast_transpose(b, new_b, b[0].row, cols_b, totalb);
  
    a[totala + 1].row=rows_a;
    
    new_b[totalb + 1].row = cols_b;
    new_b[totalb + 1].column = -1;

    for (int i = 1; i <= totala;) {
        int column = new_b[1].row; 
        for (int j = 1; j <= totalb + 1;) { 
            if (a[i].row != row) { 
                storesum(c, &totalc, row, column, &sum);
                i = row_begin;
                for (; new_b[j].row == column; j++);
                column = new_b[j].row;
            }
            else if (new_b[j].row != column) {
                storesum(c, &totalc, row, column, &sum);
                i = row_begin;
                column = new_b[j].row;
            }
            else {
                switch (COMPARE(a[i].column, new_b[j].column)) {  
                case -1:
                    i++; 
                    break;
                case 0:
                    sum += (a[i++].value * new_b[j++].value);
                    break;
                case 1: 
                    j++; 
                    break;
                }

            }
                
        }
        for (; a[i].row == row; i++);
        row_begin = i; 
        row = a[i].row;
    }
    c[0].row = rows_a; 
    c[0].column = cols_b;
    c[0].value = totalc;
}

void printMatrix(Term c[], int valid, int R, int C) {
    for (int i = 0; i <= valid; i++) {
        printf("%d %d %d\n", c[i].row, c[i].column, c[i].value);
    }
    printf("\n");
}

int main() {
    Term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];
    Term Transpose_b[MAX_TERMS];

    clock_t start, end;
    double duration;
    int R1, C1, R2, C2;
    int valid1, valid2;

    FILE* fp = fopen("matrix3.txt", "r");
    if (fp == NULL) {
        printf("NO FILE OPEN!\n");
        return 1;
    }
    readMatrix(fp, a, &R1, &C1, &valid1);
    fclose(fp);
    fp = fopen("matrix4.txt", "r");
    if (fp == NULL) {
        printf("NO FILE OPEN!\n");
        return 1;
    }
    printf("matrix3 * matrix4\n");
    readMatrix(fp, b, &R2, &C2,&valid2);
    fclose(fp);
    start = clock();
    for (int i = 0; i < 1000; i++) {
        mmult(a, b, c);
    }
    duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
    printMatrix(c,c[0].value, R1, C2);
    printf("평균 연산 시간(1000회 반복): %.6f\n\n", duration / 1000);
}
