#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_NAME 20 /* 10 */
#define MAX_VALUE 1000000010 /* 1000000000 */
#define MAX_Q 300010 /* 300000 */
#define MAX_INIT 60 /* 50 */
#define MAX_INSERT 100010 /* 100000 */
/* 쿼리*/
int Q;
void inputQ(void);
void runQuery(void);

/* 문자열*/
int isSame(const char *left, const char *right);
void copyStr(char *left, const char *right, const int left_size);

/* 세그먼트 트리 */
int segtree[4 * MAX_INSERT];
void buildSegmentTree(int node, int start, int end);
int querySum(int node, int start, int end, int left, int right);
int queryRank(int node, int start, int end, int k);
void updateSegmentTree(int node, int start, int end, int idx, int value);

/* 테이블 */
typedef struct {
	char name[MAX_NAME]; /* 이름: 10 */
	int value; /* 값: 1,000,000,000(int)*/
} Table;
Table tables[MAX_INSERT];
int table_size;
/*
 * 1. init (init)
  * 모든 데이터 삭제: isEmpty = 0
   * 최대 50
	*/
void initTable(void);
/*
 * 2. insert (insert name value)
  * 데이터 추가: isEmpty가 0인 table에 추가
   * 시간 복잡도: O(N)
	* 성공 1: 새로운 name, 새로운 value 조합
	 * 실패 0: 둘 중 하나라도 이미 존재하는 name, value
	  * 최대 100,000
	   */
void insertData(void);
/*
 * 3. delete (delete name)
  * name과 같은 값 찾기: index 찾기
   * 시간 복잡도: O(N)
	* row 삭제: 찾은 index 값 empty처리
	 * 시간 복잡도: O(1)
	  * 성공 value: name이 존재할 때
	   * 실패 0: name이 존재하지 않을 때
		* 최대 100,000
		 */
void deleteName(void);
/*
 * 4. rank (rank k)
  * k번째로 작은 value 찾기: small == k 인덱스 찾기
   * 시간 복잡도: O(N)
	* 찾은 value의 name 찾기: 찾은 index
	 * 시간 복잡도: O(1)
	  * 성공 name: row의 수가 k보다 클 때
	   * 실패 "None": row의 수가 k보다 작을 때
		* 최대 100,000
		 */
void rankK(void);
/*
 * 5. sum (sum K)
  * k 이하의 value를 찾으며 더하기: 모든 index 탐색
   * 시간 복잡도: O(N)
	* 성공 sum: 해당되는 row가 있을 때
	 * 실패 0: 해당되는 row가 없을 때
	  * 최대 100,000
	   */
void sumK(void);

int main(void) {

	inputQ();
	runQuery();

	return 0;
}
/*----------------------------------------------*/
void inputQ(void) {
	scanf("%d", &Q);
}
/*----------------------------------------------*/
void runQuery(void) {
	for (int qi = 0; qi < Q; qi++) {
		char type_query[11];
		scanf("%10s", type_query);
		if (isSame("init", type_query)) {
			initTable();
		}
		else if (isSame("insert", type_query)) {
			insertData();
		}
		else if (isSame("delete", type_query)) {
			deleteName();
		}
		else if (isSame("rank", type_query)) {
			rankK();
		}
		else {
			sumK();
		}
	}
}
/*----------------------------------------------*/
int isSame(const char *left, const char *right) {
	int si = 0;

	while (left[si] != '\0' && right[si] != '\0') {
		if (left[si] != right[si]) {
			return 0;  /* 문자열이 다르면 0 반환 */
		}
		si++;
	}

	/* 두 문자열의 길이가 동일하면 1 반환, 다르면 0 반환 */
	return (left[si] == '\0' && right[si] == '\0') ? 1 : 0;
}
/*----------------------------------------------*/
void copyStr(char *left, const char *right, const int left_size) {
	int i = 0;

	// right를 left에 복사 (최대 left_size - 1 만큼만 복사하여 널 종료 보장)
	while (i < left_size - 1 && right[i] != '\0') {
		left[i] = right[i];
		i++;
	}

	// 널 문자 추가하여 문자열 종료 보장
	left[i] = '\0';

	// 기존 `left`의 남아 있는 부분을 `\0`으로 채움
	while (i < left_size - 1) {
		left[i++] = '\0';
	}
}
/*----------------------------------------------*/
void buildSegmentTree(const int node, const int start, const int end) {
	if (start == end) {
		segtree[node] = tables[start].value;
	}
	else {
		int mid = (start + end) / 2;
		buildSegmentTree(2 * node + 1, start, mid);
		buildSegmentTree(2 * node + 2, mid + 1, end);
		segtree[node] = segtree[2 * node + 1] + segtree[2 * node + 2];
	}
}
/*----------------------------------------------*/
int querySum(int node, int start, int end, int left, int right) {
	if (right < tables[start].value || tables[end].value < left) {
		return 0;
	}
	else if (left <= tables[start].value && tables[end].value <= right) {
		return segtree[node];
	}
	else {
		int mid = (start + end) / 2;
		return querySum(2 * node + 1, start, mid, left, right) + querySum(2 * node + 2, mid + 1, end, left, right);
	}
}
/*----------------------------------------------*/
int queryRank(int node, int start, int end, int k) {
	if (start == end) return start;

	int mid = (start + end) / 2;
	int left_count = mid - start + 1;

	if (left_count >= k) {
		return queryRank(2 * node + 1, start, mid, k);
	}
	else {
		return queryRank(2 * node + 2, mid + 1, end, k - left_count);
	}
}
/*----------------------------------------------*/
void initTable(void) {
	table_size = 0;
}
/*----------------------------------------------*/
void insertData(void) {
	char name[MAX_NAME];
	int value;
	scanf("%10s %d", name, &value);
	int insert_index = 0;
	for (int i = 0; i < table_size; i++) {
		if (
			isSame(tables[i].name, name) ||
			tables[i].value == value
			) {
			printf("0\n");
			return;
		}
	}
	int i = table_size - 1;
	while (i >= 0 && tables[i].value > value) {
		tables[i + 1] = tables[i];
		i--;
	}
	Table *nowTable = &tables[i + 1];
	copyStr(nowTable->name, name, sizeof(nowTable->name));
	nowTable->value = value;
	table_size++;

	if (table_size != 0) {
		buildSegmentTree(0, 0, table_size - 1);
	}

	printf("1\n");
}
/*----------------------------------------------*/
void deleteName(void) {
	char name[MAX_NAME];
	scanf("%10s", name);

	int index = -1;
	for (int i = 0; i < table_size; i++) {
		if (isSame(tables[i].name, name)) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("0\n");
		return;
	}

	int deleted_value = tables[index].value;

	// 데이터 삭제 (배열 유지)
	for (int i = index; i < table_size - 1; i++) {
		tables[i] = tables[i + 1];
	}
	table_size--;

	// 세그먼트 트리 업데이트
	if (table_size != 0) {
		buildSegmentTree(0, 0, table_size - 1);
	}

	printf("%d\n", deleted_value);
}
/*----------------------------------------------*/
void rankK(void) {
	int k;
	scanf("%d", &k);

	if (k > table_size) {
		printf("None\n");
		return;
	}

	int index = queryRank(0, 0, table_size - 1, k);
	printf("%s\n", tables[index].name);
}
/*----------------------------------------------*/
void sumK(void) {
	int k;
	scanf("%d", &k);
	if (table_size == 0) {
		printf("0\n");
		return;
	}
	int sum = querySum(0, 0, table_size - 1, 0, k);
	printf("%d\n", sum);
}
/*----------------------------------------------*/




