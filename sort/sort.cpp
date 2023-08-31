//快排、插入、归并等四种排序算法
#include<iostream>     
#include<cstdlib>
#include<fstream>
#include<algorithm>

using namespace std;
const int maxSize = 10000;
int n[10000];
int is[10000];
int qs[10000];
int cs[10000];
int ms[10000];
int ss[10000];
int a[10000];

int** prime, ** b;
int N, M,k, Max;
int number;
int* numOf0,* even,* odd,* all;


class client
{
public:
	static string getName();
};

string client::getName() {
	return "name";
}

void randomNumber(ofstream& out,int len)
{
	int num;
	srand(time(0));
	for (int i = 0 ; i < len; i++)
	{
		num = rand() % 10000;
		out << num;
		if (i != 9999)out << " ";
	}
	out.close();
}

void randomNumber(int len)
{
	srand(time(0));
	for (int i = 0; i < len; i++)
	{
		qs[i] = rand() % 10000;
		
	}
}

void readFile(ifstream& in)
{
	int i = 0;
	while (!(in.peek() == EOF))
	{
		in >> n[i++];
	}
	in.close();
}

struct sortResult
{
	long long compareTime = 0;
	long long exchangeTime = 0;
};

sortResult insertionSort(int len)
{
	sortResult result;
	is[0] = n[0];
	int tmp;
	for (int i = 1; i < len; i++)
	{
		tmp = n[i];
		int j = i;
		while (tmp < is[j - 1]&&j>=1)
		{
			result.compareTime++;
			is[j] = is[j - 1]; 
			result.exchangeTime++;
			j--;
		}
		if(j)
			result.compareTime++;
		is[j] = tmp;
	}
	return result;
}

void Quick(int begin, int end, long long& cmpTime, long long& exTime)
{
	if (begin == end||begin>end)return;
	int flag = qs[begin];
	int i = begin, j = end;
	while (i != j) {
		while (qs[j] >= flag&&j>i)
		{
			j--;
			cmpTime++;
		}
		if(j>i)
		  cmpTime++;		
		while (qs[i] <= flag && i < j)
		{
			i++;
			cmpTime++;
		}
		if(i<j)
		  cmpTime++;
		if (i < j)
		{
			int tmp = qs[j];
			qs[j] = qs[i];
			qs[i] = tmp;
			exTime++;
		}


	}
	qs[begin] = qs[i];
	qs[i] = flag;
	exTime++;
	Quick(begin, i - 1, cmpTime, exTime);
	Quick(j + 1, end, cmpTime, exTime);
}

sortResult quickSort(int len) {
	sortResult result;
	Quick(0, len-1, result.compareTime, result.exchangeTime);
	return result;
}

sortResult cmpSort(int len) {
	sortResult result;
	int begin = 0;
	while (begin < len)
	{
		int min = cs[begin];
		int minpos = begin;
		int flag = 0;
		for(int i=begin+1;i<len;i++)
		{
			if (cs[i] < min) {
				min = cs[i];
				minpos = i;
				flag = 1;
			}
			result.compareTime++;
		}
		if (flag == 1) {
			cs[minpos] = cs[begin];
			cs[begin] = min;
			result.exchangeTime++;
		}
		begin++;

	}
	return result;
}

void merge(int left, int mid, int right, long long& cmpTime, long long& exTime)
{
	for (int i = left; i <= right; i++)
	{
		a[i] = ms[i];
	}
	int p1 = left, p2 = mid + 1, p3 = left;
	while (p1 <= mid && p2 <= right) {
		if (a[p1] <= a[p2])ms[p3++] = a[p1++];
		else
		{
			ms[p3++] = a[p2++];
			exTime++;
		}
		cmpTime++;

	}
	while (p1 <= mid)ms[p3++] = a[p1++];
	while (p2 <= right)ms[p3++] = a[p2++];
}

sortResult mergeSort(int left,int right)
{
	sortResult result;
	if (left >= right)return result;
	int mid = (right + left) / 2;
	sortResult r=mergeSort(left,mid);
	sortResult r1=mergeSort(mid + 1, right);
	result.compareTime += (r.compareTime + r1.compareTime);
	result.exchangeTime += (r.exchangeTime + r1.exchangeTime);
	merge(left, mid, right,result.compareTime,result.exchangeTime);
	return result;

}


sortResult shellSort(int left,int right)
{
	sortResult result;
	int i, j, gap = right-left+1;
	int tmp;
	do
	{
		gap = gap / 3 + 1;
		for(i=left+gap;i<=right;i++)
		{
			if (ss[i] < ss[i - gap]) {
				tmp = ss[i];
				j = i - gap;
				do {
					result.compareTime++;
					ss[j + gap] = ss[j];
					result.exchangeTime++;
					j = j - gap;
				} while (j >= left && tmp < ss[j]);
				ss[j + gap] = tmp;
				result.exchangeTime++;

			}
			result.compareTime++;
		}

	} while (gap > 1);
	return result;
}

