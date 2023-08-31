//���β����е����ڽ��������
#include<iostream>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<climits>
#include<math.h>
using namespace std;

class Point {   //�����
public:
	double x;
	double y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(double a, double b) {
		x = a;
		y = b;
	}
	bool operator<=(Point& a) {
		if (x != a.x)
			return x < a.x;
		else
			return y <= a.y;
	}
};

class point_pair {   //���
public:
	Point a;
	Point b;
	double dis;
	point_pair() {

	}
	point_pair(Point x, Point y,double dis = 0) {
		a = x;
		b = y;
		this->dis = dis;
	}
};

bool cmpx(Point a, Point b) {
	return a.x < b.x;
}

bool cmpy(Point a, Point b) {
	return a.y < b.y;
}

double dis(Point a, Point b) {
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return (dx * dx + dy * dy);    //ֻҪƽ�������ÿ���
}

point_pair Closest_Pair_Rec(vector<Point> Px, vector<Point> Py) {
	if (Px.size() <= 3) {              //���ֻ�������㣬ֱ������С
		double min = INT_MAX;
		point_pair ans;
		for (auto it = Px.begin(); it != Px.end(); it++) {
			for (auto itt = it+1; itt != Px.end(); itt++) {
				if (dis(*it, *itt) < min) {
					min = dis(*it, *itt);
					ans = point_pair(*it, *itt, min);
				}
			}
		}
		return ans;
	}
	Point mid = Px[Px.size()/2-1];     //���ֵ�
	vector<Point> Qx, Qy, Rx, Ry, Sy;
	Qx.assign(Px.begin(), Px.begin() + Px.size() / 2);    
	Rx.assign(Px.begin() + Px.size() / 2, Px.end());        //������λԪ�ػ���Qx��Rx
	for (auto it = Py.begin(); it != Py.end(); it++) {      //����Py����Qy��Ry�����Ӷ�O��n��
		if (*it <= mid )
			Qy.push_back(*it);
		else
			Ry.push_back(*it);
	}
	point_pair q = Closest_Pair_Rec(Qx, Qy);    //�ݹ����
	point_pair r = Closest_Pair_Rec(Rx, Ry);

	double delta = min(q.dis, r.dis);         //���ߵ���С����

	for (auto it = Py.begin(); it != Py.end(); it++) {        //����Sy
		if (abs(it->x - mid.x) <= sqrt(delta)) {
			Sy.push_back(*it);
		}
	}

	point_pair tmp;
	bool flag = true;
	double _delta = delta;
	for (auto it = Sy.begin(); it != Sy.end(); it++) {        //Sy�б������
		for (auto itt = it+1; itt != Sy.end(); itt++) {
			if (itt - it >= 7)break;                           //�Ż�Ϊ�ڽ�7����
			if (_delta > dis(*it, *itt)) {
				flag = false;
				_delta = dis(*it, *itt);
				tmp = point_pair(*it, *itt, _delta);
			}
		}
	}

	if (flag) {
		tmp = delta == q.dis ? q : r;
	}

	return tmp;
}

int main()
{
	int n;
	cin >> n;
	double x, y;
	vector<Point> Px, Py;

	for (int i = 0; i < n; i++) {
		cin >> x >> y;
		Point tmp(x, y);
		Px.push_back(tmp);
		Py.push_back(tmp);
	}
	sort(Px.begin(), Px.end(), cmpx);
	sort(Py.begin(), Py.end(), cmpy);

	point_pair ans = Closest_Pair_Rec(Px, Py);

	cout <<fixed<<setprecision(2)<< ans.dis;

}