#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

long long count = 0;

void move(int n, int from, int to) {

	int temp = 6 - from - to;

	if (n == 1) {
		count++;
		return;
	}
	else if (n >= 2) {
		move(n - 1, from, temp);
		move(1, from, to); 
		move(n - 1, temp, to);
	}
}

int main() {
	long long index;
	printf("The Number of disks : ");
	scanf("%lld", &index);
	move(index, 1, 2);
	printf("The total number of moves: %lld\n", count);
	return 0;
}