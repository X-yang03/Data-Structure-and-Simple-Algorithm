//最小生成树
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

struct edge {
	int begin;
	int end;
	int weight;
	
};

bool my_cmp(edge a, edge b) {
	return a.weight < b.weight;
}

int main() {
	int n, m;
	cin >> n >> m;
	edge* E = new edge[m];
	int b, e, w;
	for (int i = 0; i < m; i++) {
		cin >> b >> e >> w;
		E[i].begin = b;
		E[i].end = e;
		E[i].weight = w;
	}
	sort(E, E + m,my_cmp);
	int ans = 0;
	int num = 0;
	int parent[51] = { 0 };
	for (int i = 0; i < n+1; i++)
		parent[i] = i;

	for (int i = 0; i < m; i++) {
		edge tmp = E[i];
		if (parent[tmp.begin] == parent[tmp.end])
			continue;
		else
		{
			int t1 = parent[tmp.begin];
			int t2 = parent[tmp.end];
			int p = max(t1,t2);

			for(int j=0;j<n+1;j++){
				if (parent[j] == t1 || parent[j] == t2) {
					parent[j] = p;
			}
			}
			ans += tmp.weight;
			num++;
		}
		if (num == n - 1)
		{
			break;
		}
	}
	cout << ans;
}