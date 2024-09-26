#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 좌표가 1 ~ n
#define MAX_N 11

// 배지의 크기
int n;
// 바이러스의 개수
int m;
// 총 사이클의 수 k
int k;

// 양분 맵 n x n, 초기값 5
int YBMap[MAX_N][MAX_N];

// 마지막 추가되는 양분의 양
int addYB[MAX_N][MAX_N];

// 양분 맵 초기화 함수
void YBInit() {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            YBMap[r][c] = 5;  // 각 칸에 양분 5로 초기화
        }
    }
}

// 바이러스 구조체 나이 어린 순
struct Virus {
    int row, col, age;
};

// 바이러스 비교 함수: 나이 어린 순으로 정렬
struct cmp {
    bool operator()(const Virus& left, const Virus& right) const {
        return left.age > right.age;  // 나이가 적은 것이 우선
    }
};

// 바이러스 정보 우선 순위 큐 나이 어린 순
priority_queue<Virus, vector<Virus>, cmp> viruses;

// 사이클
void cycle() {
    // 임시 바이러스 큐 생성
    priority_queue<Virus, vector<Virus>, cmp> tempViruses;

    // 1. 바이러스가 양분을 섭취
    while (!viruses.empty()) {
        Virus nowVirus = viruses.top();
        viruses.pop();

        // 바이러스가 나이만큼 양분을 섭취할 수 있을 때
        if (nowVirus.age <= YBMap[nowVirus.row][nowVirus.col]) {
            YBMap[nowVirus.row][nowVirus.col] -= nowVirus.age;  // 양분 소비
            tempViruses.push({ nowVirus.row, nowVirus.col, nowVirus.age + 1 });  // 나이 증가 후 임시 큐에 추가
        } else {
            // 바이러스가 죽음 -> 양분으로 변환
            YBMap[nowVirus.row][nowVirus.col] += nowVirus.age / 2;
        }
    }

    // 2. 번식
    while (!tempViruses.empty()) {
        Virus liveVirus = tempViruses.top();
        tempViruses.pop();

        // 살아남은 바이러스 다시 원래 큐로 추가
        viruses.push(liveVirus);

        // 나이가 5의 배수인 바이러스만 번식
        if (liveVirus.age % 5 == 0) {
            // 8방향 번식
            int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
            int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

            for (int di = 0; di < 8; di++) {
                int nr = liveVirus.row + dr[di];
                int nc = liveVirus.col + dc[di];

                // 배지 범위 확인
                if (nr >= 0 && nc >= 0 && nr < n && nc < n) {
                    viruses.push({ nr, nc, 1 });  // 나이 1인 바이러스 추가
                }
            }
        }
    }

    // 3. 주어진 양분 추가
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            YBMap[r][c] += addYB[r][c];  // 각 칸에 추가 양분 더함
        }
    }
}

int main() {
    // 배지의 크기 n, 바이러스의 개수 m, 총 사이클의 수 k 입력
    cin >> n >> m >> k;

    // 양분 초기화
    YBInit();

    // 추가 양분 입력
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            cin >> addYB[r][c];
        }
    }

    // 바이러스 정보 입력
    for (int i = 0; i < m; i++) {
        int tempRow, tempCol, tempAge;
        cin >> tempRow >> tempCol >> tempAge;
        viruses.push({ tempRow - 1, tempCol - 1, tempAge });  // 0 기반 인덱스로 변환
    }

    // k 사이클 실행
    for (int cycleCount = 0; cycleCount < k; cycleCount++) {
        cycle();  // 한 번의 사이클 실행
    }

    // 살아남은 바이러스 수 출력
    cout << viruses.size() << endl;

    return 0;
}