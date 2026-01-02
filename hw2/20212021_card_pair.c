#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 21


typedef struct Node {
    // 연결리스트 노드 정의
    char data;           // data 명시
    struct Node* next;  // 다음 노드를 가리키는 pointer
}Node;

typedef struct {
    Node* top;
} Table;
// table 구조체 : 스택의 최상단 노드를 가리키는 포인터를 지정

int empty(Table* stack) {
    if (stack->top == NULL)
        return 1;
    else
        return 0;
}

void initialization(Table stack[]) {
    stack->top = NULL;  // stack의 top 멤버를 NULL로 초기화
}

void push(Table* stack, char index) {
    Node* New_NODE = (Node*)malloc(sizeof(Node)); // 새로운 노드를 동적으로 할당
    New_NODE->data = index; // New_NODE의 data 멤버에 매개변수로 받은 index 값을 할당
    New_NODE->next = stack->top; // 새로 생성된 노드의 next 멤버 변수를 현재 스택의 top을 가리키게 한다.
    stack->top = New_NODE; // 스택의 top을 새로 생성된 노드로 업데이트
}

char pop(Table* stack) {
    if (empty(stack)) {
        return '\0';
    }
    Node* temp = stack->top; // 스택의 top을 가리키는 포인터를 temp 변수에 저장
    char data = temp->data; // temp 포인터가 가리키는 노드의 data 멤버를 읽어와 data 변수에 저장
    stack->top = stack->top->next; // 스택의 top을 다음 노드를 가리키도록 업데이트
    free(temp);
    return data;
}

int match_pair(char left, char right) {
    if ((left == '(' && right == ')') || (left == '[' && right == ']'))
        return 1;  //짝이 맞으면 1을 output, 짝이 맞지 않으면 0을 output
    else
        return 0;
}

int CHECK(char card[]) {
    Table stack;
    initialization(&stack);
    int count = 0;
    while (card[count] != '\0') { //null-terminating character ('\0')가 나올때까지 반복
        if (card[count] == '(' || card[count] == '[') {
            push(&stack, card[count]);
            //// 현재 문자가 여는 괄호인 경우 스택에 현재 문자를 push
        }
        else if (card[count] == ')' || card[count] == ']') {
            if (empty(&stack)) {
                return 0;
            }
            //스택이 비어있는지 확인
            //만약 현재 스택이 비어있다면 짝이 맞지 않음을 의미하기 때문에 0으로 output
            char temp = pop(&stack);
            //스택에서 괄호의 짝이 되는 여는 괄호를 pop
            if (!match_pair(temp, card[count])) {
                return 0;
            }
        }
        count++;
    }
    if (!empty(&stack)) {
        return 0;
    }
    else {
        return 1;
    }
}

int main() {
    char card[21];
    scanf("%s", card);

    if (strlen(card) >= MAX) {
        printf("Error: NOT SUITABLE ARRAY");
        return 0;
    }

    int RESULT = CHECK(card);
    printf("%d\n", RESULT);
    return 0;
}

