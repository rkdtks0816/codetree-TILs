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
int w_m_direction(int di, Position_t m, Position_t now){
  if (di == 0) {
    return m.col - now.col;
  }
  else if (di == 1){
    return now.col - m.col;
  }
  else if (di == 2){
    return now.row - m.row;
  }
  else if (di == 3){
    return m.row - now.row;
  }
  return MAX_N;
}
Position_t w_m_left(int di, Position_t m) {
  int d_left[] = {0, 2, 3, 1};
  Position_t new;
  new.row = m.row + cr[d_left[di]];
  new.col = m.col + cc[d_left[di]];
  return new;
}
Position_t w_m_center(int di, Position_t m) {
  Position_t new;
  new.row = m.row + dr[di];
  new.col = m.col + dc[di];
  return new;
}
Position_t w_m_right(int di, Position_t m) {
  int d_right[] = {1, 3, 0, 2};
  Position_t new;
  new.row = m.row + cr[d_right[di]];
  new.col = m.col + cc[d_right[di]];
  return new;
}
void w_q_push(Queue_t *q, Position_t next, int di, Warriors_t *w, Queue_t *w_q){
  if (check_range(next)){
    return;
  }
  if (w->warrios[next.row][next.col] > 0) {
    q_push(w_q, next);
  }
  w->m_range[di][next.row][next.col] = 1;
  q_push(q, next);
}
void w_w_q_push(Queue_t *q, Position_t next, int di, Warriors_t *w){
  if (check_range(next)){
    return;
  }
  w->m_range[di][next.row][next.col] = 0;
  q_push(q, next);
}
void w_get_m_range(Medosa_t *m, Warriors_t *w){
  int w_max = 0;
  for (int di = 0; di < 4; ++di){
    Queue_t q = q_init();
    Queue_t w_q = q_init();
    q_push(&q, m->now);
    while (q.front < q.rear){
      Position_t now = q_front_pop(&q);
      Position_t next;
      if (w_m_direction(di, m->now, now) > 0){
        next = w_m_left(di, now);
        w_q_push(&q, next, di, w, &w_q);
      }
      else if (w_m_direction(di, m->now, now) < 0){
        next = w_m_right(di, now);
        w_q_push(&q, next, di, w, &w_q);
      }
      else {
        next = w_m_left(di, now);
        w_q_push(&q, next, di, w, &w_q);
        next = w_m_right(di, now);
        w_q_push(&q, next, di, w, &w_q);
        next = w_m_center(di, now);
        w_q_push(&q, next, di, w, &w_q);
      }
    }
    if (w_max >= w_q.rear) {
      continue;
    }
    w_max = w_q.rear;
    w->m_direction = di;
    while (w_q.front < w_q.rear){
      Position_t now = q_front_pop(&w_q);
      Position_t next;
      if (w_m_direction(di, m->now, now) > 0){
        next = w_m_left(di, now);
        w_w_q_push(&w_q, next, di, w);
      }
      else if (w_m_direction(di, m->now, now) < 0){
        next = w_m_right(di, now);
        w_w_q_push(&w_q, next, di, w);
      }
      next = w_m_center(di, now);
      w_w_q_push(&w_q, next, di, w);
    }
  }
}
void w_move (Warriors_t *w, Medosa_t *m, Answer_t *a) {
  int visited[MAX_N][MAX_N] = { 0 };
  for (int ri = 0; ri < n; ++ri) {
    for (int ci = 0; ci < n; ++ci) {
      int temp = w->warrios[ri][ci] - visited[ri][ci];
      if (temp == 0){
        continue;
      }
      if (m->now.row == ri && m->now.col == ci) {
        w->warrios[ri][ci] = 0;
        continue;
      }
      if (w->m_range[w->m_direction][ri][ci] == 1){
        a->stone += temp;
        continue;
      }
      w->warrios[ri][ci] -= temp;
      int nr = ri;
      int nc = ci;
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
        if ( i == 0 ) {
          if (nnr != -1 && w->m_range[w->m_direction][nnr][nc] != 1) {
            ++a->sum;
            if (m->now.row == nnr && m->now.col == nc){
              a->attack += temp;
              temp = 0;
            }
            nr = nnr;
          }
          else if (nnc != -1 && w->m_range[w->m_direction][nr][nnc] != 1) {
            ++a->sum;
            if (m->now.row == nr && m->now.col == nnc){
              a->attack += temp;
              temp = 0;
            }
            nc = nnc;
          }
        }
        else {
          if (nnc != -1 && w->m_range[w->m_direction][nr][nnc] != 1) {
            ++a->sum;
            if (m->now.row == nr && m->now.col == nnc){
              a->attack += temp;
              temp = 0;
            }
            nc = nnc;
          }
          else if (nnr != -1 && w->m_range[w->m_direction][nnr][nc] != 1) {
            ++a->sum;
            if (m->now.row == nnr && m->now.col == nc){
              a->attack += temp;
              temp = 0;
            }
            nr = nnr;
          }
        }
      }
      w->warrios[nr][nc] += temp;
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
    int de = 1;
    m_move(&m);
  }
  printf("0");
  return 0;
}
