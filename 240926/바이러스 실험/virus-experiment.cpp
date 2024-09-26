// 바이러스 실험 
// 삼성 SW 역량테스트 2018 하반기 오후 1번 문제
// https://www.codetree.ai/training-field/frequent-problems/problems/virus-experiment/description?page=3&pageSize=20
#include <iostream>
#include <queue>
using namespace std;
// 좌표가 1 ~ n
#define MAX_N 11
// 배지의 크기
int n;
// 바이러스의 개수
int m;
// 총 사이클의 수 k
int k;

// 양분 맵 n n 초기값 5
int YBMap[MAX_N][MAX_N];

// 양분 맵 초기화 함수 
void YBInit() {
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			YBMap[r][c] = 5;
}

// 바이러스 구조체 나이 어린 순
struct Virus {
	int row;
	int col;
	int age;
};

// 연산자 오버로딩 나이 어린 순
struct cmp {
	bool operator() (const Virus& left, const Virus& right) const {
		return left.age > right.age;  // 나이 어린 순으로 정렬
	}
};

// 바이러스 정보 우선 순위 큐 나이 어린 순
priority_queue<Virus, vector<Virus>, cmp> viruses;

// 마지막 추가되는 양분의 양
int addYB[MAX_N][MAX_N];

// 사이클
void cycle() {
	// 새로운 바이러스 큐 생성
	priority_queue<Virus, vector<Virus>, cmp> tempViruses;
	// 죽은 바이러스로 인해 추가될 양분
	int death[MAX_N][MAX_N] = { 0 };
	// 기존 바이러스 모두 양분 섭취
	while (!viruses.empty()) {
		int de = 1;
		// 준비 - 우선 순위 큐에서 바이러스 하나씩 꺼내기
		Virus nowVirus = viruses.top();
		viruses.pop();
		// 조건 - 양분과 바이러스 나이 비교
		// 섭취할 수 있을 때
		if (nowVirus.age <= YBMap[nowVirus.row][nowVirus.col]) {
			// 양분 섭취
			YBMap[nowVirus.row][nowVirus.col] -= nowVirus.age;
			tempViruses.push({ nowVirus.row, nowVirus.col, nowVirus.age + 1 });
		}
		// 섭취할 수 없을 때
		else {
			// 죽음
			death[nowVirus.row][nowVirus.col] += nowVirus.age / 2;
		}

	}
	// 번식
	// 살아 남은 5의 배수 나이 바이러스  번식
	while (!tempViruses.empty()) {
		// 준비 - 살아남은 바이러스 하나씩 꺼내기
		Virus liveVirus = tempViruses.top();
		tempViruses.pop();
		// 살아님은 바이러스 추가
		viruses.push(liveVirus);
		// 조건 - 5의 배수의 나이
		// 번식 하지 않는다면 그대로 바이러스 큐에 추가
		if (liveVirus.age % 5 != 0) continue;
		// 번식 한다면 8칸에 나이가 1인 바이러스 추가
		int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		for (int di = 0; di < 8; di++) {
			int nr = liveVirus.row + dr[di];
			int nc = liveVirus.col + dc[di];
			// 조건 - 범위 벗어나지 않게
			if (nr < 0 || nc < 0 || nr >= n || nc >= n) continue;
			viruses.push({ nr, nc, 1 });
		}
	}
	// 주어진 양분 추가
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++) {
			YBMap[r][c] += death[r][c];
			YBMap[r][c] += addYB[r][c];
		}
}

int main() {
	// 배지의 크기 n, 바이러스의 개수 m, 총 사이클의 수 k
	cin >> n >> m >> k;
	// 양분 추가
	YBInit();
	// 마지막에 추가되는 양분의 양
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			cin >> addYB[r][c];
	// 바이러스 정보
	for (int mi = 0; mi < m; mi++){
		int tempRow, tempCol, tempAge;
		cin >> tempRow >> tempCol >> tempAge;
		// 원점 0, 0 기준
		viruses.push({ tempRow - 1, tempCol - 1, tempAge });
	}
	// 사이클
	for (int ki = 0; ki < k; ki++)
		cycle();
	// 살아남은 바이러스의 수
	cout << viruses.size() << endl;
}