#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50

typedef enum { head, entry } tagfield;
typedef struct matrix_node* matrix_pointer;
typedef struct entry_node {
    int row;
    int col;
    int value;
} entry_node;

typedef struct matrix_node {
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    union {
        matrix_pointer next;
        struct entry_node entry;
    } u;
} matrix_node;


matrix_pointer hdnode[MAX_SIZE];


matrix_pointer mread(FILE*);
void mwrite(matrix_pointer);
void merase(matrix_pointer*);
matrix_pointer madd(matrix_pointer, matrix_pointer);

void saveforfile(int** matrix, int rows, int cols, FILE* fp) {
    fprintf(fp, "%d %d\n", rows, cols);

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            fprintf(fp, "%d ", matrix[x][y]);
        }
        fprintf(fp, "\n");
    }
}

int main() {

    matrix_pointer a, b, d;
    int rowsA = 0;
    int colsA = 0;

    scanf("%d %d", &rowsA, &colsA);

    FILE* fpA = fopen("A.txt", "w");
    if (!fpA) {
        fprintf(stderr, "A.txt: No Open file\n");
        return 1;
    }

    int** matrixA = (int**)malloc(rowsA * sizeof(int*));

    for (int x = 0; x < rowsA; x++) {
        matrixA[x] = (int*)malloc(colsA * sizeof(int));
    }

    for (int x = 0; x < rowsA; x++) {
        for (int y = 0; y < colsA; y++) {
            scanf("%d", &matrixA[x][y]);
        }
    }

    saveforfile(matrixA, rowsA, colsA, fpA);

    for (int x = 0; x < rowsA; x++) {
        free(matrixA[x]);
    }

    free(matrixA);

    fclose(fpA);


    int rowsB = 0, colsB = 0;
    scanf("%d %d", &rowsB, &colsB);

    FILE* fpB = fopen("B.txt", "w");
    if (!fpB) {
        fprintf(stderr, "B.txt: No Open file\n");
        return 1;
    }

    int** matrixB = (int**)malloc(rowsB * sizeof(int*));

    for (int x = 0; x < rowsB; x++) {
        matrixB[x] = (int*)malloc(colsB * sizeof(int));
    }

    for (int x = 0; x < rowsB; x++) {
        for (int y = 0; y < colsB; y++) {
            scanf("%d", &matrixB[x][y]);
        }
    }

    saveforfile(matrixB, rowsB, colsB, fpB);

    for (int i = 0; i < rowsB; i++) {
        free(matrixB[i]);
    }
    free(matrixB);

    fclose(fpB);

    fpA = fopen("A.txt", "r");
    fpB = fopen("B.txt", "r");

    a = mread(fpA);
    b = mread(fpB);

    printf("\n");

    mwrite(a);
    mwrite(b);
    d = madd(a, b);
    mwrite(d);

    merase(&d);
    merase(&a);
    merase(&b);

    mwrite(a);
    mwrite(b);
    mwrite(d);
    fclose(fpA);
    fclose(fpB);

    return 0;
}

matrix_pointer new_node() {
    matrix_pointer node = (matrix_pointer)malloc(sizeof(matrix_node));
    if (!node) {
        fprintf(stderr, "No Available Memory\n");
        exit(0);
    }
    return node;
}

matrix_pointer mread(FILE* fp) {
    int numRows, numCols, numHeads;
    int value;
    matrix_pointer temp, last, node;

    fscanf(fp, "%d %d", &numRows, &numCols);

    numHeads = (numCols > numRows) ? numCols : numRows;

    node = new_node();
    node->u.entry.row = numRows;
    node->u.entry.col = numCols;

    if (!numHeads) node->right = node;
    else {
        for (int i = 0; i < numHeads; i++) {
            temp = new_node();
            hdnode[i] = temp; hdnode[i]->tag = head;
            hdnode[i]->right = temp; hdnode[i]->u.next = temp;
        }
    }

    last = hdnode[0];
    int current_row = 0;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            fscanf(fp, "%d", &value);
            if (!value) {
                continue;
            }
            if (current_row != i) {
                last->right = hdnode[current_row];
                current_row = i;
                last = hdnode[i];
            }
            temp = new_node();
            temp->tag = entry;
            temp->u.entry.row = i;
            temp->u.entry.col = j;
            temp->u.entry.value = value;
            last->right = temp;
            last = temp;
            hdnode[j]->u.next->down = temp;
            hdnode[j]->u.next = temp;
        }
    }

    last->right = hdnode[numRows - 1];

    for (int i = 0; i < numHeads; i++) {
        hdnode[i]->u.next->down = hdnode[i];
    }

    for (int i = 0; i + 1 < numHeads; i++) {
        hdnode[i]->u.next = hdnode[i + 1];
    }

    hdnode[numHeads - 1]->u.next = node;
    node->right = hdnode[0];

    return node;
}

