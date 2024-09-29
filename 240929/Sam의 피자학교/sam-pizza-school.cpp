// Sam의 피자학교
// 삼성 SW 역량테스트 2021 하반기 오후 2번 문제
// https://www.codetree.ai/training-field/frequent-problems/problems/sam-pizza-school/description?page=2&pageSize=20
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
// 롤 크기 최대
#define MAX_ROW 10
#define MAX_COL 100
// 이동 좌상우하
int dr[] = { 0, -1, 0, 1 };
int dc[] = { -1, 0, 1, 0 };
// 밀가루 최댓값, 최솟값 구하는 함수
pair<int, int> findMaxMin(vector<int> target) {
	// 최댓값, 최솟값
	int max = 0;
	int min = INT_MAX;
	// 배열 반복
	for (auto flour : target) {
		if (max < flour) max = flour;
		if (min > flour) min = flour;
	}
	return { max, min };
}
// 밀가루 양이 가장 적다면 밀가루 1 추가
void addflour(vector<int>& target) {
	// 밀가루 최솟값 찾기
	pair<int, int> maxMin = findMaxMin(target);
	int min = maxMin.second;
	// 최솟값이라면 1추가
	for (int& flour : target) {
		if (flour == min) flour += 1;
	}
}
// 얼마나 말 수 있는지 확인
pair<int, int> findRollSize(int targetSize) {
	// 1 1 2 2 3 3 4 4 5 5 6 6
	pair<int, int> rollSize = { 1, 1 };
	while (true) {
		int nr = rollSize.first;
		int nc = rollSize.second;
		if (rollSize.first == rollSize.second) nr += 1;
		else nc += 1;
		if (nr * nc >= targetSize) break;
		rollSize = { nr, nc };
	}
	return rollSize;
}
// 롤 채우기
vector<vector<int>> fillRoll(vector<int> target, pair<int, int> rollSize) {
	// 빈 롤 배열
	vector<vector<int>> tempRoll(MAX_ROW, vector<int>(MAX_COL, 0));
	// 롤의 끝부터 시계방향으로
	int nowRow = rollSize.first - 1;
	int nowCol = rollSize.second - 1;
	// 시작 방향 왼쪽
	int nowDirect = 0;
	for (int i = rollSize.first * rollSize.second - 1; i >= 0; i--) {
		tempRoll[nowRow][nowCol] = target[i];
		// 시계방향으로 갈 수 있는지 확인
		int nr = nowRow + dr[nowDirect];
		int nc = nowCol + dc[nowDirect];
		if (
			nr < 0 ||
			nc < 0 ||
			nr >= rollSize.first ||
			nc >= rollSize.second ||
			tempRoll[nr][nc] != 0 
			) nowDirect = (nowDirect + 1) % 4;
		nowRow += dr[nowDirect];
		nowCol += dc[nowDirect];
	}
	// 남은 밀가루 채우기
	int restFlour = target.size() - rollSize.first * rollSize.second;
	for (int i = 0; i < restFlour; i++) {
		tempRoll[rollSize.first - 1][rollSize.second + i] = target[rollSize.first * rollSize.second + i];
	}
	return tempRoll;
}
// 롤 누르기
vector<int> pressRoll(pair<int, int> rollSize, vector<vector<int>> roll) {
	// 새로운 롤
	vector<vector<int>> tempRoll = roll;
	// 모든 밀가루 계산
	for (int row = 0; row < rollSize.first; row++) {
		int col = 0;
		while (roll[row][col] != 0) {
			// 우 하 계산
			for (int di = 2; di < 4; di++) {
				int nr = row + dr[di];
				int nc = col + dc[di];
				if (
					nr < 0 ||
					nc < 0 ||
					nr >= MAX_ROW ||
					nc >= MAX_COL ||
					roll[nr][nc] == 0 
					) continue;
				if (roll[row][col] > roll[nr][nc]) {
					int d = (roll[row][col] - roll[nr][nc]) / 5;
					tempRoll[row][col] -= d;
					tempRoll[nr][nc] += d;
				}
				if (roll[row][col] < roll[nr][nc]) {
					int d = (roll[nr][nc] - roll[row][col]) / 5;
					tempRoll[row][col] += d;
					tempRoll[nr][nc] -= d;
				}
			}
			col++;
		}
	}
	// 롤 펴기
	vector<int> newDough;
	for (int col = 0; col < rollSize.second; col++) {
		for (int row = rollSize.first - 1; row >= 0; row--) {
			newDough.push_back(tempRoll[row][col]);
		}
	}
	int restRow = rollSize.first - 1;
	int restCol = rollSize.second;
	while (tempRoll[restRow][restCol] != 0) {
		newDough.push_back(tempRoll[restRow][restCol]);
		restCol++;
	}
	return newDough;
}
// 도우 말기
vector<int> rollDough(vector<int> target) {
	// 롤 사이즈 확인
	pair<int, int> rollSize = findRollSize(target.size());
	// 롤 채우기
	vector<vector<int>> roll = fillRoll(target, rollSize);
	// 롤 누르기
	vector<int> newDough = pressRoll(rollSize, roll);
	return newDough;
}
// 도우 접기
vector<int> foldDough(vector<int> target) {
	// 접은 도우 사이즈
	int foldColSize = target.size() / 4;
	pair<int, int> rollSize = { 4, foldColSize };
	// 도우 접기
	vector<vector<int>> tempRoll(MAX_ROW, vector<int>(MAX_COL, 0));
	for (int i = 0; i < foldColSize; i++) {
		tempRoll[0][i] = target[foldColSize * 3 - 1 - i];
		tempRoll[1][i] = target[foldColSize + i];
		tempRoll[2][i] = target[foldColSize - 1 - i];
		tempRoll[3][i] = target[foldColSize * 3 + i];
	}
	// 도우 누르기
	vector<int> newDough = pressRoll(rollSize, tempRoll);
	return newDough;
}


int main() {
	int n, k;
	cin >> n >> k;
	// 도우 배열
	vector<int> dough;
	// 피자 도우 추가
	for (int ni = 0; ni < n; ni++) {
		int temp;
		cin >> temp;
		dough.push_back(temp);
	}
	int answer = 0;
	while (true) {
		pair<int, int> maxMin = findMaxMin(dough);
		if (maxMin.first - maxMin.second <= k) break;
		// 밀가루 추가addflour(dough);
		// 도우 말기
		dough = rollDough(dough);
		// 도우 접기
		dough = foldDough(dough);
		answer++;
	}
	cout << answer;

	return 0;
}