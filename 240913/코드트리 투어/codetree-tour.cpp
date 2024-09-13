#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using namespace std;

// 간선 정보를 저장할 구조체
struct Edge {
    int dest;
    int weight;
};

// 여행 상품 구조체
struct Product {
    int id;
    int revenue;
    int dest;
    int cost;

    // 우선순위 큐에서 이득(revenue - cost)이 큰 순, id가 작은 순으로 정렬
    bool operator<(const Product& other) const {
        int profit1 = revenue - cost;
        int profit2 = other.revenue - other.cost;
        if (profit1 == profit2) return id > other.id;
        return profit1 < profit2;
    }
};

// 그래프와 도시 정보
vector<vector<Edge>> graph;
int n, m; // 도시 수, 간선 수

// 최단 경로를 계산하는 함수 (다익스트라 알고리즘)
vector<int> dijkstra(int start) {
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

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
                pq.push({newDist, nextNode});
            }
        }
    }

    return dist;
}

// 여행 상품을 관리할 우선순위 큐
priority_queue<Product> products;
vector<Product> allProducts; // 전체 상품 저장
vector<bool> availableProducts(30001, false); // 최대 30000개의 상품

// 여행 상품 추가 함수
void addProduct(int id, int revenue, int dest) {
    vector<int> dist = dijkstra(0); // 출발지는 0번 도시
    int cost = dist[dest];

    // 상품을 목록에 추가 (팔 수 없더라도 추가)
    allProducts.push_back({id, revenue, dest, cost});
    products.push({id, revenue, dest, cost}); // 팔 수 없는 상품도 우선순위 큐에 추가
    availableProducts[id] = (cost != INT_MAX && cost <= revenue);
}

// 여행 상품 취소 함수
void cancelProduct(int id) {
    availableProducts[id] = false;
}

// 최적의 여행 상품 판매 함수
int sellBestProduct() {
    while (!products.empty()) {
        Product bestProduct = products.top();
        if (availableProducts[bestProduct.id]) {
            products.pop();
            availableProducts[bestProduct.id] = false;
            return bestProduct.id;
        }
        products.pop(); // 판매 불가 상품 제거
    }
    return -1; // 판매할 수 있는 상품이 없는 경우
}

// 여행 상품 출발지 변경 함수
void changeStartingPoint(int newStart) {
    priority_queue<Product> newQueue;

    // 새로운 출발지로 최단 거리 갱신
    vector<int> dist = dijkstra(newStart);

    for (auto& p : allProducts) {
        int newCost = dist[p.dest];
        p.cost = newCost;
        newQueue.push({p.id, p.revenue, p.dest, newCost}); // 팔 수 없어도 추가
        availableProducts[p.id] = (newCost != INT_MAX && newCost <= p.revenue); // 팔 수 있는지 여부 갱신
    }

    products = newQueue; // 새로운 큐로 대체
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
                graph[v].push_back({u, w});
                graph[u].push_back({v, w});
            }
        } else if (command == 200) {
            int id, revenue, dest;
            cin >> id >> revenue >> dest;
            addProduct(id, revenue, dest);
        } else if (command == 300) {
            int id;
            cin >> id;
            cancelProduct(id);
        } else if (command == 400) {
            cout << sellBestProduct() << endl;
        } else if (command == 500) {
            int newStart;
            cin >> newStart;
            changeStartingPoint(newStart);
        }
    }

    return 0;
}