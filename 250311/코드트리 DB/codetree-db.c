#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_NAME 20 /* 10 */
#define MAX_VALUE 1000000010 /* 1000000000 */
#define MAX_Q 300010 /* 300000 */
#define MAX_INIT 60 /* 50 */
#define MAX_INSERT 100010 /* 100000 */
/* 쿼리의 수 */
int Q;

/* 쿼리의 수를 입력 받는 함수 */
void inputQ(void);

/* 쿼리를 실행하는 함수 */
void runQuery(void);

/* 문자열 비교 */
int isSame(const char left[], const char right[]);

/* 문자열 복사 */
void copyStr(const int table_index, const char str[]);

/* 테이블 구조체 */
typedef struct {
  char name[MAX_NAME]; /* 이름: 10 */
  int value; /* 값: 1,000,000,000(int)*/
  int small; /* 몇번 째로 작은지 */
} Table;
/* 테이블: insert 최대 갯수 */
Table tables[MAX_INSERT];
/* 테이블이 비었는지 확인하는 배열 */
int isEmpty[MAX_INSERT];
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
void deleteName (void);
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
    } else if (isSame("insert", type_query)) {
      insertData();
    } else if (isSame("delete", type_query)) {
       deleteName();
    } else if (isSame("rank", type_query)) {
       rankK();
    } else {
       sumK();
    }
  }
}
/*----------------------------------------------*/
int isSame(const char left[], const char right[]) {
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
void copyStr(const int table_index, const char str[]) {
  int i = 0;
  while (str[i] != '\0') {
    tables[table_index].name[i] = 0;
    i++;
  }
  tables[table_index].name[i] = 0;
  i = 0;
  while (str[i] != '\0') {
    tables[table_index].name[i] = str[i];
    i++;
  }
}
/*----------------------------------------------*/
void initTable(void) {
  for (int i = 0; i < MAX_INSERT; i++) {
    isEmpty[i] = 0;
  }
}
/*----------------------------------------------*/
void insertData(void) {
  char name[MAX_NAME];
  int value;
  int small_number = 1;
  int first_empty = -1;
  scanf("%10s %d", name, &value);
  for (int i = 0; i < MAX_INSERT; i++) {
    if (isEmpty[i] == 0) {
      /* 저장할 인덱스 */
      if (first_empty == -1) {
        first_empty = i;
      } else {
        continue;
      }
    } else {
      Table now = tables[i];
      /* 같은 이름 확인 */
      if (isSame(now.name, name)) {
        printf("0\n");
        return;
      } else {
        /* 순서 수정 */
        if (now.value == value) {
          printf("0\n");
          return;
        }
      }
    }
  }
  for (int i = 0; i < MAX_INSERT; i++) {
    if (isEmpty[i] == 0) {
      continue;
    }
    if (tables[i].value < value) {
      small_number++;
    } else {
      tables[i].small++;
    }
  }
  isEmpty[first_empty] = 1;
  copyStr(first_empty, name);
  tables[first_empty].value = value;
  tables[first_empty].small = small_number;
  printf("1\n");
}
/*----------------------------------------------*/
void deleteName (void) {
  char name[MAX_NAME];
  scanf("%10s", name);
  int delete_index = -1;
  for (int i = 0; i < MAX_INSERT; i++) {
    if (isSame(tables[i].name, name)) {
      delete_index = i;
      break;
    }
  }
  if (delete_index == -1) {
    printf("0\n");
    return;
  } else {
    isEmpty[delete_index] = 0;
    for (int i = 0; i < MAX_INSERT; i++) {
      if (isEmpty[i] == 0) {
        continue;
      } else {
        int left = tables[i].value;
        int right = tables[delete_index].value;
        /* 순서 수정 */
        if (left > right) {
          tables[i].small--;
        }
      }
    }
  }
  printf("%d\n", tables[delete_index].value);
}
/*----------------------------------------------*/
void rankK(void) {
  int k;
  scanf("%d", &k);
  for (int i = 0; i < MAX_INSERT; i++) {
    if (isEmpty[i] == 0) {
      continue;
    }
    if (k == tables[i].small) {
      printf("%s\n", tables[i].name);
      return;
    }
  }
  printf("None\n");
}
/*----------------------------------------------*/
void sumK(void) {
  int k;
  scanf("%d", &k);
  long long sum = 0;
  for (int i = 0; i < MAX_INSERT; i++) {
    if (isEmpty[i] == 0) {
      continue;
    }
    if (k >= tables[i].value) {
      sum += tables[i].value;
    }
  }
  printf("%lld\n", sum);
}
/*----------------------------------------------*/
