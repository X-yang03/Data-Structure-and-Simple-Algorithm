//GS稳定匹配问题
#include<iostream>
#include<queue>
#include<cstring>
using namespace std;

int main()
{
	int n;
	int man[30][30], woman[30][30];
	cin >> n;
	queue<int> freeMan;
	for (int i = 0; i < n; i++)
	{
		freeMan.push(i);
		for (int j = 0; j < n; j++)
			cin >> man[i][j];
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> woman[i][j];

	int mstate[30] = { 0 }, wmstate[30];  //��¼ƥ��
	memset(mstate, 0xff, sizeof(mstate));
	memset(wmstate, 0xff, sizeof(wmstate));
	int nextPropose[30] = { 0 };   //��¼��һ��������
	int m = 0, w = 0;
	while (!freeMan.empty())  //������������
	{
		m = freeMan.front();
		if(nextPropose[m]<n)
			w = man[m][nextPropose[m]]-1;
		if (wmstate[w] == -1)
		{
			mstate[m] = w;
			wmstate[w] = m;   //Լ��
			freeMan.pop();
			nextPropose[m]++;  //���ָ���1
		}
		else
		{
			int _m = wmstate[w];
			int preferenceM = 0, preference_M = 0;
			for (int i = 0; i < n; i++)
			{
				if (woman[w][i] == m+1)preferenceM = i;
				if (woman[w][i] == _m+1)preference_M = i;
			}
			if (preference_M < preferenceM)
			{
				nextPropose[m]++;
			}
			else
			{
				mstate[m] = w;
				wmstate[w] = m;
				freeMan.pop();
				freeMan.push(_m);
			}

		}

		
	}
	for (int i = 0; i < n; i++)
		cout << mstate[i]+1<<" ";

}