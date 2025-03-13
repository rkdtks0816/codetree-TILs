#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
long long querySum(int node, int start, int end, int left, int right);
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
long long querySum(int node, int start, int end, int left, int right) {
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
typedef struct NameNode {
	char name[MAX_NAME];
	int index;
	struct NameNode *left, *right;
} NameNode;
typedef struct ValueNode {
	int value;
	struct ValueNode *left, *right;
} ValueNode;

/* 전역 변수 */
NameNode* nameRoot = NULL;    // name → index 저장하는 BST
ValueNode* valueRoot = NULL;  // value 저장하는 BST
/*----------------------------------------------*/
/* BST 초기화 함수 */
void freeNameTree(NameNode* root) {
	if (!root) return;
	freeNameTree(root->left);
	freeNameTree(root->right);
	free(root);
}

void freeValueTree(ValueNode* root) {
	if (!root) return;
	freeValueTree(root->left);
	freeValueTree(root->right);
	free(root);
}

/* 전체 초기화 */
void initTable(void) {
	table_size = 0;

	/* BST 초기화 (메모리 해제 후 NULL 설정) */
	freeNameTree(nameRoot);
	freeValueTree(valueRoot);
	nameRoot = NULL;
	valueRoot = NULL;
}
/*----------------------------------------------*/
/* 새로운 노드 생성 */
NameNode* createNameNode(const char *name, int index) {
	NameNode* newNode = (NameNode*)malloc(sizeof(NameNode));
	int i;
	for (i = 0; name[i] != '\0' && i < MAX_NAME - 1; i++) {
		newNode->name[i] = name[i];
	}
	newNode->name[i] = '\0';
	newNode->index = index;
	newNode->left = newNode->right = NULL;
	return newNode;
}

/* `name`을 BST에 삽입 */
NameNode* insertName(NameNode* root, const char *name, int index) {
	if (!root) return createNameNode(name, index);

	int cmp = strcmp(name, root->name);
	if (cmp < 0) root->left = insertName(root->left, name, index);
	else if (cmp > 0) root->right = insertName(root->right, name, index);

	return root;
}

/* `name`을 BST에서 찾기 */
int findName(NameNode* root, const char *name) {
	if (!root) return -1;

	int cmp = strcmp(name, root->name);
	if (cmp == 0) return root->index;
	else if (cmp < 0) return findName(root->left, name);
	else return findName(root->right, name);
}

/* 새로운 노드 생성 */
ValueNode* createValueNode(int value) {
	ValueNode* newNode = (ValueNode*)malloc(sizeof(ValueNode));
	newNode->value = value;
	newNode->left = newNode->right = NULL;
	return newNode;
}

/* `value`를 BST에 삽입 */
ValueNode* insertValue(ValueNode* root, int value) {
	if (!root) return createValueNode(value);

	if (value < root->value) root->left = insertValue(root->left, value);
	else if (value > root->value) root->right = insertValue(root->right, value);

	return root;
}

/* `value`가 존재하는지 확인 */
int findValue(ValueNode* root, int value) {
	if (!root) return 0;

	if (value == root->value) return 1;
	else if (value < root->value) return findValue(root->left, value);
	else return findValue(root->right, value);
}

void insertData(void) {
	char name[MAX_NAME];
	int value;
	scanf("%10s %d", name, &value);

	/* `name`과 `value`가 이미 존재하는지 검사 (O(log N)) */
	if (findName(nameRoot, name) != -1 || findValue(valueRoot, value)) {
		printf("0\n");
		return;
	}

	/* 이진 탐색으로 `value`의 삽입 위치 찾기 */
	int left = 0, right = table_size - 1, mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (tables[mid].value < value) left = mid + 1;
		else right = mid - 1;
	}

	/* 데이터 이동 (O(N)) */
	for (int i = table_size; i > left; i--) {
		tables[i] = tables[i - 1];
	}

	/* `name`과 `value` 삽입 */
	int i;
	for (i = 0; name[i] != '\0' && i < MAX_NAME - 1; i++) {
		tables[left].name[i] = name[i];
	}
	tables[left].name[i] = '\0';
	tables[left].value = value;
	table_size++;

	/* BST에 추가 */
	nameRoot = insertName(nameRoot, name, table_size - 1);
	valueRoot = insertValue(valueRoot, value);

	/* 세그먼트 트리 업데이트 */
	buildSegmentTree(0, 0, table_size - 1);
	printf("1\n");
}
/*----------------------------------------------*/
/* `name`을 BST에서 삭제하는 함수 */
NameNode* deleteNameNode(NameNode* root, const char *name) {
	if (!root) return NULL;

	int cmp = strcmp(name, root->name);
	if (cmp < 0) {
		root->left = deleteNameNode(root->left, name);
	}
	else if (cmp > 0) {
		root->right = deleteNameNode(root->right, name);
	}
	else {  // 삭제할 노드를 찾음
		if (!root->left) {
			NameNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (!root->right) {
			NameNode* temp = root->left;
			free(root);
			return temp;
		}

		// 후속 노드 찾기 (오른쪽 서브트리에서 가장 작은 값)
		NameNode* temp = root->right;
		while (temp->left) temp = temp->left;

		// 값 복사 후 기존 노드 삭제
		strcpy(root->name, temp->name);
		root->index = temp->index;
		root->right = deleteNameNode(root->right, temp->name);
	}
	return root;
}
/* `value`를 BST에서 삭제하는 함수 */
ValueNode* deleteValueNode(ValueNode* root, int value) {
	if (!root) return NULL;

	if (value < root->value) {
		root->left = deleteValueNode(root->left, value);
	}
	else if (value > root->value) {
		root->right = deleteValueNode(root->right, value);
	}
	else {  // 삭제할 노드를 찾음
		if (!root->left) {
			ValueNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (!root->right) {
			ValueNode* temp = root->left;
			free(root);
			return temp;
		}

		// 후속 노드 찾기 (오른쪽 서브트리에서 가장 작은 값)
		ValueNode* temp = root->right;
		while (temp->left) temp = temp->left;

		// 값 복사 후 기존 노드 삭제
		root->value = temp->value;
		root->right = deleteValueNode(root->right, temp->value);
	}
	return root;
}
void deleteName(void) {
	char name[MAX_NAME];
	scanf("%10s", name);

	/* BST를 이용한 `name` 검색 (O(log N)) */
	int index = findName(nameRoot, name);
	if (index == -1) {
		printf("0\n");
		return;
	}

	int deleted_value = tables[index].value;

	/* BST에서 `name` 삭제 */
	nameRoot = deleteNameNode(nameRoot, name);
	valueRoot = deleteValueNode(valueRoot, deleted_value);

	/* 배열에서 삭제된 요소를 정리 (O(N)) */
	for (int i = index; i < table_size - 1; i++) {
		tables[i] = tables[i + 1];
	}
	table_size--;

	/* 세그먼트 트리 업데이트 */
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
	long long sum = querySum(0, 0, table_size - 1, 0, k);
	printf("%lld\n", sum);
}
/*----------------------------------------------*/




