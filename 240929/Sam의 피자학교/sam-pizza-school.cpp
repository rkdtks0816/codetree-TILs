#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// 이동 좌상우하
int dr[] = { 0, -1, 0, 1 };
int dc[] = { -1, 0, 1, 0 };

// 밀가루 최댓값, 최솟값 구하는 함수
pair<int, int> findMaxMin(const vector<int>& target) {
    int max = 0, min = INT_MAX;
    for (int flour : target) {
        if (max < flour) max = flour;
        if (min > flour) min = flour;
    }
    return { max, min };
}

// 밀가루 양이 가장 적다면 밀가루 1 추가
void addflour(vector<int>& target) {
    pair<int, int> maxMin = findMaxMin(target);
    int min = maxMin.second;
    for (int& flour : target) {
        if (flour == min) flour += 1;
    }
}

// 얼마나 말 수 있는지 확인
pair<int, int> findRollSize(int targetSize) {
    pair<int, int> rollSize = { 1, 1 };
    while (true) {
        int nr = rollSize.first;
        int nc = rollSize.second;
        if (nr == nc) nr += 1;
        else nc += 1;
        if (nr * nc > targetSize) break;
        rollSize = { nr, nc };
    }
    return rollSize;
}

// 롤 채우기
vector<vector<int>> fillRoll(const vector<int>& target, pair<int, int> rollSize) {
    vector<vector<int>> tempRoll(rollSize.first, vector<int>(rollSize.second, 0));
    int nowRow = rollSize.first - 1;
    int nowCol = rollSize.second - 1;
    int nowDirect = 0;
    for (int i = rollSize.first * rollSize.second - 1; i >= 0; i--) {
        tempRoll[nowRow][nowCol] = target[i];
        int nr = nowRow + dr[nowDirect];
        int nc = nowCol + dc[nowDirect];
        if (nr < 0 || nc < 0 || nr >= rollSize.first || nc >= rollSize.second || tempRoll[nr][nc] != 0)
            nowDirect = (nowDirect + 1) % 4;
        nowRow += dr[nowDirect];
        nowCol += dc[nowDirect];
    }
    return tempRoll;
}

// 롤 누르기
vector<int> pressRoll(pair<int, int> rollSize, vector<vector<int>>& roll) {
    vector<vector<int>> tempRoll = roll;
    for (int row = 0; row < rollSize.first; row++) {
        for (int col = 0; col < rollSize.second; col++) {
            if (roll[row][col] == 0) continue;
            for (int di = 2; di < 4; di++) {
                int nr = row + dr[di];
                int nc = col + dc[di];
                if (nr < 0 || nc < 0 || nr >= rollSize.first || nc >= rollSize.second || roll[nr][nc] == 0)
                    continue;
                int diff = abs(roll[row][col] - roll[nr][nc]) / 5;
                if (roll[row][col] > roll[nr][nc]) {
                    tempRoll[row][col] -= diff;
                    tempRoll[nr][nc] += diff;
                } else {
                    tempRoll[row][col] += diff;
                    tempRoll[nr][nc] -= diff;
                }
            }
        }
    }
    vector<int> newDough;
    for (int col = 0; col < rollSize.second; col++) {
        for (int row = rollSize.first - 1; row >= 0; row--) {
            if (tempRoll[row][col] != 0) newDough.push_back(tempRoll[row][col]);
        }
    }
    return newDough;
}

// 도우 말기
vector<int> rollDough(vector<int>& target) {
    pair<int, int> rollSize = findRollSize(target.size());
    vector<vector<int>> roll = fillRoll(target, rollSize);
    vector<int> newDough = pressRoll(rollSize, roll);
    return newDough;
}

// 도우 접기
vector<int> foldDough(vector<int>& target) {
    int foldColSize = target.size() / 4;
    pair<int, int> rollSize = { 4, foldColSize };
    vector<vector<int>> tempRoll(4, vector<int>(foldColSize, 0));
    for (int i = 0; i < foldColSize; i++) {
        tempRoll[0][i] = target[3 * foldColSize - 1 - i];
        tempRoll[1][i] = target[foldColSize + i];
        tempRoll[2][i] = target[foldColSize - 1 - i];
        tempRoll[3][i] = target[3 * foldColSize + i];
    }
    vector<int> newDough = pressRoll(rollSize, tempRoll);
    return newDough;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> dough(n);
    for (int i = 0; i < n; i++) {
        cin >> dough[i];
    }
    int answer = 0;
    while (true) {
        pair<int, int> maxMin = findMaxMin(dough);
        if (maxMin.first - maxMin.second <= k) break;
        addflour(dough);
        dough = rollDough(dough);
        dough = foldDough(dough);
        answer++;
    }
    cout << answer;
    return 0;
}