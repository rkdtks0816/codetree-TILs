#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_N 30
#define MAX_M 900
#define MAX_K 1000

typedef struct {
  int row;
  int col;
  int value;
} Position_t;
typedef struct {
  Position_t q[MAX_N * MAX_N];
  int front;
  int rear;
} Queue_t;
void q_init(Queue_t *q) {
  q->front = 0;
  q->rear = 0;
}
void q_push(Queue_t *q, int row, int col, int value) {
  Position_t *insert_q = &q->q[q->rear];
  insert_q->row = row;
  insert_q->col = col;
  insert_q->value = value;
  ++q->rear;
}
Position_t q_front_pop(Queue_t *q) {
  Position_t front = q->q[q->front];
  ++q->front;
  return front;
}
/* 입력 */
void nmk_input(int *n, int *m, int *k) {
  scanf("%d %d %d", n, m, k);
}
typedef struct {
  Position_t nodes[MAX_N * MAX_N];
  int count;
} Nodes_t;
void grid_input(int n, Nodes_t *acs, Nodes_t *offices) {
  int temp;
  for (int ri = 0; ri < n; ++ri){
    for (int ci = 0; ci < n; ++ci) {
      scanf("%d", &temp);
      if (temp == 0) {
        continue;
      }
      else if (temp == 1) {
        Position_t *new_office = &offices->nodes[offices->count];
        new_office->row = ri;
        new_office->col = ci;
        ++offices->count;
      }
      else {
        Position_t *new_ac = &acs->nodes[acs->count];
        new_ac->row = ri;
        new_ac->col = ci;
        new_ac->value = temp;
        ++acs->count;
      }
    }
  }
}
void walls_input(int m, int walls[][MAX_N][2]) {
  int row, col, direction;
  for (int mi = 0; mi < m; ++mi) {
    scanf("%d %d %d", &row, &col, &direction);
    walls[row - 1][col - 1][direction] = 1;
  }
}
/* 에어컨 확산 */
int ac_check_left(int n, int row, int col, int walls[][MAX_N][2]){
  return col >= n - 1 || walls[row][col + 1][1] == 1;
}
int ac_check_up(int n, int row, int col, int walls[][MAX_N][2]){
  return row >= n - 1 || walls[row + 1][col][0] == 1;
}
int ac_check_right(int row, int col, int walls[][MAX_N][2]){
  return col < 0 || walls[row][col][1] == 1;
}
int ac_check_down(int row, int col, int walls[][MAX_N][2]){
  return row < 0 || walls[row][col][0] == 1;
}
int ac_check_range(int n, int row, int col, int value) {
  return row < 0 || row >= n || col < 0 || col >= n || value <= 0;
}
void ac_spread(int n, Position_t ac, int walls[][MAX_N][2], int ac_grid[][MAX_N]) {
  int value = 5;
  int spreaded[MAX_N][MAX_N] = {0};
  int d[] = {-1, 0, 1};
  Queue_t q;
  q_init(&q);
  
  /* 좌, 상, 우, 하 */
  switch (ac.value) {
    case 2: {
      if (ac.col >= 0) {
        ac_grid[ac.row][ac.col - 1] += value;
        q_push(&q, ac.row, ac.col - 1, value);
      }
      while (q.front < q.rear) {
        Position_t now = q_front_pop(&q);
        int nc = now.col - 1;
        int nv = now.value - 1;
        for (int di = 0; di < 3; ++di) {
          int nr = now.row + d[di];
          if (ac_check_range(n, nr, nc, nv)) {
            continue;
          }
          if (di == 0 && ac_check_up(n, nr, now.col, walls)) {
            continue;
          }
          if (di == 2 && ac_check_down(nr, now.col, walls)) {
            continue;
          }
          if (ac_check_left(n, nr, nc, walls) || spreaded[nr][nc]) {
            continue;
          }
          ac_grid[nr][nc] += nv;
          spreaded[nr][nc] = 1;
          q_push(&q, nr, nc, nv);
        }
      }
      break;
    }
    case 3: {
      if (ac.row >= 0) {
        ac_grid[ac.row - 1][ac.col] += value;
        q_push(&q, ac.row - 1, ac.col, value);
      }
      while (q.front < q.rear) {
        Position_t now = q_front_pop(&q);
        int nr = now.row - 1;
        int nv = now.value - 1;
        for (int di = 0; di < 3; ++di) {
          int nc = now.col + d[di];
          if (ac_check_range(n, nr, nc, nv)) {
            continue;
          }
          if (di == 0 && ac_check_left(n, now.row, nc, walls)) {
            continue;
          }
          if (di == 2 && ac_check_right(now.row, nc, walls)) {
            continue;
          }
          if (ac_check_up(n, nr, nc, walls) || spreaded[nr][nc]) {
            continue;
          }
          ac_grid[nr][nc] += nv;
          spreaded[nr][nc] = 1;
          q_push(&q, nr, nc, nv);
        }
      }
      break;
    }
    case 4: {
      if (ac.col < n - 1) {
        ac_grid[ac.row][ac.col + 1] += value;
        q_push(&q, ac.row, ac.col + 1, value);
        
      }
      while (q.front < q.rear) {
        Position_t now = q_front_pop(&q);
        int nc = now.col + 1;
        int nv = now.value - 1;
        for (int di = 0; di < 3; ++di) {
          int nr = now.row + d[di];
          if (ac_check_range(n, nr, nc, nv)) {
            continue;
          }
          if (di == 0 && ac_check_up(n, nr, now.col, walls)) {
            continue;
          }
          if (di == 2 && ac_check_down(nr, now.col, walls)) {
            continue;
          }
          if (ac_check_right(nr, nc, walls) || spreaded[nr][nc]) {
            continue;
          }
          ac_grid[nr][nc] += nv;
          spreaded[nr][nc] = 1;
          q_push(&q, nr, nc, nv);
        }
      }
      break;
    }
    case 5: {
      if (ac.row >= 0) {
        ac_grid[ac.row + 1][ac.col] += value;
        q_push(&q, ac.row + 1, ac.col, value);
      }
      while (q.front < q.rear) {
        Position_t now = q_front_pop(&q);
        int nr = now.row + 1;
        int nv = now.value - 1;
        for (int di = 0; di < 3; ++di) {
          int nc = now.col + d[di];
          if (ac_check_range(n, nr, nc, nv)) {
            continue;
          }
          if (di == 0 && ac_check_left(n, now.row, nc, walls)) {
            continue;
          }
          if (di == 2 && ac_check_right(now.row, nc, walls)) {
            continue;
          }
          if (ac_check_down(nr, nc, walls) || spreaded[nr][nc]) {
            continue;
          }
          ac_grid[nr][nc] += nv;
          spreaded[nr][nc] = 1;
          q_push(&q, nr, nc, nv);
        }
      }
      break;
    }
    default: {
      printf("잘못된 에어컨 값입니다.");
    }
  }
}
/* 섞기 */
void ac_mix(int n, int walls[][MAX_N][2], int ac_grid[][MAX_N]) {
  int change[MAX_N][MAX_N] = { 0 };
  int dr[] = {1, 0};
  int dc[] = {0, 1};
  for (int ri = 0; ri < n; ++ri) {
    for (int ci = 0; ci < n; ++ci) {
      int value = ac_grid[ri][ci];
      for (int di = 0; di < 2; ++di) {
        int nr = ri + dr[di];
        int nc = ci + dc[di];
        if (nr >= n || nc >= n) {
          continue;
        }
        if (di == 0 && ac_check_down(nr, nc, walls)) {
          continue;
        }
        if (di == 1 && ac_check_right(nr, nc, walls)) {
          continue;
        }
        int nv = ac_grid[nr][nc];
        int gap = (value - nv) / 4;
        change[ri][ci] -= gap;
        change[nr][nc] += gap;
      }
      ac_grid[ri][ci] += change[ri][ci];
    }
  }
}
/* 외벽 */
void ac_edge(int n, int ac_grid[][MAX_N]) {
  int edge[] = {0, n - 1};
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      if (ac_grid[edge[i]][edge[j]] != 0) {
        --ac_grid[edge[i]][edge[j]];
      }
    }
  }
  for (int i = 0; i < 2; ++i) {
    for (int ni = 1; ni < n - 1; ni++){
      if (ac_grid[edge[i]][ni] != 0) {
        --ac_grid[edge[i]][ni];
      }
      if (ac_grid[ni][edge[i]] != 0) {
        --ac_grid[ni][edge[i]];
      }
    }
  }
}
/* 사무실 온도 체크 */
int office_check(Nodes_t offices, int ac_grid[][MAX_N], int k){
  int flag = 0;
  for (int oi = 0; oi < offices.count; ++oi){
    int or = offices.nodes[oi].row;
    int oc = offices.nodes[oi].col;
    if (ac_grid[or][oc] < k) {
      flag = 1;
    }
  }
  return flag;
}
/* 사무실의 온도가 k 이상이 될 때까지*/
void simulate(int n, int k, Nodes_t acs, Nodes_t offices, int walls[][MAX_N][2]) {
  int ac_grid[MAX_N][MAX_N] = { 0 };
  int time = 0;
  while (office_check(offices, ac_grid, k)) {
    for (int ai = 0; ai < acs.count; ++ai) {
      ac_spread(n, acs.nodes[ai], walls, ac_grid);
    }
    ac_mix(n, walls, ac_grid);
    ac_edge(n, ac_grid);
    ++time;
  }
  printf("%d", time);
}

int main(void) {
  int n, m, k;
  nmk_input(&n, &m, &k);
  Nodes_t acs;
  acs.count = 0;
  Nodes_t offices;
  offices.count = 0;
  grid_input(n, &acs, &offices);
  
  int walls[MAX_N][MAX_N][2] = {0};
  walls_input(m, walls);
  
  simulate(n, k, acs, offices, walls);
  
  return 0;
}
