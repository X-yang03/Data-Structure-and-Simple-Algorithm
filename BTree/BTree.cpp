//一颗可以进行插入删除等操作的完备的B树，可结合dot语言进行可视化
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<fstream>
#include<string>
using namespace std;

const int maxValue = 10000;

struct Bnode {
	int num;          //关键码个数
	Bnode* parent;    //父结点指针
	int m;
	Bnode* child[100] = { 0 };    //子树结点指针数组
	int key[100];     //记录关键码，key[0]不用，从1计数，key[num+1]放置maxValue
	Bnode(int m) {
		num = 0;
		this->m = m;
	     parent = NULL;
	}
	~Bnode() {
	}
	void printNode(ofstream &out) {
		//cout << setw(space);
		//cout << "(";
		string name=to_string(key[1]);
		string str=name+"[label=\"";
		for(int i=1;i<=num;i++)
		{
			str+=to_string(key[i]);
			str+=",";
		}
		str+="\"];";
		out<<str;

		for(int i=0;i<=num;i++)
		{
			if (child[i] != NULL) {
				out << name << "->" << child[i]->key[1] << ";" << endl;
			}
		}
		//for (int i = 1; i <= num; i++)
		//{
		//	cout << key[i] << " ";
		//}
		//cout <<")"<<endl;
		int i = 0;
		while (child[i] != NULL)
		{
			child[i]->printNode(out);
			i++;
		}
		
	}
};

struct ans
{
	Bnode* r;   //地址
	int i;    //关键码
	bool tag;  // 0成功，1失败
};

class BTree {
public:
	BTree();
	BTree(int m);
	Bnode* root;
	int m;     //路数

	ans search(const int& x);
	bool insert(const int& x);
	bool remove(const int& x);

	bool insertKey(Bnode* p, const int& x,Bnode *rp);
	void move(Bnode* p, Bnode* q, int s, int m);
	void compress(Bnode* p, int pos);
	void leftAdjust(Bnode* p, Bnode* q, int d, int j);
	void rightAdjust(Bnode* p, Bnode* q, int d, int j);
	void merge(Bnode* p, Bnode* q, Bnode* p1, int j);
	void treePrint();
};

void BTree::treePrint()
{
	//Bnode* parent=NULL;
	//Bnode* now = root;
	//while (now!= NULL)
	//{
	//	for (int i = 1; i <= now->num; i++)
	//	{
	//		
	//
	//	}
	//}
	ofstream out("out.dot",ios::out|ios::trunc);
	out<<"digraph{"<<endl<<"node[shape=record];"<<endl;
	root->printNode(out);
	out<<"}";

}

void BTree::compress(Bnode* p, int pos)
{
	for (int i = pos; i < p->num; i++)
	{
		p->key[i] = p->key[i + 1];
		p->child[i] = p->child[i + 1];
	}
	p->key[p->num] = maxValue;
	p->child[p->num] = NULL;
	p->num--;
}

BTree::BTree(int m)
{
	this->m = m;
	root = new Bnode(m);
}

ans BTree::search(const int& x)
{
	ans result;
	Bnode* p = root, * q = NULL;//p为扫描指针，q为p的父结点指针
	int i = 0;
	while (p != NULL)
	{
	    i = 0;
		p->key[(p->num) + 1] = maxValue;
		while (p->key[i + 1] < x)i++;
		if (p->key[i + 1] == x)
		{
			result.r = p;
			result.i = i + 1;
			result.tag = 0;
			return result;
		}
		q = p;
		p = p->child[i];    //下移到相应子树

	}
	result.r = q;
	result.i = i;   //记录位置i
	result.tag = 1;
	return result;
}

bool BTree::insertKey(Bnode* p, const int& x,Bnode* rp)
{
	int i = p->num;
	while (i > 0 && x < p->key[i])
	{
		p->key[i + 1] = p->key[i];//向后移位
		p->child[i + 1] = p->child[i];
		i--;
	}


	p->key[i + 1] = x;   //插入x
	p->child[i + 1] = rp;
	p->num++;

	return true;
}

