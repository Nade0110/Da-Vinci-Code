#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

typedef enum {BLACK = 0, WHITE = 1} Color;

typedef struct {
	int num; // 타일 숫자
	Color color; // 타일 색
	bool isOpen; // 타일 공개 여부
} Tile;

Tile all_tile[24]; bool tileDraw[24]; int top; // 전체 타일 목록
Tile me[24]; int meLen; // 플레이어 타일
Tile bot[24]; int botLen; // 봇 타일

int top = 0;

bool knownNums[12] = { false }; //botTurn()에서 쓸 숫자 기록용 배열
int numCount[12] = { 0 }; //botTurn()에서 쓸 숫자 등장 획수 기록용 배열
bool guessFailed[11][12] = { false }; //botTunr()에서 쓸 실패한 추측 기록용 배열

Tile drawOne() {
	while (top < 24 && tileDraw[top]) {
		top++;
	}
	tileDraw[top] = true;
	return all_tile[top++];
}

int guess_num = 0; // 봇이 추측할때 추측할 숫자를 담는 전역변수

int compare(const void *a, const void *b) { // qsort에 이용할 정렬 함수
	Tile tileA = *(Tile *)a;
	Tile tileB = *(Tile *)b;

	if (tileA.num != tileB.num) {
		return tileA.num - tileB.num;
	}
	else {
		return tileA.color - tileB.color;
	}
}

void shuffle(Tile a[], int n) { // 전체 타일 셔플
	srand((unsigned)time(NULL));
	for (int i = n - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		Tile tmp = a[i]; a[i] = a[j]; a[j] = tmp;
	}
}

void initGame() { // 플레이어, 봇의 타일을 세팅
	int idx = 0;
	for (int num = 0; num <= 11; num++) {
		for (int color = 0; color <= 1; color++) {
			all_tile[idx].num = num;
			all_tile[idx].color = color;
			idx++;
		}
	}
	shuffle(all_tile, 24);
	for (int i = 0; i < 4; i++) {
		me[i] = all_tile[i];
		bot[i] = all_tile[i + 5];
		tileDraw[i] = true;
		meLen++;
		tileDraw[i + 5] = true;
		botLen++;
	}
}

void meDraw(OorX) { // 플레이어 드로우
	if (meLen == 11) return;
	me[meLen] = drawOne();
	if (OorX == false) me[meLen].isOpen = true;
	meLen++;
	qsort(me, meLen, sizeof(Tile), compare);
}

void botDraw(OorX) { // 봇 드로우
	if (botLen == 11) return;
	bot[botLen] = drawOne();
	if (OorX == false) bot[botLen].isOpen = true;
	botLen++;
	qsort(bot, botLen, sizeof(Tile), compare);
}

void printALLTest() {
	for (int i = 0; i < meLen; i++) {
		if (me[i].color == 0) {
			printf("(%d, black)\n", me[i].num);
		}
		else {
			printf("(%d, white)\n", me[i].num);
		}
	}

	printf("---------------------------------------------------\n");
	for (int i = 0; i < botLen; i++) {
		if (bot[i].color == 0) {
			printf("(%d, black)\n", bot[i].num);
		}
		else {
			printf("(%d, white)\n", bot[i].num);
		}
	}
}


