// 미로 탐색 프로그램
#include <stdio.h>
#define MAZE_SIZE 6
#define MAX_STACK_SIZE 100

typedef struct {
	short r;
	short c;
} location; // 좌표
typedef struct {
	location data[MAX_STACK_SIZE];
	int top;
} PathStack; // 좌표를 담는 스택
// 전역 변수
location here = { 2,0 }, entry = { 2,0 }; // 현재 좌표와 시작 좌표
char maze[MAZE_SIZE][MAZE_SIZE] = { // 미로 구조 선언 및 초기화
	{'1', '1', '1', '1', '1', '1'},
	{'1', '0', '1', '0', '0', '1'},
	{'e', '0', '0', '0', '1', '1'},
	{'1', '0', '1', '0', '1', '1'},
	{'1', '0', '1', '0', '0', 'x'},
	{'1', '1', '1', '1', '1', '1'}
};

// 스택 초기화 함수
void init_stack(PathStack* s) { s->top = -1; }

// 공백 상태 검출 함수
int is_empty(PathStack* s) { return (s->top == -1); }

// 포화 상태 검출 함수
int is_full(PathStack* s) { return (s->top == (MAX_STACK_SIZE - 1)); }

// 삽입함수
void push(PathStack* s, location item) {
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		exit(1);
	}
	else s->data[++(s->top)] = item;
}

// 삭제함수
location pop(PathStack* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

// 엿보기함수
location peek(PathStack* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)];
}

//미로 출력 함수
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]) {
	for (int r = 0; r < MAZE_SIZE; r++) { // 행 개수만큼 반복
		for (int c = 0; c < MAZE_SIZE; c++) { // 열 개수만큼 반복
			printf("%c", maze[r][c]); // 해당 좌표의 미로상태 출력
		}
		printf("\n");
	}
	printf("\n");
}

//메인 함수
int main(void) {
	PathStack path; // 경로 추적용 스택 생성
	init_stack(&path);
	int visited[MAZE_SIZE][MAZE_SIZE] = { 0 }; //방문 기록용 테이블
	here = entry; // 현재위치 = 시작위치
	short* r = &(here.r);
	short* c = &(here.c);

	push(&path, here); // 현재위치 경로 스택에 저장
	visited[*r][*c] = 1; // 방문테이블에 기록

	// 경로 스택이 공백이 되기 전까지 반복
	while (!is_empty(&path)) {

		// 현재 위치가 도착지점인 경우
		if (maze[*r][*c] == 'x') {
			printf("탐색 성공\n");
			break; // 루프 탈출
		}

		// 남쪽이 이동가능이고, 안가본 곳이면
		if (maze[*r + 1][*c] != '1' && !visited[*r + 1][*c]) {
			(*r)++;
			visited[*r][*c] = 1; // 방문테이블에 기록
			if (maze[*r][*c] != 'x') maze[*r][*c] = '.'; // 탐색 족적 남기기
			maze_print(maze);
			push(&path, here); // 경로 스택에 추가
		}

		// 북쪽이 이동가능이고. 안가본 곳이면
		else if (maze[*r - 1][*c] != '1' && !visited[*r - 1][*c]) {
			(*r)--;
			visited[*r][*c] = 1; // 방문테이블에 기록
			if (maze[*r][*c] != 'x') maze[*r][*c] = '.'; // 탐색 족적 남기기
			maze_print(maze);
			push(&path, here); // 경로 스택에 추가
		}

		// 동쪽이 이동가능이고, 안가본 곳이면
		else if (maze[*r][*c + 1] != '1' && !visited[*r][*c + 1]) {
			(*c)++;
			visited[*r][*c] = 1; // 방문테이블에 기록
			if (maze[*r][*c] != 'x') maze[*r][*c] = '.'; // 탐색 족적 남기기
			maze_print(maze);
			push(&path, here); // 경로 스택에 추가
		}

		// 서쪽이 이동가능이고, 안가본 곳이면
		else if (maze[*r][*c - 1] != '1' && !visited[*r][*c - 1]) {
			(*c)--;
			visited[*r][*c] = 1; // 방문테이블에 기록
			if (maze[*r][*c] != 'x') maze[*r][*c] = '.'; // 탐색 족적 남기기
			maze_print(maze);
			push(&path, here); // 경로 스택에 추가
		}

		// 막다른 길이라면
		else {
			pop(&path); // 경로 스택에서 현재 좌표 제거
			if (is_empty(&path)) { // 경로 스택이 공백이면 탐색 실패
				printf("탐색 실패\n");
				return 0;
			}
			here = peek(&path); // 현재 좌표를 이전 경로 좌표로 업데이트
		}
	}

	// 최종 경로 표시
	printf("\n최종 경로:\n");
	pop(&path); // 도착지는 경로 표시에서 제외
	while (!is_empty(&path)) { // 경로 스택이 공백이 되기 전까지 반복
		location p = pop(&path); // 가장 최근의 경로 좌표 꺼내오기
		maze[p.r][p.c] = '*'; // 해당 좌표에 경로 족적 남기기
	}
	maze[entry.r][entry.c] = 'e'; // 시작 지점 'e'복원
	maze_print(maze);
	return 0;
}