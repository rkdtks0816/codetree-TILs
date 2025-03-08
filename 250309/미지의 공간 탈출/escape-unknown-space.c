#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>

#define MAX_N 30
#define MAX_M 20
#define MAX_F 20
#define MAX_NUM 1500 // 여유롭게 증가

// N, M, F 입력
int N, M, F;
void inputNMF(void);

// 미지의 공간
int grid[MAX_N][MAX_N];
void inputGrid(void);

// 동 남 서 북 윗면 단면도
int wall_d[] = { 0, 2, 1, 3, 4 };
int walls[5][MAX_M][MAX_M];
void inputWalls(void);

// 시간 이상 현상
typedef struct {
    int r;
    int c;
    int d;
    int v;
} Event;
Event event[MAX_F];
void inputEvent(void);

// 평면도 번호 매기기
int grid_exit; // 탈출구
int time_machine; // 타임머신
int grid_numbers[MAX_N][MAX_N];
int walls_numbers[5][MAX_M][MAX_M];
void insertNumber(void);

// 평면도 합치기
int dr[] = { 0, 1, 0, -1 }; // 동 남 서 북
int dc[] = { 1, 0, -1, 0 };
int total_grid[MAX_NUM][5]; // 동 남 서 북 벽인지 아닌지
int crossD(int now_d);
void unionGrid(void);

// 최단거리 찾기
typedef struct {
    int space_num;
    int dist;
} Queue;
int dists[MAX_NUM];
int checkEvent(int space_num, int dist);
void bfs(void); // dfs -> bfs로 변경

// 정답 출력
void printAnswer(void);

int main() {
    inputNMF();
    inputGrid();
    inputWalls();
    inputEvent();
    insertNumber();
    unionGrid();
    bfs(); // bfs로 변경
    printAnswer();
    return 0;
}

// 입력 처리
void inputNMF(void) {
    scanf("%d %d %d", &N, &M, &F);
}

void inputGrid(void) {
    for (int ri = 0; ri < N; ri++) {
        for (int ci = 0; ci < N; ci++) {
            scanf("%d", &grid[ri][ci]);
        }
    }
}

void inputWalls(void) {
    for (int di = 0; di < 5; di++) {
        for (int ri = 0; ri < M; ri++) {
            for (int ci = 0; ci < M; ci++) {
                scanf("%d", &walls[wall_d[di]][ri][ci]);
            }
        }
    }
}

void inputEvent(void) {
    for (int fi = 0; fi < F; fi++) {
        scanf("%d %d %d %d", &event[fi].r, &event[fi].c, &event[fi].d, &event[fi].v);
    }
}

// 인덱스 초과 방지
int crossD(int now_d) {
    return (now_d + 2) % 4;
}

// 최단 거리 찾기 (bfs 사용)
void bfs(void) {
    for (int di = 0; di < MAX_NUM; di++) {
        dists[di] = INT_MAX;
    }
    
    Queue q[MAX_NUM * 2]; // 큐 크기 증가
    int qf = 0, qr = 0;
    
    dists[time_machine] = 0;
    q[qr].space_num = time_machine;
    q[qr].dist = 0;
    qr++;
    
    while (qf < qr) {
        Queue now = q[qf++];
        
        for (int di = 0; di < 4; di++) {
            int nn = total_grid[now.space_num][di];
            int nd = now.dist + 1;
            
            if (nn == -1 || total_grid[nn][4] == 1) continue;
            if (checkEvent(nn, nd)) continue;
            
            if (nd < dists[nn]) {
                dists[nn] = nd;
                q[qr].space_num = nn;
                q[qr].dist = nd;
                qr++;
                if (qr >= MAX_NUM * 2) return; // 큐 오버플로 방지
            }
        }
    }
}

// 이벤트 처리
int checkEvent(int space_num, int dist) {
    for (int fi = 0; fi < F; fi++) {
        Event now = event[fi];
        int now_space_num = grid_numbers[now.r][now.c];
        
        if (now_space_num == space_num) return 1;
        
        int max_E = dist / now.v;
        for (int ei = 0; ei < max_E; ei++) {
            int next_space_num = total_grid[now_space_num][wall_d[now.d]];
            if (total_grid[next_space_num][4] == 1 || next_space_num == grid_exit) break;
            if (next_space_num == space_num) return 1;
            now_space_num = next_space_num;
        }
    }
    return 0;
}

// 정답 출력
void printAnswer(void) {
    if (dists[grid_exit] == INT_MAX) {
        printf("-1\n");
    } else {
        printf("%d\n", dists[grid_exit]);
    }
}
