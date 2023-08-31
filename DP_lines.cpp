//根据动态规划，找到最佳的多段最小二乘法的拟合曲线
#include<iostream>
#include<iomanip>
#include<climits>
using namespace std;

int p[1001][2];
double e[1001][1001];
double OPT[1001];
int n;
int c;
double cal_square(int b, int e) {
	if (e - b == 1||e -b ==0)return 0;
	double sum_x=0, sum_y=0, sum_square=0, sum_xy=0;
	for (int i = b; i <= e; i++) {
		sum_x += p[i][0];
		sum_y += p[i][1];
		sum_square += p[i][0]* p[i][0];
		sum_xy += p[i][1] * p[i][0];
	}
	double A = ((e-b+1) * sum_xy - sum_x * sum_y) / ((e-b+1) * sum_square - sum_x * sum_x);
	double B = (sum_y - A * sum_x) / (e-b+1);
	double ans=0;
	for (int i = b; i <= e; i++) {
		ans += (p[i][1] - A * p[i][0] - B) * (p[i][1] - A * p[i][0] - B);
	}
	return ans;
}

void segmented_Least_Squares() {
	OPT[0] = 0; //
	//OPT[1] = c;
	//OPT[2] = c;  //两个点，一条线
	for (int i = 1; i <= n; i++) {
		double min = INT_MAX;
		for (int j = 1; j < i; j++) {
			double tmp = e[j][i] + c + OPT[j-1];
			if (tmp < min)min = tmp;
		}
		OPT[i] = min;
	}
}

int main() {
	cin >> n;
	cin >> c;
	for (int i = 1; i <= n; i++) {
		cin >> p[i][0] >> p[i][1];
	}
	for (int i = 1; i <=n ; i++) {
		for (int j = i+1; j <= n; j++) {
			e[i][j] = cal_square(i, j);
		}
	}
	segmented_Least_Squares();
	cout <<fixed<<setprecision(2)<< OPT[n]<<endl;
}