void BTree::move(Bnode* p, Bnode* q, int s, int m)//将p的key[s+1...m]和child[s...m]移动到q的key[1...m-s]和child[0...m-s]，p.n变为s-1，q.n变为m-s
{
	int i;
	for (i = 1; i <= m-s; i++)
	{
		q->key[i] = p->key[i + s];
		q->child[i - 1] = p->child[i + s - 1];
		if(q->child[i-1]!=NULL)q->child[i - 1]->parent = q;
		p->child[i + s - 1] = NULL;
	}
	q->child[m-s] = p->child[m];
	if(q->child[m-s]!=NULL)q->child[m - s]->parent = q;
	p->child[m] = NULL;
	p->num = s - 1;
	q->num = m - s;
}

bool BTree::insert(const int& x)
{
	ans pos = search(x);
	if (!pos.tag)
		return false;//x已经存在
	Bnode* p = pos.r;  //p为要插入的地址
	Bnode* q, *t,* rp = NULL;
	int k = x;
	int j = pos.i;

	while (true)
	{
		if (p->num < m - 1)   //关键码个数未超出
		{
			insertKey(p, k,rp);
			return true;
		}
		int s = (m + 1) / 2;     //溢出则分裂结点,s为中间项
		insertKey(p, k,rp);          //先插入x
		q = new Bnode(m);           //分裂结点，先建立新结点
		move(p, q, s, m);          //将p的key[s+1...m]和child[s...m]移动到q的key[1...m-s]和child[0...m-s]，p.n变为s-1，q.n变为m-s
		k = p->key[s];             //新要插入的结点，p中只剩下s-1个关键码，p中有m-s个关键码
		p->key[s] = maxValue;      
		rp = q;                    //形成新的插入二元组（k,rp）
		if (p->parent != NULL)
		{
			t = p->parent;
			t->key[(t->num) + 1] = maxValue;
			q->parent = p->parent;
			p = t;
		}
		else
		{
			root = new Bnode(m);
			root->num = 1;
			root->parent = NULL;
			root->key[1] = k;
			root->child[0] = p;
			root->child[1] = rp;
			q->parent = p->parent = root;
			return true;
		}

	}


}

bool BTree::remove(const int& x)
{
	ans pos = search(x);
	if (pos.tag == 1)return false;
	Bnode* p = pos.r;
	Bnode* q, * s;
	int j = pos.i;
	if (p->child[j] != NULL)   //非叶节点
	{
		s = p->child[j];
		q = p;
		while (s != NULL)     //寻找叶节点
		{
			q = s;
			s = s->child[0];
		}
		p->key[j] = q->key[1];    //从叶节点找到替补
		compress(q, 1);     //删去叶结点中最小元
		p = q;         //转换为叶结点的删除

	}
	else compress(p, j);
	int d = (m + 1) / 2;    
	while (true)
	{
		if (p->num < d - 1)      //关键码数过少
		{
			j = 0;
			q = p->parent;     //开始从父节点中调整
			while (j <= q->num && q->child[j] != p)
			{
				j++;    //
			}
			if (j == q->num)    //p是q的最后个子树，p与q与p的左兄弟进行调整
				rightAdjust(p, q, d, j);
			else               //一般情况下，与q和p的右兄弟进行调整
				leftAdjust(p, q, d, j);
			p = q;          //向上调整
			if (p == root)break;
		}
		else
			break;   //关键码数量足够支持删除
	}
	if (root->num == 0)   //调整后根为0
	{
		p = root->child[0];
		delete root;
		root = p;
		root->parent = NULL;
	}
	return true;

}

