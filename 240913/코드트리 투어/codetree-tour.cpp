#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
	int dest;
	int weight;
};

struct Product {
	int id;
	int revenue;
	int dest;
	int cost;

	bool operator<(const Product& other) const {
		int profit1 = revenue - cost;
		int profit2 = other.revenue - other.cost;
		if (profit1 == profit2) return id > other.id;
		return profit1 < profit2;
	}
};

// 전역 변수 선언
vector<vector<Edge>> graph;
vector<int> dist;  // 최단 경로 결과 저장용
int n, m;  // 도시 수, 간선 수

// 최단 경로를 계산하는 함수 (다익스트라 알고리즘)
void dijkstra(int start) {
	dist.assign(n, INT_MAX);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

	dist[start] = 0;
	pq.push({ 0, start });

	while (!pq.empty()) {
		int currentDist = pq.top().first;
		int currentNode = pq.top().second;
		pq.pop();

		if (currentDist > dist[currentNode]) continue;

		for (auto& edge : graph[currentNode]) {
			int nextNode = edge.dest;
			int weight = edge.weight;
			int newDist = currentDist + weight;

			if (newDist < dist[nextNode]) {
				dist[nextNode] = newDist;
				pq.push({ newDist, nextNode });
			}
		}
	}
}

// 여행 상품을 관리할 우선순위 큐
priority_queue<Product> products;
vector<bool> availableProducts(30001, false);  // 최대 30000개의 상품

// 여행 상품 추가 함수
void addProduct(int id, int revenue, int dest) {
	int cost = dist[dest];  // 전역 변수 dist에서 최단 거리 정보 가져오기

	products.push({ id, revenue, dest, cost });
	availableProducts[id] = true;
}

// 여행 상품 취소 함수
void cancelProduct(int id) {
	availableProducts[id] = false;
}

// 최적의 여행 상품 판매 함수
int sellBestProduct() {
	while (!products.empty()) {
		Product bestProduct = products.top();
		if (!availableProducts[bestProduct.id]) {
			products.pop();  // 판매 불가 상품 제거
			continue;
		}
		if (bestProduct.revenue - bestProduct.cost < 0) break;
		products.pop();
		availableProducts[bestProduct.id] = false;
		return bestProduct.id;
	}
	return -1;  // 판매할 수 있는 상품이 없는 경우
}

// 여행 상품 출발지 변경 함수
void changeStartingPoint(int newStart) {
	priority_queue<Product> newQueue;

	// 새로운 출발지로 최단 거리 갱신
	dijkstra(newStart);

	while (!products.empty()) {
		Product p = products.top();
		products.pop();

		int newCost = dist[p.dest];
		p.cost = newCost;
		newQueue.push({ p.id, p.revenue, p.dest, newCost });
	}

	products = newQueue;  // 새로운 큐로 대체
}

int main() {
	int Q;
	cin >> Q;

	for (int i = 0; i < Q; ++i) {
		int command;
		cin >> command;

		if (command == 100) {
			cin >> n >> m;
			graph.resize(n);
			for (int j = 0; j < m; ++j) {
				int v, u, w;
				cin >> v >> u >> w;
				graph[v].push_back({ u, w });
				graph[u].push_back({ v, w });
			}
			dijkstra(0);  // 출발지 0번에서 최초로 다익스트라 실행
		}
		else if (command == 200) {
			int id, revenue, dest;
			cin >> id >> revenue >> dest;
			addProduct(id, revenue, dest);
		}
		else if (command == 300) {
			int id;
			cin >> id;
			cancelProduct(id);
		}
		else if (command == 400) {
			cout << sellBestProduct() << endl;
		}
		else if (command == 500) {
			int newStart;
			cin >> newStart;
			changeStartingPoint(newStart);  // 출발지 변경 시 다익스트라 재실행
		}
	}

	return 0;
}