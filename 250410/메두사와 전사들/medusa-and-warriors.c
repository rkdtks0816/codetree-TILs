#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_N 60

typedef struct {
  int row;
  int col;
} Position_t;
/* 정답 */
typedef struct {
  int sum;
  int stone;
  int attack;
} Answer_t;
Answer_t a_init(void){
  Answer_t a;
  a.sum = 0;
  a.stone = 0;
  a.attack = 0;
  return a;
}
/* 마을 크기, 전사의 수 */
int n, m;
void n_m_input(void) {
  scanf("%d %d", &n, &m);
}
/* 상 하 좌 우 */
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
/* 상좌 상우 하우 하좌 */
int cr[] = {-1, -1, 1, 1};
int cc[] = {-1, 1, 1, -1};
/* 체크 */
int check_range(Position_t next) {
  return next.row < 0 || next.row >= n || next.col < 0 || next.col >= n;
}
int check_min(int left, int right) {
  return left >= right;
}
int check_road(int grid[][MAX_N], Position_t next) {
  return grid[next.row][next.col] == 1;
}
int check_same(Position_t left, Position_t right) {
  return left.row == right.row && left.col == right.col;
}
/* 큐 */
typedef struct {
  Position_t q[MAX_N * MAX_N];
  int front;
  int rear;
} Queue_t;
Queue_t q_init(void) {
  Queue_t q;
  q.front = 0;
  q.rear = 0;
  return q;
}
void q_push(Queue_t *q, Position_t new) {
  Position_t *push = &q->q[q->rear];
  push->row = new.row;
  push->col = new.col;
  ++q->rear;
}
Position_t q_front_pop(Queue_t *q) {
  Position_t front = q->q[q->front];
  ++q->front;
  return front;
}
/* 최단경로 */
void BFS(int grid[][MAX_N], Position_t start, Position_t end, int path[][MAX_N]) {
  Queue_t q = q_init();
  path[end.row][end.col] = 0;
  q_push(&q, end);
  while (q.front < q.rear){
    Position_t now = q_front_pop(&q);
    if (check_same(now, start)){
      continue;
    }
    Position_t next;
    for (int di = 0; di < 4; ++di){
      next.row = now.row + dr[di];
      next.col = now.col + dc[di];
      int nd = path[now.row][now.col] + 1;
      if (check_range(next)) {
        continue;
      }
      if (check_road(grid, next)) {
        continue;
      }
      if (check_min(nd, path[next.row][next.col])){
        continue;
      }
      path[next.row][next.col] = nd;
      q_push(&q, next);
    }
  }
}
/* 메두사 */
typedef struct {
  int path[MAX_N][MAX_N];
  Position_t now;
  Position_t end;
} Medosa_t;
void m_input(Medosa_t *m){
  scanf("%d %d %d %d", &m->now.row, &m->now.col, &m->end.row, &m->end.col);
}
void m_init(Medosa_t *m, int grid[][MAX_N]){
  for (int ri = 0; ri < n; ++ri){
    for (int ci = 0; ci < n; ++ci){
      m->path[ri][ci] = MAX_N * MAX_N;
    }
  }
  BFS(grid, m->now, m->end, m->path);
}
void m_move(Medosa_t *m) {
  for (int di = 0; di < 4; ++di){
    Position_t next;
    next.row = m->now.row + dr[di];
    next.col = m->now.col + dc[di];
    if (check_range(next)) {
      continue;
    }
    if (m->path[next.row][next.col] >= m->path[m->now.row][m->now.col]){
      continue;
    }
    m->now.row = next.row;
    m->now.col = next.col;
    break;
  }
}
/* 전사 */
typedef struct {
  int warrios[MAX_N][MAX_N];
  int m_range[4][MAX_N][MAX_N];
  int m_direction;
} Warriors_t;
void w_init(Warriors_t *w){
  for (int ri = 0; ri < n; ++ri){
    for (int ci = 0; ci < n; ++ci){
      w->warrios[ri][ci] = 0;
    }
  }
  for (int i = 0; i < 4; ++i){
    for (int ri = 0; ri < n; ++ri){
      for (int ci = 0; ci < n; ++ci){
        w->m_range[i][ri][ci] = 0;
      }
    }
  }
  w->m_direction = 0;
  int row, col;
  for (int mi = 0; mi < m; ++mi){
    scanf("%d %d", &row, &col);
    ++w->warrios[row][col];
  }
}
void w_m_left(int di, Position_t now, Position_t m, Queue_t *q, Warriors_t *w, int *w_count){
  Position_t side;
  side.row = -1;
  int level;
  switch (di) {
    case 0: {
      level =  m.row - now.row;
      if (m.col - level == now.col) {
        side.row = now.row + cr[0];
        side.col = now.col + cc[0];
      }
      break;
    }
    case 1: {
      level = now.row - m.row;
      if (m.col + level == now.col) {
        side.row = now.row + cr[2];
        side.col = now.col + cc[2];
      }
      break;
    }
    case 2: {
      level = m.col - now.col;
      if (m.row + level == now.row) {
        side.row = now.row + cr[3];
        side.col = now.col + cc[3];
      }
      break;
    }
    case 3: {
      level = now.col - m.col;
      if (m.row - level == now.row) {
        side.row = now.row + cr[1];
        side.col = now.col + cc[1];
      }
      break;
    }
  }
  if (side.row != -1 && !check_range(side)){
    if (w->warrios[side.row][side.col] == 0) {
      q_push(q, side);
    }
    else {
      *w_count += w->warrios[side.row][side.col];
    }
    w->m_range[di][side.row][side.col] = 1;
  }
}
void w_m_right(int di, Position_t now, Position_t m, Queue_t *q, Warriors_t *w, int *w_count){
  Position_t side;
  side.row = -1;
  int level;
  switch (di) {
    case 0: {
      level = m.row - now.row;
      if (m.col + level == now.col) {
        side.row = now.row + cr[1];
        side.col = now.col + cc[1];
      }
      break;
    }
    case 1: {
      level = now.row - m.row;
      if (m.col - level == now.col) {
        side.row = now.row + cr[3];
        side.col = now.col + cc[3];
      }
      break;
    }
    case 2: {
      level = m.col - now.col;
      if (m.row - level == now.row) {
        side.row = now.row + cr[0];
        side.col = now.col + cc[0];
      }
      break;
    }
    case 3: {
      level = now.col - m.col;
      if (m.row + level == now.row) {
        side.row = now.row + cr[2];
        side.col = now.col + cc[2];
      }
      break;
    }
  }
  if (side.row != -1 && !check_range(side)){
    if (w->warrios[side.row][side.col] == 0) {
      q_push(q, side);
    }
    else {
      *w_count += w->warrios[side.row][side.col];
    }
    w->m_range[di][side.row][side.col] = 1;
  }
}
void w_get_m_range(Medosa_t *m, Warriors_t *w){
  int w_max = 0;
  for (int di = 0; di < 4; ++di){
    int w_count = 0;
    Queue_t q = q_init();
    q_push(&q, m->now);
    while (q.front < q.rear){
      Position_t now = q_front_pop(&q);
      w_m_left(di, now, m->now, &q, w, &w_count);
      w_m_right(di, now, m->now, &q, w, &w_count);
      Position_t next;
      next.row = now.row + dr[di];
      next.col = now.col + dc[di];
      if (!check_range(next)){
        if (w->warrios[next.row][next.col] == 0) {
          q_push(&q, next);
        }
        else {
          w_count += w->warrios[next.row][next.col];
        }
        w->m_range[di][next.row][next.col] = 1;
      }
    }
    if (w_count > w_max) {
      w_max = w_count;
      w->m_direction = di;
    }
  }
}
void w_move (Warriors_t *w, Medosa_t *m, Answer_t *a) {
  int visited[MAX_N][MAX_N] = { 0 };
  for (int ri = 0; ri < n; ++ri) {
    for (int ci = 0; ci < n; ++ci) {
      if (w->warrios[ri][ci] == 0){
        continue;
      }
      if (w->warrios[ri][ci] == visited[ri][ci]) {
        continue;
      }
      if (m->now.row == ri && m->now.col == ci) {
        w->warrios[ri][ci] = 0;
        continue;
      }
      if (w->m_range[w->m_direction][ri][ci] == 1){
        a->stone += w->warrios[ri][ci];
        continue;
      }
      int nr = ri;
      int nc = ci;
      int temp = w->warrios[nr][nc] - visited[nr][nc];
      for (int i = 0; i < 2; ++i){
        int nnr = -1;
        int nnc = -1;
        if (nr > m->now.row) {
          nnr = nr - 1;
        }
        else if (nr < m->now.row) {
          nnr = nr + 1;
        }
        if (nc > m->now.col) {
          nnc = nc - 1;
        }
        else if (nc < m->now.col) {
          nnc = nc + 1;
        }
        temp = w->warrios[nr][nc] - visited[nr][nc];
        w->warrios[nr][nc] -= temp;
        if (nnr != -1 && w->m_range[w->m_direction][nnr][nc] != 1) {
          w->warrios[nnr][nc] += temp;
          ++a->sum;
          if (m->now.row == nnr && m->now.col == nc){
            a->attack += temp;
            w->warrios[nnr][nc] = 0;
            temp = 0;
          }
          nr = nnr;
        }
        else if (nnc != -1 && w->m_range[w->m_direction][nr][nnc] != 1) {
          w->warrios[nr][nnc] = temp;
          ++a->sum;
          if (m->now.row == nr && m->now.col == nnc){
            a->attack += temp;
            w->warrios[nr][nnc] = 0;
            temp = 0;
          }
          nc = nnc;
        }
        else {
          w->warrios[nr][nc] = temp;
          break;
        }
      }
      if (nr != ri || nc != ci) {
        visited[nr][nc] += temp;
      }
    }
  }
}

int main(void){
  n_m_input();
  Medosa_t m;
  m_input(&m);
  Warriors_t w;
  w_init(&w);
  int grid[MAX_N][MAX_N];
  for (int ri = 0; ri < n; ++ri){
    for (int ci = 0; ci < n; ++ci){
      scanf("%d", &grid[ri][ci]);
    }
  }
  m_init(&m, grid);
  if (grid[m.now.row][m.now.col] == MAX_N * MAX_N) {
    printf("-1");
    return 0;
  }
  m_move(&m);
  while(m.now.row != m.end.row || m.now.col != m.end.col) {
    Answer_t a = a_init();
    for (int i = 0; i < 4; ++i){
      for (int ri = 0; ri < n; ++ri){
        for (int ci = 0; ci < n; ++ci){
          w.m_range[i][ri][ci] = 0;
        }
      }
    }
    w.m_direction = 0;
    w_get_m_range(&m, &w);
    w_move(&w, &m, &a);
    printf("%d %d %d\n", a.sum, a.stone, a.attack);
    m_move(&m);
  }
  printf("0");
  return 0;
}
