#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define MAX_N 11

int n, m, k; // 배지 크기, 바이러스 개수, 사이클 수
int YBMap[MAX_N][MAX_N]; // 양분 맵
int addYB[MAX_N][MAX_N]; // 사이클마다 추가되는 양분

// 바이러스 정보를 배열로 관리
vector<int> viruses[MAX_N][MAX_N];

// 초기 양분 맵 설정 (모든 칸 5로 초기화)
void YBInit() {
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            YBMap[r][c] = 5;
}

// 사이클 실행
void cycle() {
    vector<int> newViruses[MAX_N][MAX_N];  // 임시 배열
    int death[MAX_N][MAX_N] = {0}; // 죽은 바이러스로 인해 추가될 양분

    // 1. 양분 섭취 및 바이러스 처리
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (viruses[r][c].empty()) continue;  // 바이러스 없으면 스킵

            // 나이 순으로 정렬된 상태에서 가장 어린 바이러스부터 양분 섭취
            vector<int> survived;  // 살아남은 바이러스 임시 저장
            sort(viruses[r][c].begin(), viruses[r][c].end());  // 바이러스 나이 순 정렬

            for (int age : viruses[r][c]) {
                if (YBMap[r][c] >= age) {
                    YBMap[r][c] -= age;
                    survived.push_back(age + 1);  // 나이 1 증가
                } else {
                    death[r][c] += age / 2;  // 죽은 바이러스의 양분 추가
                }
            }

            viruses[r][c] = survived;  // 살아남은 바이러스들 업데이트
        }
    }

    // 2. 번식
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            for (int age : viruses[r][c]) {
                if (age % 5 == 0) {  // 나이가 5의 배수인 바이러스만 번식
                    for (int d = 0; d < 8; d++) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];

                        if (nr >= 0 && nc >= 0 && nr < n && nc < n) {
                            newViruses[nr][nc].push_back(1);  // 나이 1인 바이러스 추가
                        }
                    }
                }
            }
        }
    }

    // 새로 번식된 바이러스 추가
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            viruses[r][c].insert(viruses[r][c].end(), newViruses[r][c].begin(), newViruses[r][c].end());
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
        viruses[r - 1][c - 1].push_back(age);  // 0-based index로 변환하여 삽입
    }

    // k번의 사이클 실행
    for (int i = 0; i < k; i++) {
        cycle();
    }

    // 살아남은 바이러스의 수 계산 및 출력
    int result = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            result += viruses[r][c].size();  // 살아남은 바이러스 수
        }
    }

    cout << result << endl;

    return 0;
}