void printALL() { // 플레이어와 봇의 타일 출력
	char *cards[50][8][27] = {
	{ // w0
		"________",
		"|■■■|",
		"|■  ■|",
		"|■  ■|",
		"|■  ■|",
		"|■■■|",
		"--------"
	},//w0
	{ // w1
		"________",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"--------"
	},//w1
	{ // w2
		"________",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"--------"
	},//w2
	{ // w3
		"________",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"--------"
	},//w3
	{ // w4
		"________",
		"|■  ■|",
		"|■  ■|",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//w4
	{ // w5
		"________",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"--------"
	},//w5
	{ // w6
		"________",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"--------"
	},//w6
	{ // w7
		"________",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//w7
	{ // w8
		"________",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"--------"
	},//w8
	{ // w9
		"________",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//w9
	{ // w10
		"___________",
		"|■ ■■■|",
		"|■ ■  ■|",
		"|■ ■  ■|",
		"|■ ■  ■|",
		"|■ ■■■|",
		"-----------"
	},//w10
	{ // w11
		"_______",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"-------"
	},//w11
	{ // b0
		"________",
		"|□□□|",
		"|□  □|",
		"|□  □|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//b0
	{ // b1
		"________",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"--------"
	},//b1
	{ // b2
		"________",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"--------"
	},//b2
	{ // b3
		"________",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"--------"
	},//b3
	{ // b4
		"________",
		"|□  □|",
		"|□  □|",
		"|□□□|",
		"|    □|",
		"|    □|",
		"--------"
	},//b4
	{ // b5
		"________",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"--------"
	},//b5
	{ // b6
		"________",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//b6
	{ // b7
		"________",
		"|□□□|",
		"|    □|",
		"|    □|",
		"|    □|",
		"|    □|",
		"--------"
	},//b7
	{ // b8
		"________",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//b8
	{ // b9
		"________",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"|    □|",
		"|    □|",
		"--------"
	},//b9
	{ // b10
		"___________",
		"|□ □□□|",
		"|□ □  □|",
		"|□ □  □|",
		"|□ □  □|",
		"|□ □□□|",
		"-----------"
	},//b10
	{ // b11
		"_______",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"-------"
	},//b11
	{ // Cw0
	"___Nd___",
	"|■■■|",
	"|■  ■|",
	"|■  ■|",
	"|■  ■|",
	"|■■■|",
	"--------"
	},//Cw0
	{ // Cw1
		"___Nd___",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"|  ■  |",
		"--------"
	},//Cw1
	{ // Cw2
		"___Nd___",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"--------"
	},//Cw2
	{ // Cw3
		"___Nd___",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"--------"
	},//Cw3
	{ // Cw4
		"___Nd___",
		"|■  ■|",
		"|■  ■|",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//Cw4
	{ // Cw5
		"___Nd___",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"|    ■|",
		"|■■■|",
		"--------"
	},//Cw5
	{ // Cw6
		"___Nd___",
		"|■■■|",
		"|■    |",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"--------"
	},//Cw6
	{ // Cw7
		"___Nd___",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//Cw7
	{ // Cw8
		"___Nd___",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"--------"
	},//Cw8
	{ // Cw9
		"___Nd___",
		"|■■■|",
		"|■  ■|",
		"|■■■|",
		"|    ■|",
		"|    ■|",
		"--------"
	},//Cw9
	{ // Cw10
		"_____Nd____",
		"|■ ■■■|",
		"|■ ■  ■|",
		"|■ ■  ■|",
		"|■ ■  ■|",
		"|■ ■■■|",
		"-----------"
	},//Cw10
	{ // Cw11
		"___Nd__",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"|■ ■|",
		"-------"
	},//Cw11
	{ // Cb0
		"___Nd___",
		"|□□□|",
		"|□  □|",
		"|□  □|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//Cb0
	{ // Cb1
		"___Nd___",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"|  □  |",
		"--------"
	},//Cb1
	{ // Cb2
		"___Nd___",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"--------"
	},//Cb2
	{ // Cb3
		"___Nd___",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"--------"
	},//Cb3
	{ // Cb4
		"___Nd___",
		"|□  □|",
		"|□  □|",
		"|□□□|",
		"|    □|",
		"|    □|",
		"--------"
	},//Cb4
	{ // Cb5
		"___Nd___",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"|    □|",
		"|□□□|",
		"--------"
	},//Cb5
	{ // Cb6
		"___Nd___",
		"|□□□|",
		"|□    |",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//Cb6
	{ // Cb7
		"___Nd___",
		"|□□□|",
		"|    □|",
		"|    □|",
		"|    □|",
		"|    □|",
		"--------"
	},//Cb7
	{ // Cb8
		"___Nd___",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"--------"
	},//Cb8
	{ // Cb9
		"___Nd___",
		"|□□□|",
		"|□  □|",
		"|□□□|",
		"|    □|",
		"|    □|",
		"--------"
	},//Cb9
	{ // Cb10
		"_____Nd____",
		"|□ □□□|",
		"|□ □  □|",
		"|□ □  □|",
		"|□ □  □|",
		"|□ □□□|",
		"-----------"
	},//Cb10
	{ // Cb11
		"___Nd__",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"|□ □|",
		"-------"
	},//Cb11
	{ //?w
		"________",
		"|■■■|",
		"|■■■|",
		"|■■■|",
		"|■■■|",
		"|■■■|",
		"--------"
	},//?w
	{ //?b
		"________",
		"|□□□|",
		"|□□□|",
		"|□□□|",
		"|□□□|",
		"|□□□|",
		"--------"
	}
	};//?b
	char **sumcardMe[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	char **sumcardBot[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	qsort(me, meLen, sizeof(Tile), compare);
	for (int i = 0; i < meLen; i++) {
		if (me[i].color == BLACK) {
			if (me[i].isOpen == false)
				sumcardMe[i] = cards[me[i].num + 36];
			else
				sumcardMe[i] = cards[me[i].num + 12];
		}
		else {
			if (me[i].isOpen == false)
				sumcardMe[i] = cards[me[i].num + 24];
			else
				sumcardMe[i] = cards[me[i].num];
		}
	}
	qsort(bot, botLen, sizeof(Tile), compare);
	for (int i = 0; i < botLen; i++) {
		if (bot[i].color == BLACK) {
			if (bot[i].isOpen == false)
				sumcardBot[i] = cards[49];
			else
				sumcardBot[i] = cards[bot[i].num + 12];
		}
		else {
			if (bot[i].isOpen == false)
				sumcardBot[i] = cards[48];
			else
				sumcardBot[i] = cards[bot[i].num];
		}
	}

	printf("MyTile\n");
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 11; j++) {
			if (sumcardMe[j] == '0') {
				break;
			}
			else {
				printf("%s ", sumcardMe[j][i]);
			}
		}
		printf("\n");
	}
	printf("BotTile\n");
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 11; j++) {
			if (sumcardBot[j] == '0') {
				break;
			}
			else {
				printf("%s ", sumcardBot[j][i]);
			}
		}
		printf("\n");
	}
}

void myTurn() { // 플레이어 턴
	int lo, num;
	while (1) {
		printf("추측할 타일의 위치와 숫자를 입력해주십시오.\n: ");
		scanf("%d %d", &lo, &num);
		if (lo - 1 >= botLen) {
			printf("잘못된 위치입니다.\n");
			continue;
		}
		else if (num > 11) {
			printf("잘못된 숫자입니다.\n");
			continue;
		}
		else if (bot[lo - 1].isOpen == true) {
			printf("이미 추측된 타일입니다.\n");
			continue;
		}
		else {
			break;
		}
	}
	if (bot[lo - 1].num == num) {
		bot[lo - 1].isOpen = true;
		meDraw(true);
		printf("맞았습니다!!\n");
	}
	else {
		meDraw(false);
		printf("틀렸습니다.\n");
	}
	bool isWin = true;
	for (int i = 0; i < botLen; i++) {
		if (bot[i].isOpen == false) {
			isWin = false;
			break;
		}
	}
	if (isWin == true) {
		printf("\n▨▨▨▨▨▨▨▨▨\n▨              ▨\n▨   YOU WIN!   ▨\n▨              ▨\n▨▨▨▨▨▨▨▨▨\n");
		exit(0);
	}
}

void botTurn() { // 봇 턴
	int possibleNums[11][12];
	int minCandidate = 13;
	int targetTile = -1;
	int targetNum = -1;
	printf("상대방의 추측이 시작됩니다...\n");
	for (int i = 0; i < meLen; i++) {
		if (me[i].isOpen)
			numCount[me[i].num]++;
	}
	for (int i = 0; i < botLen; i++) {
		if (bot[i].isOpen)
			numCount[bot[i].num]++;
	}

	for (int i = 0; i < meLen; i++) {
		if (me[i].isOpen) continue;

		int candidateCount = 0;
		int lastCandidate = -1;

		for (int num = 0; num <= 11; num++) {
			if (numCount[num] >= 2) continue;
			if (guessFailed[i][num]) continue;
			if (i > 0 && me[i - 1].isOpen && me[i - 1].num > num) continue;
			if (i + 1 < meLen && me[i + 1].isOpen && me[i + 1].num < num) continue;

			candidateCount++;
			lastCandidate = num;
			break;
		}

		if (lastCandidate != -1 && candidateCount < minCandidate) {
			minCandidate = candidateCount;
			targetTile = i;
			targetNum = lastCandidate;
		}
		//printf("%d %d\n", targetTile + 1, targetNum);
	}

	if (me[targetTile].num == targetNum) {
		printf("상대방의 추측이 맞았습니다.\n추측된 타일은 (%d, %d)입니다.\n", targetTile + 1, targetNum);
		me[targetTile].isOpen = true;
		botDraw(true);
	}
	else {
		printf("상대방의 추측이 틀렸습니다.\n");
		guessFailed[targetTile][targetNum] = true;
		botDraw(false);
	}

	bool isLose = true;
	for (int i = 0; i < meLen; i++) {
		if (me[i].isOpen == false) {
			isLose = false;
			break;
		}
	}
	if (isLose == true) {
		printf("\n▨▨▨▨▨▨▨▨▨\n▨              ▨\n▨   YOU LOSE   ▨\n▨              ▨\n▨▨▨▨▨▨▨▨▨\n");
		exit(0);
	}

	printf("계속하려면 아무 키나 누르십시오.");
}

int main() { // 메인
	initGame();
	char c = 1;
	while (c != '0') {
		system("cls");
		//printALLTest();
		printALL();
		myTurn();
		botTurn();

		c = getch();
	}
	return 0;
}
