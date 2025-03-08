#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>

#define MAX_N 30
#define MAX_M 20
#define MAX_F 20
#define MAX_NUM 20 * 20 + 5 * 10 * 10 + 10

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
int total_grid[MAX_NUM][5]; // 동 남 서 북 벽인지아닌지
int crossD(int now_d);
void unionGrid(void);

// 최단거리 찾기
typedef struct {
	int space_num;
	int dist;
} Queue;
int dists[MAX_NUM];
int checkEvent(int space_num, int dist);
void dfs(void);

// 정답 출력
void printAnswer(void);

int main(int argc, const char * argv[]) {

	inputNMF();
	inputGrid();
	inputWalls();
	inputEvent();
	insertNumber();
	unionGrid();
	dfs();
	printAnswer();

	return 0;
}
//
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
//
void inputWalls(void) {
	for (int di = 0; di < 5; di++) {
		for (int ri = 0; ri < M; ri++) {
			for (int ci = 0; ci < M; ci++) {
				scanf("%d", &walls[wall_d[di]][ri][ci]);
			}
		}
	}
}
//
void inputEvent(void) {
	for (int fi = 0; fi < F; fi++) {
		scanf("%d %d %d %d", &event[fi].r, &event[fi].c, &event[fi].d, &event[fi].v);
	}
}
//
void insertNumber(void) {
	int space_num = 0;
	int wall_num = 0;
	for (int nri = 0; nri < N; nri++) {
		for (int nci = 0; nci < N; nci++) {
			if (grid[nri][nci] == 4) {
				grid_exit = space_num;
				grid_numbers[nri][nci] = space_num;
				space_num++;
			}
			else if (grid[nri][nci] == 3) {
				grid_numbers[nri][nci] = wall_num;
				wall_num++;
			}
			else {
				grid_numbers[nri][nci] = space_num;
				space_num++;
			}
		}
	}
	for (int mdi = 0; mdi < 5; mdi++) {
		for (int mri = 0; mri < M; mri++) {
			for (int mci = 0; mci < M; mci++) {
				if (walls[mdi][mri][mci] == 2) {
					time_machine = space_num;
				}
				walls_numbers[mdi][mri][mci] = space_num;
				space_num++;
			}
		}
	}
}
//
int crossD(int now_d) {
	return (now_d + 2) % 4;
}
//
void unionGrid(void) {
	// 미지의 공간
	for (int nri = 0; nri < N; nri++) {
		for (int nci = 0; nci < N; nci++) {
			if (grid[nri][nci] == 3) {
				continue;
			}
			// 벽인지 확인
			int now_space_num = grid_numbers[nri][nci];
			if (grid[nri][nci] == 1) {
				total_grid[now_space_num][4] = 1;
			}
			// 동 남 서 북 연결
			for (int di = 0; di < 4; di++) {
				int nnr = nri + dr[di];
				int nnc = nci + dc[di];
				// 범위 확인
				if (nnr < 0 || nnc < 0 || nnr >= N || nnc >= N) {
					total_grid[now_space_num][di] = -1;
					continue;
				}
				// 벽인지 확인
				if (grid[nnr][nnc] == 3) {
					// 진행 방향의 반대 벽, row: M - 1, col: 벽 번호로 계산
					int wr = M - 1;
					int wc = grid_numbers[nnr][nnc];
					int wd = crossD(di);
					switch (di) {
					case 0: // 동: 서벽 col: wc / M
						wc /= M;
						break;
					case 1: // 남: 북벽 col: M - 1 - wc
						wc = M - 1 - wc;
						break;
					case 2: // 서: 동벽 col: M - 1 - wc / M
						wc = M - 1 - wc / M;
						break;
					default: // 북: 남벽 col: wc - M * (M - 1)
						wc -= M * (M - 1);
						break;
					}
					int wn = walls_numbers[wd][wr][wc];
					total_grid[now_space_num][di] = wn;
					total_grid[wn][1] = now_space_num;
				}
				else {
					total_grid[now_space_num][di] = grid_numbers[nnr][nnc];
				}
			}
		}
	}
	// 시간의 벽 동 남 서 북
	for (int mdi = 0; mdi < 4; mdi++) {
		for (int mri = 0; mri < M; mri++) {
			for (int mci = 0; mci < M; mci++) {
				int now_space_num = walls_numbers[mdi][mri][mci];
				if (walls[mdi][mri][mci] == 1) {
					total_grid[now_space_num][4] = 1;
				}
				for (int di = 0; di < 4; di++) {
					int mnr = mri + dr[di];
					int mnc = mci + dc[di];
					if (mnr < 0 || mnr >= M || mnc >= M) {
						continue;
					}
					// 왼쪽으로 넘을 때
					if (mnc < 0) {
						int nw = (mdi + 1) % 4;
						int nr = mnr;
						int nc = M - 1;
						int nd = crossD(di);
						int nn = walls_numbers[nw][nr][nc];
						total_grid[now_space_num][di] = nn;
						total_grid[nn][nd] = now_space_num;
					}
					else {
						total_grid[now_space_num][di] = walls_numbers[mdi][mnr][mnc];
					}
				}
			}
		}
	}
	// 시간의 벽 위
	for (int mri = 0; mri < M; mri++) {
		for (int mci = 0; mci < M; mci++) {
			int now_space_num = walls_numbers[4][mri][mci];
			if (walls[4][mri][mci] == 1) {
				total_grid[now_space_num][4] = 1;
			}
			for (int di = 0; di < 4; di++) {
				int mnr = mri + dr[di];
				int mnc = mci + dc[di];
				if (mnr < 0 || mnr >= M || mnc < 0 || mnc >= M) {
					int nr = 0;
					int nd = crossD(di);
					int nc = -1;
					if (mnr < 0) {
						nc = M - 1 - mnc;
					}
					else if (mnr >= M) {
						nc = mnc;
					}
					else if (mnc < 0) {
						nc = mnr;
					}
					else if (mnc >= M) {
						nc = M - 1 - mnr;
					}
					int nn = walls_numbers[di][nr][nc];
					total_grid[now_space_num][di] = nn;
					total_grid[nn][3] = now_space_num;
					continue;
				}
				total_grid[now_space_num][di] = walls_numbers[4][mnr][mnc];
			}
		}
	}
}
//
int checkEvent(int space_num, int dist) {
	for (int fi = 0; fi < F; fi++) {
		Event now = event[fi];
		int now_space_num = grid_numbers[now.r][now.c];
		if (now_space_num == space_num) {
			return 1;
		}
		// v의 배수 마다 이동
		int max_E = dist / now.v;
		for (int ei = 0; ei < max_E; ei++) {
			int next_space_num = total_grid[now_space_num][wall_d[now.d]];
			// 벽인지 확인
			if (total_grid[next_space_num][4] == 1 || next_space_num == grid_exit) {
				break;
			}
			if (next_space_num == space_num) {
				return 1;
			}
			now_space_num = next_space_num;
		}
	}
	return 0;
}
//
void dfs(void) {
	// 거리 초기화
	for (int di = 0; di < MAX_NUM; di++) {
		dists[di] = INT_MAX;
	}
	Queue q[MAX_NUM];
	int qf = 0; // 큐 프론트
	int qr = 0; // 큐 리어
	// 초기 q
	dists[time_machine] = 0;
	q[qr].space_num = time_machine;
	q[qr].dist = 0;
	qr++;
	while (qf < qr) {
		Queue now = q[qf];
		qf++;
		for (int di = 0; di < 4; di++) {
			int nn = total_grid[now.space_num][di];
			int nd = now.dist + 1;
			// 범위 확인
			if (nn == -1) {
				continue;
			}
			// 벽인지 확인
			if (total_grid[nn][4] == 1) {
				continue;
			}
			// 시간 이상 현상 확인
			if (checkEvent(nn, nd)) {
				continue;
			}
			// 최솟값인지 확인
			if (nd < dists[nn]) {
				dists[nn] = nd;
				q[qr].space_num = nn;
				q[qr].dist = nd;
				qr++;
			}
		}
	}
}
//
void printAnswer(void) {
	if (dists[grid_exit] == INT_MAX) {
		printf("-1\n");
	}
	else {
		printf("%d\n", dists[grid_exit]);
	}
}

