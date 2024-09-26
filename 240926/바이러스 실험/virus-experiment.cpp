#include <iostream>
#include <queue>
using namespace std;

#define MAX_N 11

int n, m, k; // 배지의 크기, 바이러스의 개수, 총 사이클의 수

int YBMap[MAX_N][MAX_N]; // 양분 맵
int addYB[MAX_N][MAX_N]; // 마지막에 추가되는 양분의 양

void YBInit() {
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			YBMap[r][c] = 5; // 초기 양분 설정
}

// 바이러스 구조체 (나이 어린 순)
struct Virus {
	int row, col, age;
	bool operator> (const Virus& right) const {
		return age > right.age; // 나이가 어린 것이 우선순위가 높게
	}
};

// 바이러스 정보 우선 순위 큐 (나이 어린 순)
priority_queue<Virus, vector<Virus>, greater<Virus>> viruses;

void cycle() {
	// 새로운 바이러스 큐 생성 (임시)
	priority_queue<Virus, vector<Virus>, greater<Virus>> tempViruses;
	
	// 기존 바이러스가 양분을 섭취하는 과정
	while (!viruses.empty()) {
		Virus nowVirus = viruses.top();
		viruses.pop();
		
		// 양분과 나이를 비교하여 섭취 가능한 경우
		if (nowVirus.age <= YBMap[nowVirus.row][nowVirus.col]) {
			YBMap[nowVirus.row][nowVirus.col] -= nowVirus.age;
			tempViruses.push({ nowVirus.row, nowVirus.col, nowVirus.age + 1 });
		} else {
			// 죽음 - 양분을 추가
			YBMap[nowVirus.row][nowVirus.col] += nowVirus.age / 2;
		}
	}
	
	// 번식하는 바이러스 처리
	while (!tempViruses.empty()) {
		Virus liveVirus = tempViruses.top();
		tempViruses.pop();
		
		// 바이러스 다시 추가
		viruses.push(liveVirus);
		
		// 번식 조건 - 나이가 5의 배수인 경우
		if (liveVirus.age % 5 != 0) continue;
		
		// 8방향으로 번식
		int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		for (int di = 0; di < 8; di++) {
			int nr = liveVirus.row + dr[di];
			int nc = liveVirus.col + dc[di];
			
			// 범위 벗어나는지 확인
			if (nr < 0 || nc < 0 || nr >= n || nc >= n) continue;
			viruses.push({ nr, nc, 1 });
		}
	}
	
	// 주어진 양분 추가
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			YBMap[r][c] += addYB[r][c];
}

int main() {
	cin >> n >> m >> k;
	
	// 양분 맵 초기화
	YBInit();
	
	// 추가 양분 입력
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			cin >> addYB[r][c];
	
	// 바이러스 정보 입력
	for (int mi = 0; mi < m; mi++) {
		int tempRow, tempCol, tempAge;
		cin >> tempRow >> tempCol >> tempAge;
		viruses.push({ tempRow - 1, tempCol - 1, tempAge });
	}
	
	// 사이클 실행
	for (int ki = 0; ki < k; ki++)
		cycle();
	
	// 살아남은 바이러스의 수 출력
	cout << viruses.size() << endl;
	
	return 0;
}