void output(int len)
{
	ofstream out("result.txt");
	sortResult s1 = insertionSort(len);
	sortResult s2 = cmpSort(len);
	sortResult s3 = quickSort(len);
	sortResult s4 = mergeSort(0, len - 1);
	sortResult s5 = shellSort(0, len - 1);

	out << "          " << "比较次数" << "  " << "交换次数" << endl;
	out << "插入排序" << "    " << s1.compareTime << "     " << s1.exchangeTime << endl;
	out << "选择排序" << "     " << s2.compareTime << "     " << s2.exchangeTime << endl;
	out << "快速排序" << "     " << s3.compareTime << "      " << s3.exchangeTime << endl;
	out << "归并排序" << "    " << s4.compareTime << "     " << s4.exchangeTime << endl;
	out << "希尔排序" << "     " << s5.compareTime << "     " << s5.exchangeTime << endl;

}

void homework1()
{
	int len = 10000;
	ofstream out("num.txt");
	randomNumber(out, len);
	ifstream in("num.txt");
	readFile(in);
	/*for (int i = 0; i < len; i++)
		cout << n[i] << " ";*/
	for (int i = 0; i < len; i++) {
		qs[i] = n[i];
		cs[i] = n[i];
		ms[i] = n[i];
		ss[i] = n[i];
	}
	output(len);
	cout << "finished";
	for (int i = 0; i < len; i++)
	{
		cout << qs[i]<<" ";
	}
	cout << endl;
	for (int i = 0; i < len; i++)
	{
		cout << qs[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < len; i++)
	{
		cout << ss[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < len; i++)
	{
		cout << ms[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < len; i++)
	{
		cout << is[i] << " ";
	}
	cout << endl;
}

void GetData()
{
	cout << "请依次输入行数N、列数M、激光照射次数k(3≤N≤100，3≤M≤20，1<=k<=M):";
	cin >> N >> M >> k;
	prime = new int* [N];
	b = new int* [N];
	for (int i = 0; i < N; i++)
	{
		prime[i] = new int[M];
		b[i] = new int[M];
	}

	cout << "请输入矩阵：" << endl;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			cin >> prime[i][j];
			b[i][j] = prime[i][j];
		}
}

void Trans(int m)//列反转
{
	int n = N;
	while (n--)
		b[n][m] = abs(b[n][m] - 1);
}

int Judge()//计算导通行数
{
	int cnt = 0;
	for (int i = 0; i < N; i++)
	{
		bool flag = true;
		for (int j = 0; j < M; j++)
		{
			if (b[i][j] == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
			cnt++;
	}
	return cnt;
}

void Count()//计数每行0的个数存在numOf0里,0的个数为偶数且小于k的存于even，奇数存于odd里
{
	numOf0 = new int[N];
	odd = new int[N];
	even = new int[N];
	all = new int[N];
	int count0 = 0, count1 = 0;

	for (int i = 0; i < N; i++)
	{
		int tmp = 0;
		for (int j = 0; j < M; j++)
			if (prime[i][j] == 0)
				tmp++;
		numOf0[i] = tmp;
	}

	for (int i = 0; i < N; i++)
	{
		if (numOf0[i] <= k && numOf0[i] % 2 == 0)
			even[count0++] = numOf0[i];
		if (numOf0[i] <= k && numOf0[i] % 2 == 1)
			odd[count1++] = numOf0[i];
	}
	sort(even, even+count0);
	sort(odd, odd+count1);
	if (k % 2 == 0)
	{
		for (int i = 0; i < count0; i++)
			all[i] = even[i];
		number = count0;
	}
	else
	{
		for (int i = 0; i < count1; i++)
			all[i] = odd[i];
		number = count1;
	}
}


void GetResult()
{
	for (int i = 0; i < number; i++)
		for (int j = 0; j < N; j++)
		{
			if (all[i] == numOf0[j])
			{
				for (int n = 0; n < N; n++)
					for (int m = 0; m < M; m++)
						b[n][m] = prime[n][m];
				for (int m = 0; m < M; m++)
					if (b[j][m] == 0)
						Trans(m);
			}
			Max = (Max>= Judge() ? Max : Judge());
		}
	cout << "最大导通数为：" << Max << endl;
}

void homework2()
{
	GetData();
	Count();
	GetResult();
}
int main()
{
	homework1();
	//homework2();

}