#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
/* --- 매크로 --- */
#define MAX_N 60
/* --- 구조체 --- */
typedef struct {
	int row;
	int col;
	int value;
} Position_t;
typedef struct {
	Position_t q_values[MAX_N * MAX_N];
	int front;
	int rear;
} Queue_t;
typedef struct {
	Position_t now;
	Position_t end;
	int path[MAX_N][MAX_N];
	int sight[4][MAX_N][MAX_N];
	int sight_d;
} Medosa_t;
typedef struct {
	int warriors[MAX_N][MAX_N];
} Warrior_t;
typedef struct {
	int move;
	int stone;
	int attack;
} Anwser_t;
/* --- 전역 변수 --- */
int n, m;
Medosa_t me;
Warrior_t wa;
int grid[MAX_N][MAX_N];
Anwser_t a;
int dr[] = { -1, 1, 0, 0 };
int dc[] = { 0, 0, -1, 1 };
/* --- COMMON --- */
void c_arr_init(int arr[][MAX_N], int value) {
	for (int ri = 0; ri < n; ++ri) {
		for (int ci = 0; ci < n; ++ci) {
			arr[ri][ci] = value;
		}
	}
}
void a_init(void) {
	a.move = 0;
	a.attack = 0;
	a.stone = 0;
}
/* --- input --- */
void i_nm(void) {
	scanf("%d %d", &n, &m);
}
void i_medosa(void) {
	scanf("%d %d %d %d", &me.now.row, &me.now.col, &me.end.row, &me.end.col);
}
void i_warriors(void) {
	int row, col;
	for (int mi = 0; mi < m; ++mi) {
		scanf("%d %d", &row, &col);
		++wa.warriors[row][col];
	}
}
void i_grid(void) {
	for (int ri = 0; ri < n; ++ri) {
		for (int ci = 0; ci < n; ++ci) {
			scanf("%d", &grid[ri][ci]);
		}
	}
}
/* --- CHECK --- */
int in_range(Position_t position) {
	return (
		position.row >= 0 &&
		position.col >= 0 &&
		position.row < n &&
		position.col < n
		);
}
int same_position(Position_t left, Position_t right) {
	return left.row == right.row && left.col == right.col;
}
/* --- Queue --- */
Queue_t q_init(void) {
	Queue_t q;
	q.front = 0;
	q.rear = 0;
	return	q;
}
void q_push(Queue_t *q, Position_t new_q) {
	Position_t *new_q_value = &q->q_values[q->rear];
	new_q_value->row = new_q.row;
	new_q_value->col = new_q.col;
	new_q_value->value = new_q.value;
	++q->rear;
}
Position_t q_front_pop(Queue_t *q) {
	Position_t q_front = q->q_values[q->front];
	++q->front;
	return q_front;
}
/* --- 메두사 --- */
void m_init(void) {
	c_arr_init(me.path, MAX_N * MAX_N);
	for (int di = 0; di < 4; ++di) {
		c_arr_init(me.sight[di], 0);
	}
}
void m_path(void) {
	Queue_t m_q = q_init();
	q_push(&m_q, me.end);
	me.path[me.end.row][me.end.col] = 0;
	while (m_q.front < m_q.rear) {
		Position_t now = q_front_pop(&m_q);
		for (int di = 0; di < 4; ++di) {
			Position_t next;
			next.row = now.row + dr[di];
			next.col = now.col + dc[di];
			int nd = me.path[now.row][now.col] + 1;
			if (!in_range(next)) {
				continue;
			}
			if (grid[next.row][next.col] == 1) {
				continue;
			}
			if (me.path[next.row][next.col] <= nd) {
				continue;
			}
			me.path[next.row][next.col] = nd;
			q_push(&m_q, next);
		}
	}
}
void m_move(void) {
	for (int di = 0; di < 4; ++di) {
		Position_t next;
		next.row = me.now.row + dr[di];
		next.col = me.now.col + dc[di];
		if (!in_range(next)) {
			continue;
		}
		int now_d = me.path[me.now.row][me.now.col];
		int next_d = me.path[next.row][next.col];
		if (next_d >= now_d) {
			continue;
		}
		me.now.row = next.row;
		me.now.col = next.col;
		return;
	}
}
int m_sight_up(void) {
	Position_t center = me.now;
	int w_count = 0;
	int level = 1;
	int defence[MAX_N][MAX_N];
	c_arr_init(defence, 0);
	Position_t next;
	for (next.row = center.row - 1; next.row >= 0; --next.row) {
		for (next.col = center.col - level; next.col <= center.col + level; ++next.col) {
			if (!in_range(next)) {
				continue;
			}
			if (defence[next.row][next.col] == 1) {
				continue;
			}
			me.sight[0][next.row][next.col] = 1;
			if (wa.warriors[next.row][next.col] > 0) {
				w_count += wa.warriors[next.row][next.col];
				Position_t w_next;
				int w_level = 1;
				for (w_next.row = next.row - 1; w_next.row >= 0; --w_next.row) {
					if (next.col < center.col) {
						for (w_next.col = next.col - w_level; w_next.col <= next.col; ++w_next.col) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else if (next.col > center.col) {
						for (w_next.col = next.col + w_level; w_next.col >= next.col; --w_next.col) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else {
						defence[w_next.row][next.col] = 1;
					}
					++w_level;
				}
			}
		}
		++level;
	}
	return w_count;
}
int m_sight_down(void) {
	Position_t center = me.now;
	int w_count = 0;
	int level = 1;
	int defence[MAX_N][MAX_N];
	c_arr_init(defence, 0);
	Position_t next;
	for (next.row = center.row + 1; next.row < n; ++next.row) {
		for (next.col = center.col - level; next.col <= center.col + level; ++next.col) {
			if (!in_range(next)) {
				continue;
			}
			if (defence[next.row][next.col] == 1) {
				continue;
			}
			me.sight[1][next.row][next.col] = 1;
			if (wa.warriors[next.row][next.col] > 0) {
				w_count += wa.warriors[next.row][next.col];
				Position_t w_next;
				int w_level = 1;
				for (w_next.row = next.row + 1; w_next.row < n; ++w_next.row) {
					if (next.col < center.col) {
						for (w_next.col = next.col - w_level; w_next.col <= next.col; ++w_next.col) {
							if(!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else if (next.col > center.col) {
						for (w_next.col = next.col + w_level; w_next.col >= next.col; --w_next.col) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else {
						defence[w_next.row][next.col] = 1;
					}
					++w_level;
				}
			}
		}
		++level;
	}
	return w_count;
}
int m_sight_left(void) {
	Position_t center = me.now;
	int w_count = 0;
	int level = 1;
	int defence[MAX_N][MAX_N];
	c_arr_init(defence, 0);
	Position_t next;
	for (next.col = center.col - 1; next.col >= 0; --next.col) {
		for (next.row = center.row - level; next.row <= center.row + level; ++next.row) {
			if (!in_range(next)) {
				continue;
			}
			if (defence[next.row][next.col] == 1) {
				continue;
			}
			me.sight[2][next.row][next.col] = 1;
			if (wa.warriors[next.row][next.col] > 0) {
				w_count += wa.warriors[next.row][next.col];
				Position_t w_next;
				int w_level = 1;
				for (w_next.col = next.col - 1; w_next.col >= 0; --w_next.col) {
					if (next.row < center.row) {
						for (w_next.row = next.row - w_level; w_next.row <= next.row; ++w_next.row) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else if (next.row > center.row) {
						for (w_next.row = next.row + w_level; w_next.row >= next.row; --w_next.row) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else {
						defence[next.row][w_next.col] = 1;
					}
					++w_level;
				}
			}
		}
		++level;
	}
	return w_count;
}
int m_sight_right(void) {
	Position_t center = me.now;
	int w_count = 0;
	int level = 1;
	int defence[MAX_N][MAX_N];
	c_arr_init(defence, 0);
	Position_t next;
	for (next.col = center.col + 1; next.col < n; ++next.col) {
		for (next.row = center.row - level; next.row <= center.row + level; ++next.row) {
			if (!in_range(next)) {
				continue;
			}
			if (defence[next.row][next.col] == 1) {
				continue;
			}
			me.sight[3][next.row][next.col] = 1;
			if (wa.warriors[next.row][next.col] > 0) {
				w_count += wa.warriors[next.row][next.col];
				Position_t w_next;
				int w_level = 1;
				for (w_next.col = next.col + 1; w_next.col < n; ++w_next.col) {
					if (next.row < center.row) {
						for (w_next.row = next.row - w_level; w_next.row <= next.row; ++w_next.row) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else if (next.row > center.row) {
						for (w_next.row = next.row + w_level; w_next.row >= next.row; --w_next.row) {
							if (!in_range(w_next)) {
								continue;
							}
							defence[w_next.row][w_next.col] = 1;
						}
					}
					else {
						defence[next.row][w_next.col] = 1;
					}
					++w_level;
				}
			}
		}
		++level;
	}
	return w_count;
}
void m_sight(void) {
	int w_max = -1;
	int w_up = m_sight_up();
	int w_down = m_sight_down();
	int w_left = m_sight_left();
	int w_right = m_sight_right();
	if (w_max < w_up) {
		w_max = w_up;
		me.sight_d = 0;
	}
	if (w_max < w_down) {
		w_max = w_down;
		me.sight_d = 1;
	}
	if (w_max < w_left) {
		w_max = w_left;
		me.sight_d = 2;
	}
	if (w_max < w_right) {
		w_max = w_right;
		me.sight_d = 3;
	}
}
/* --- 전사 --- */
void w_init(void) {
	c_arr_init(wa.warriors, 0);
}
void w_move(void) {
	int new_ws[MAX_N][MAX_N];
	c_arr_init(new_ws, 0);
	Position_t now;
	for (now.row = 0; now.row < n; ++now.row) {
		for (now.col = 0; now.col < n; ++now.col) {
			if (wa.warriors[now.row][now.col] == 0) {
				continue;
			}
			if (me.sight[me.sight_d][now.row][now.col] == 1) {
				a.stone += wa.warriors[now.row][now.col];
				new_ws[now.row][now.col] += wa.warriors[now.row][now.col];
				continue;
			}
			if (same_position(now, me.now)) {
				continue;
			}
			Position_t first;
			first.row = now.row;
			first.col = now.col;
			if (now.row > me.now.row) {
				first.row = now.row - 1;
			}
			else if (now.row < me.now.row) {
				first.row = now.row + 1;
			}
			if (me.sight[me.sight_d][first.row][first.col] == 1 || now.row == me.now.row) {
				first.row = now.row;
				if (now.col > me.now.col) {
					first.col = now.col - 1;
				}
				else if (now.col < me.now.col) {
					first.col = now.col + 1;
				}
				if (me.sight[me.sight_d][first.row][first.col] == 1) {
					first.col = now.col;
				}
			}
			if (!same_position(now, first)) {
				a.move += wa.warriors[now.row][now.col];
			}
			Position_t second;
			second.row = first.row;
			second.col = first.col;
			if (first.col > me.now.col) {
				second.col = first.col - 1;
			}
			else if (first.col < me.now.col) {
				second.col = first.col + 1;
			}
			if (me.sight[me.sight_d][second.row][second.col] == 1 || first.col == me.now.col) {
				second.col = first.col;
				if (first.row > me.now.row) {
					second.row = first.row - 1;
				}
				else if (first.row < me.now.row) {
					second.row = first.row + 1;
				}
				if (me.sight[me.sight_d][second.row][second.col] == 1) {
					second.row = first.row;
				}
			}
			if (!same_position(second, first)) {
				a.move += wa.warriors[now.row][now.col];
			}
			if (same_position(second, me.now)) {
				a.attack += wa.warriors[now.row][now.col];
			}
			else {
				new_ws[second.row][second.col] += wa.warriors[now.row][now.col];
			}
		}
	}
	for (int ri = 0; ri < n; ++ri) {
		for (int ci = 0; ci < n; ++ci) {
			wa.warriors[ri][ci] = new_ws[ri][ci];
		}
	}
}
int main(void) {
	i_nm();
	m_init();
	i_medosa();
	w_init();
	i_warriors();
	i_grid();
	m_path();
	if (me.path[me.now.row][me.now.col] == MAX_N * MAX_N) {
		printf("-1");
		return 0;
	}

	m_move();
	while (!same_position(me.now, me.end)) {
		a_init();
		for (int di = 0; di < 4; ++di) {
			c_arr_init(me.sight[di], 0);
		}
		m_sight();
		w_move();
		printf("%d %d %d\n", a.move, a.stone, a.attack);
		m_move();
	}
	printf("0");

	return 0;
}

/* 체크리스트
 ✅ 0 ~ N - 1 범위의 N x N
 ✅ 도로 0 아니면 1
 ✅ 입력은 좌표 그대로
 --- 메두사 ---
 ✅ end에서 start로 BFS
 ✅ 집과 공원의 도로위이나 좌표는 다름
 ✅ 한 칸 이동
 ✅ 이동 칸 전사는 사라짐 어택 아님
 ✅ 상 하 좌 우 우선순위
 ✅ 공원까지 경로가 없다면 -1
 ✅ 상 하 좌 우 시선
 ✅ 90도 시야각 범위 안의 전사는 돌
 ✅ 다른 전사에 가려진 경우 돌 안됨
 ✅ 8방향 기준으로 전사 뒤 가려짐
 ✅ 같은 칸의 여러 전사는 같이 돌
 ✅ 전사를 가장 많이 볼 수 있는 방향
 ✅ 가려진건 봤다고 할 수 없음
 --- 전사 ---
 - 도로 비도로 구분없음
 ✅ 메두사 집에 초기에 있지 않음
 ✅ 돌로 변했다면 이동 못함
 ✅ 한 칸 이동
 ✅ 거리를 줄일 수 있는 방향
 ✅ 메두사의 시야에 들어오는 곳은 이동 못함
 ✅ 첫 번째 이동은 상 하 좌 우 우선순위
 ✅ 두 번째 이동은 좌 우 상 하 우선순위
 ✅ 메두사를 만나면 공격 +1 후 사라짐
 -- 출력 --
 ✅ 전사가 이동한 거리의 합
 ✅ 돌이 된 전사의 수
 ✅ 메두사를 공격한 전사의 수
 ✅ 메두사 도착 0
 ✅ 공원으로 못간다면 -1
 */

/*
6 4
3 1 1 2
3 5 1 4 0 4 1 3
0 0 0 0 1 0
0 1 0 0 1 1
1 1 0 0 0 0
0 0 1 0 1 1
0 0 0 0 0 0
0 0 0 0 1 1

0 0 0 0 # 0
0 1 0 # # 1
1 1 0 0 0 0
0 0 1 0 1 #
0 & 0 0 0 0
0 0 0 0 1 1

0 0 0 0 0 1 0
0 0 0 0 0 0 1
0 0 0 0 # 1 1
0 0 0 2 0 1 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
*/