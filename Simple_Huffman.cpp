//哈夫曼树练习
#include <stdio.h>
#include <queue>
#include <iostream>
#include <iomanip>
#include<algorithm>
using namespace std;
float sum = 0;
int main() {
    int n;
    cin >> n;
    if (n == 0) {
        cout << 0;
    }
    if (n == 1) {
        cout << 1.00;
    }
    int m;
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int i = 0; i < n; i++) {
        cin >> m;
        minHeap.push(m);
    }
    while (minHeap.size() > 1) {
        int t1 = minHeap.top();
        minHeap.pop();
        int t2 = minHeap.top();
        minHeap.pop();
        int t3 = t1 + t2;
        sum += t3;
        minHeap.push(t3);
    }
    int total = minHeap.top();
    cout << fixed << setprecision(2) << sum / total;

}