void BTree::leftAdjust(Bnode* p, Bnode* q, int d, int j)//结点p与父节点q一起调整，d是最少阶数，j是父节点调整位置
{
	Bnode* p1 = q->child[j + 1];    //p的右兄弟
	if (p1->num > d - 1)          //右兄弟关键码足够分给p
	{
		p->num++;
		p->key[p->num] = q->key[j + 1];   //父节点相应关键码下沉
		q->key[j + 1] = p1->key[1];       //p1的最小关键码上移
		p->child[p->num] = p1->child[0];
		p1->child[0] = p1->child[1];
		compress(p1, 1);

	}
	else
		merge(p, q, p1, j + 1);
}

void BTree::rightAdjust(Bnode* p, Bnode* q, int d, int j)//与左子树调整
{
	Bnode* p1 = q->child[j - 1];    //p的左兄弟
	if (p1->num > d - 1)          //左兄弟关键码足够分给p
	{
		p->num++;
		for (int i = 2; i <= p->num; i++)
		{
			p->key[i] = p->key[i - 1];
			p->child[i] = p->child[i - 1];
		}
		p->key[1] = q->key[j - 1];   //父节点相应关键码下沉
		q->key[j - 1] = p1->key[p1->num];  //左兄弟最大关键码上移
		p->child[0] = p1->child[p1->num];
		p1->child[p1->num] = NULL;
		p1->key[p1->num] = maxValue;
		p1->num--;

	}
	else
		merge(p1, q, p, j);
}

void BTree::merge(Bnode* p, Bnode* q, Bnode* p1, int j)
{
	p->key[(p->num) + 1] = q->key[j];
	p->child[(p->num) + 1] = p1->child[0];
	for (int i = 1; i <= p1->num; i++)
	{
		p->key[p->num + 1 + i] = p1->key[i];
		p->child[p->num + 1 + i] = p1->child[i];
	}
	compress(q, j);   //将父节点进行压缩，j后的key和child左移
	p->num = p->num + p1->num + 1;
	delete p1;
}

int main()
{
	cout << "请输入初始关键码数与阶数：";
	//int n, m;//关键字个数，阶数
	//int m = 3;
	//BTree Tree(3);
	//Tree.insert(53);
	//Tree.insert(75);
	//Tree.insert(139);
	//Tree.insert(49);
	//Tree.insert(145);
	//Tree.insert(36);
	//int i;
	////Tree.insert(101);
	//int j = 0;
	//Tree.treePrint();
	////Tree.remove(101);
	//int k = 1;

	int n,m;
	cin >>n>> m;
	BTree Tree(m);
	int x;
	for (int i = 0; i < n; i++)
	{
		cin >> x;
		Tree.insert(x);
	}
	Tree.treePrint();
	string cmd("1");
	while (cmd != "0")
	{
		cin >> cmd;
		if (cmd == "I")
		{
			cin >> x;
			Tree.insert(x);
		}
		else if (cmd == "D")
		{
			cin >> x;
			Tree.remove(x);
		}
		else if(cmd=="MI")
		{
			//cout << "以以下格式输入：num，num，num……，num#" << endl;
			string str; 
			cin >> str;
			int i = 0;
			while (true)
			{
				x = 0;
				while (str[i] != ',')
				{
					if (str[i] == '#')break;
					x = x * 10 + (int)str[i++] - 48;
					
				}
				ans re = Tree.search(x);
				if (re.tag) {
					Tree.insert(x);
				}
				else
				{
					cout << "重复插入" << x<<endl;
				}
				if (str[i] == '#')break;
			
				i++;

			}
		}
		else if (cmd == "MD")
		{
			//cout << "以以下格式输入：num，num，num……，num#" << endl;
			string str;
			cin >> str;
			int i = 0;
			while (true)
			{
				x = 0;
				while (str[i] != ',')
				{
					if (str[i] == '#')break;
					x = x * 10 + (int)str[i++] - 48;

				}
				ans re = Tree.search(x);
				if (re.tag)
				{
					cout << "树中不存在" << x<<endl;
				}
				else
					Tree.remove(x);
				if (str[i] == '#')break;

				i++;

			}
		}
		cout << "___________________________" << endl;
		Tree.treePrint();
	}
	


}