void mwrite(matrix_pointer node) {
    if (node == NULL) {
        printf("Zero Matrix\n");
        return;
    }
    matrix_pointer temp, head = node->right;

    for (int i = 0; i < node->u.entry.row; i++) {
        head = head->right;
        for (int j = 0; j < node->u.entry.col; j++) {
            if (j == head->u.entry.col) {
                printf("%d ", head->u.entry.value);
                head = head->right;
            }
            else printf("%d ", 0);
        }
        head = head->u.next;
        printf("\n");
    }
    printf("\n");
}

void merase(matrix_pointer* node) {
    matrix_pointer x, y, head = (*node)->right;
    for (int i = 0; i < (*node)->u.entry.row; i++) {
        y = head->right;
        while (y != head) {
            x = y; y = y->right; free(x);
        }
        x = head; head = head->u.next; free(x);
    }
    y = head;
    while (y != *node) {
        x = y; y = y->u.next; free(x);
    }
    free(*node); *node = NULL;
}

matrix_pointer madd(matrix_pointer a, matrix_pointer b) {

    int N_Heads;
    int N_Cols = a->u.entry.col;
    int N_Rows = a->u.entry.row;

    if (a->u.entry.row != b->u.entry.row || a->u.entry.col != b->u.entry.col) {
        printf("Not Compatible Matrices Dimensions!!\n");
        return 0;
    }

    N_Heads = (N_Cols > N_Rows) ? N_Cols : N_Rows;

    matrix_pointer Node = new_node();

    Node->tag = entry;
    Node->u.entry.row = N_Rows;
    Node->u.entry.col = N_Cols;


    for (int x = 0; x < N_Heads; x++) {
        matrix_pointer tmp = new_node();
        hdnode[x] = tmp;
        hdnode[x]->tag = head;
        hdnode[x]->right = tmp;
        hdnode[x]->u.next = tmp;
    }

    matrix_pointer last;
    matrix_pointer rowB = b->right;
    matrix_pointer rowA = a->right;

    int index = 0;

    while (index < N_Rows) {
        last = hdnode[index];

        matrix_pointer tmpB = rowB->right;
        matrix_pointer tmpA = rowA->right;

        for (int j = 0; j < N_Cols; j++) {
            int A_Value = 0, B_Value = 0;


            while (tmpA != rowA && tmpA->u.entry.col < j) {
                tmpA = tmpA->right;
            }

            if (tmpA != rowA && tmpA->u.entry.col == j) {
                A_Value = tmpA->u.entry.value;
            }


            while (tmpB != rowB && tmpB->u.entry.col < j) {
                tmpB = tmpB->right;
            }

            if (tmpB != rowB && tmpB->u.entry.col == j) {
                B_Value = tmpB->u.entry.value;
            }


            matrix_pointer New_Node = new_node();
            New_Node->tag = entry;
            New_Node->u.entry.col = j;
            New_Node->u.entry.row = index;
            New_Node->u.entry.value = (A_Value != 0 || B_Value != 0) ? A_Value + B_Value : NULL;
            last->right = New_Node;
            last = New_Node;

        }

        last->right = hdnode[index];
        rowA = rowA->u.next;
        rowB = rowB->u.next;
        index++;
    }

    for (int x = 0; x < N_Cols; x++) {

        hdnode[x]->u.next->down = hdnode[x];

        hdnode[x]->u.next = (x < N_Cols - 1) ? hdnode[x + 1] : Node;
    }

    Node->right = hdnode[0];

    return Node;
}
