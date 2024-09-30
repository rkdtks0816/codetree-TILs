// 예술성
// 삼성 SW 역량테스트 2022 상반기 오전 2번 문제
// https ://www.codetree.ai/training-field/frequent-problems/problems/artistry/description?page=4&pageSize=5
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
// n 최대
#define MAX_N 29
// n
int n;
// n Map
vector<vector<int>> nMap;
// 상하좌우
int dr[] = { -1, 1, 0, 0 };
int dc[] = { 0, 0, -1, 1 };
// 그룹 나누기
vector<vector<pair<int, int>>> splitGroup(vector<vector<int>> targetMap) {
	// 그룹 전체 벡터
	vector<vector<pair<int, int>>> allGroup;
	// 그룹 속했는지
	bool visited[MAX_N][MAX_N] = { 0, };
	// 전체 맵 탐색
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			// 이미 속한 색인지
			if (visited[row][col]) continue;
			// 그룹 초기화 (0 : 그룹 숫자, 그룹 칸 수, 1 ~ : 그룹 좌표)
			vector<pair<int, int>> group;
			// 그룹 숫자, 칸 수 초기값
			group.push_back({ targetMap[row][col], 1 });
			// BFS Q
			queue<pair<int, int>> q;
			// 초기값
			q.push({ row, col });
			visited[row][col] = true;
			group.push_back({ row, col });
			// BFS
			while (!q.empty()) {
				// q 꺼내기
				pair<int, int> now = q.front();
				q.pop();
				// 상하좌우 이동
				for (int di = 0; di < 4; di++) {
					// 다음 좌표
					int nr = now.first + dr[di];
					int nc = now.second + dc[di];
					// 좌표 밖 확인
					if (nr < 0 || nc < 0 || nr >= n || nc >= n) continue;
					// 같은 색인지 확인
					if (targetMap[row][col] != targetMap[nr][nc]) continue;
					// 속한 색인지 확인
					if (visited[nr][nc]) continue;
					q.push({ nr, nc });
					visited[nr][nc] = true;
					group[0].second += 1;
					group.push_back({ nr, nc });
				}
			}
			allGroup.push_back(group);
		}
	}
	return allGroup;
}
// 맞닿은 변 계산
int joinGroup(vector<pair<int, int>> leftGroup, vector<pair<int, int>> rightGroup) {
	int cnt = 0;
	for (int li = 1; li < leftGroup.size(); li++) {
		for (int ri = 1; ri < rightGroup.size(); ri++) {
			if (leftGroup[li].first != rightGroup[ri].first && leftGroup[li].second != rightGroup[ri].second) continue;
			if (leftGroup[li].first == rightGroup[ri].first && abs(leftGroup[li].second - rightGroup[ri].second) != 1) continue;
			if (leftGroup[li].second == rightGroup[ri].second && abs(leftGroup[li].first - rightGroup[ri].first) != 1) continue;
			cnt += 1;
		}
	}
	return cnt;
}
// 조화로움 계산
int artScore(vector<vector<pair<int, int>>> allGroup) {
	int sum = 0;
	// 전체 조합 계산
	for (int li = 0; li < allGroup.size() - 1; li++) {
		vector<pair<int, int>> leftGroup = allGroup[li];
		for (int ri = li + 1; ri < allGroup.size(); ri++) {
			vector<pair<int, int>> rightGroup = allGroup[ri];
			// 조화로움 계산
			sum += (leftGroup[0].second + rightGroup[0].second) * leftGroup[0].first * rightGroup[0].first * joinGroup(leftGroup, rightGroup);
		}
	}
	return sum;
}
// 맵 회전
vector<vector<int>> rotateMap(vector<vector<int>> targetMap) {
	// 맵 복사
	vector<vector<int>> newMap = targetMap;
	// 반시계 십자 회전
	/*
		시계 방향
		90 :
			col
			n - 1 - row
		180 :
			n - 1 - row
			n - 1 - col
		270 :
			n - 1 - col
			n - 1 - n + 1 + row
	*/
	// 중앙 값
	int centerIndex = n / 2;
	// 세로 중앙 회전
	for (int vi = 0; vi < n; vi++) {
		int nr = n - 1 - centerIndex;
		int nc = vi;
		newMap[nr][nc] = targetMap[vi][centerIndex];
	}
	// 가로 중앙 회전
	for (int hi = 0; hi < n; hi++) {
		int nr = n - 1 - hi;
		int nc = centerIndex;
		newMap[nr][nc] = targetMap[centerIndex][hi];
	}
	// 4개 사각형 회전
	for (int si = 0; si < 4; si++) {
		// 시작 인덱스
		pair<int, int> startIndex;
		switch (si) {
			case 0 :
				startIndex = { 0, 0 };
				break;
			case 1:
				startIndex = { 0, centerIndex + 1 };
				break;
			case 2:
				startIndex = { centerIndex + 1, 0 };
				break;
			case 3:
				startIndex = { centerIndex + 1, centerIndex + 1 };
				break;
		}
		for (int sr = 0; sr < centerIndex; sr++) {
			for (int sc = 0; sc < centerIndex; sc++) {
				int nowr = sr + startIndex.first;
				int nowc = sc + startIndex.second;
				int nr = sc + startIndex.first;
				int nc = centerIndex - 1 - sr + startIndex.second;
				newMap[nr][nc] = targetMap[nowr][nowc];
			}
		}
	}
	return newMap;
}

int main() {
	cin >> n;
	for (int nr = 0; nr < n; nr++) {
		vector<int> tempVector;
		for (int nc = 0; nc < n; nc++) {
			int temp;
			cin >> temp;
			tempVector.push_back(temp);
		}
		nMap.push_back(tempVector);
	}
	// 정답
	int answer = 0;
	// 그룹
	vector<vector<pair<int, int>>> allGroup;
	// 초기점수
	allGroup = splitGroup(nMap);
	answer += artScore(allGroup);
	// 회전 후 점수 더하기 3회
	for (int i = 0; i < 3; i++) {
		nMap = rotateMap(nMap);
		allGroup = splitGroup(nMap);
		answer += artScore(allGroup);
	}
	cout << answer;

	return 0;
}