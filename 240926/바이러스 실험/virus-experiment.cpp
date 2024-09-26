#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define MAX_N 11

int n, m, k; // 배지 크기, 바이러스 개수, 사이클 수
int YBMap[MAX_N][MAX_N]; // 양분 맵
int addYB[MAX_N][MAX_N]; // 사이클마다 추가되는 양분

// 바이러스 구조체
struct Virus {
    int row, col, age;
};

// 나이 어린 순으로 정렬하기 위한 비교 구조체
struct cmp {
    bool operator() (const Virus& left, const Virus& right) const {
        return left.age > right.age; // 나이가 적을수록 우선순위가 높다
    }
};

priority_queue<Virus, vector<Virus>, cmp> viruses;

// 초기 양분 맵 설정 (모든 칸 5로 초기화)
void YBInit() {
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            YBMap[r][c] = 5;
}

// 사이클 실행
void cycle() {
    // 임시 큐
    priority_queue<Virus, vector<Virus>, cmp> tempViruses;
    int death[MAX_N][MAX_N] = {0}; // 죽은 바이러스로 인해 추가될 양분

    // 1. 양분 섭취 및 바이러스 처리
    while (!viruses.empty()) {
        Virus nowVirus = viruses.top();
        viruses.pop();

        // 바이러스가 양분을 먹을 수 있으면 나이를 증가시키고 tempViruses에 추가
        if (nowVirus.age <= YBMap[nowVirus.row][nowVirus.col]) {
            YBMap[nowVirus.row][nowVirus.col] -= nowVirus.age;
            tempViruses.push({nowVirus.row, nowVirus.col, nowVirus.age + 1});
        } else {
            // 양분 부족 시 죽고, 죽은 바이러스는 나이를 2로 나눈 만큼 양분이 됨
            death[nowVirus.row][nowVirus.col] += nowVirus.age / 2;
        }
    }

    // 2. 바이러스 번식
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (!tempViruses.empty()) {
        Virus liveVirus = tempViruses.top();
        tempViruses.pop();
        viruses.push(liveVirus); // 살아남은 바이러스는 다시 바이러스 큐에 추가

        // 나이가 5의 배수일 경우 번식
        if (liveVirus.age % 5 == 0) {
            for (int d = 0; d < 8; d++) {
                int nr = liveVirus.row + dr[d];
                int nc = liveVirus.col + dc[d];

                // 범위를 벗어나지 않는 칸에 번식
                if (nr >= 0 && nc >= 0 && nr < n && nc < n) {
                    viruses.push({nr, nc, 1}); // 나이 1로 번식
                }
            }
        }
    }

    // 3. 죽은 바이러스로 인한 양분 추가
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            YBMap[r][c] += death[r][c];
        }
    }

    // 4. 각 칸에 추가되는 양분 더하기
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            YBMap[r][c] += addYB[r][c];
        }
    }
}

int main() {
    // 입력 처리
    cin >> n >> m >> k;
    YBInit(); // 양분 초기화

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            cin >> addYB[r][c];
        }
    }

    // 바이러스 정보 입력
    for (int i = 0; i < m; i++) {
        int r, c, age;
        cin >> r >> c >> age;
        viruses.push({r - 1, c - 1, age}); // 0-based index로 변환
    }

    // k번의 사이클 실행
    for (int i = 0; i < k; i++) {
        cycle();
    }

    // 살아남은 바이러스의 수 출력
    cout << viruses.size() << endl;

    return 0;
}