#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50

typedef struct NODE {
    int score;  // 4바이트
    char data;  // 1바이트
    struct NODE* point; //8바이트
} node; // padding 3바이트를 추가해서 16바이트 크기의 NODE구조체를 정의하고, 그 구조체의 별칭을 node로 정의

typedef struct {
    node* top;
} STACK;


void initialization(STACK* stack) {
    stack->top = NULL;
}

void push(STACK* stack, char data, int score) {
    node* temp = (node*)malloc(sizeof(node));// 노드를 가키리큰 포인터 변수 temp를 정의
    temp->data = data; // node에 data를 저장
    temp->score = score; //node에 score를 저장
    temp->point = stack->top; //node가 가리켜야 할 다음 주소를 stack의 top으로 가리키게 한다.
    stack->top = temp; // stack의 top을 node의 주소에 위치시킨다.
}


node* pop(STACK* stack) {
    if (stack->top == NULL) {
        return 0;
    }
    node* temp = stack->top; // temp는 stack의 top주소를 저장한다.
    stack->top = stack->top->point; //stack의 top을 원래 stack의 top이 가리키고 있는 다음 주소로 설정한다.
    return temp; // 원래 stack의 top을 반환한다.
}

int empty(STACK* stack) {
    return stack->top == NULL;
}


int check_match(char* buf) {
    STACK stack; // STACK자료형으로 정의된  stack변수 정의한다.
    initialization(&stack);

    int index = 0;
    while (buf[index] != '\0') { //scanf는 마지막에  '\0'을 붙인다.
        int in_Score = 0; // 내부 점수를 0으로 세팅한다.
        if (buf[index] == '(' || buf[index] == '[') { // 여는 괄호를 push해서 stack에 넣는다.
            push(&stack, buf[index], 0);
        } // push할때 stack의 주소, 문자로 설정한다. 여는 괄호는 닫는 괄호가 있어야 의미가 있기 때문에 0으로 설정한다.
        else if (buf[index] == ')' || buf[index] == ']') {
            // 닫는 괄호를 정의한다. 여기서부터 점수를 계산한다.

            if (empty(&stack)) {
                return 0;
            } // stack이 텅비었는지를 확인한다. -> 스택이 비어있다면 올바른 괄호가 될 수 없기 때문에 0을 return한다.


            while (!empty(&stack) && stack.top->data != '(' && stack.top->data != '[') {
                // 괄호 내부의 값을 처리한다.
                node* temp = pop(&stack);
                in_Score += temp->score;
                free(temp);
            }

            if (empty(&stack))
                return 0;

            node* top_node = pop(&stack);

            //올바른 괄호를 찾고, 중복된 괄호에 대해서는 곱해준다
            if (top_node->data == '(' && buf[index] == ')') {
                in_Score = in_Score > 0 ? in_Score * 3 : 3;
            }
            else if (top_node->data == '[' && buf[index] == ']') {
                in_Score = in_Score > 0 ? in_Score * 7 : 7;
            }
            else {
                free(top_node);
                return 0;
            }

            free(top_node);
            push(&stack, '\0', in_Score);
        }
        index++;
    }

    int RESULT = 0;
    while (!empty(&stack)) {
        node* temp = pop(&stack);
        // 정확한 짝을 찾지 못한 경우
        if (temp->data == '(' || temp->data == '[') {
            free(temp);
            return 0;
        }
        RESULT += temp->score;
        free(temp);
    }
    // 최종 결과  RESULT를 반환한다.
    return RESULT;
}

int main() {
    char buf[MAX_LEN];
    scanf("%s", buf);

    int RESULT = check_match(buf);

    if (RESULT == 0) {
        printf("0\n");
    }
    else {
        printf("%d\n", RESULT);
    }
    return 0;
}
