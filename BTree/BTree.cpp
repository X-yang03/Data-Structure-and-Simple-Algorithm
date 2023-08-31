//һ�ſ��Խ��в���ɾ���Ȳ������걸��B�����ɽ��dot���Խ��п��ӻ�
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<fstream>
#include<string>
using namespace std;

const int maxValue = 10000;

struct Bnode {
	int num;          //�ؼ������
	Bnode* parent;    //�����ָ��
	int m;
	Bnode* child[100] = { 0 };    //�������ָ������
	int key[100];     //��¼�ؼ��룬key[0]���ã���1������key[num+1]����maxValue
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
	Bnode* r;   //��ַ
	int i;    //�ؼ���
	bool tag;  // 0�ɹ���1ʧ��
};

class BTree {
public:
	BTree();
	BTree(int m);
	Bnode* root;
	int m;     //·��

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
	Bnode* p = root, * q = NULL;//pΪɨ��ָ�룬qΪp�ĸ����ָ��
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
		p = p->child[i];    //���Ƶ���Ӧ����

	}
	result.r = q;
	result.i = i;   //��¼λ��i
	result.tag = 1;
	return result;
}

bool BTree::insertKey(Bnode* p, const int& x,Bnode* rp)
{
	int i = p->num;
	while (i > 0 && x < p->key[i])
	{
		p->key[i + 1] = p->key[i];//�����λ
		p->child[i + 1] = p->child[i];
		i--;
	}


	p->key[i + 1] = x;   //����x
	p->child[i + 1] = rp;
	p->num++;

	return true;
}

void BTree::move(Bnode* p, Bnode* q, int s, int m)//��p��key[s+1...m]��child[s...m]�ƶ���q��key[1...m-s]��child[0...m-s]��p.n��Ϊs-1��q.n��Ϊm-s
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
		return false;//x�Ѿ�����
	Bnode* p = pos.r;  //pΪҪ����ĵ�ַ
	Bnode* q, *t,* rp = NULL;
	int k = x;
	int j = pos.i;

	while (true)
	{
		if (p->num < m - 1)   //�ؼ������δ����
		{
			insertKey(p, k,rp);
			return true;
		}
		int s = (m + 1) / 2;     //�������ѽ��,sΪ�м���
		insertKey(p, k,rp);          //�Ȳ���x
		q = new Bnode(m);           //���ѽ�㣬�Ƚ����½��
		move(p, q, s, m);          //��p��key[s+1...m]��child[s...m]�ƶ���q��key[1...m-s]��child[0...m-s]��p.n��Ϊs-1��q.n��Ϊm-s
		k = p->key[s];             //��Ҫ����Ľ�㣬p��ֻʣ��s-1���ؼ��룬p����m-s���ؼ���
		p->key[s] = maxValue;      
		rp = q;                    //�γ��µĲ����Ԫ�飨k,rp��
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
	if (p->child[j] != NULL)   //��Ҷ�ڵ�
	{
		s = p->child[j];
		q = p;
		while (s != NULL)     //Ѱ��Ҷ�ڵ�
		{
			q = s;
			s = s->child[0];
		}
		p->key[j] = q->key[1];    //��Ҷ�ڵ��ҵ��油
		compress(q, 1);     //ɾȥҶ�������СԪ
		p = q;         //ת��ΪҶ����ɾ��

	}
	else compress(p, j);
	int d = (m + 1) / 2;    
	while (true)
	{
		if (p->num < d - 1)      //�ؼ���������
		{
			j = 0;
			q = p->parent;     //��ʼ�Ӹ��ڵ��е���
			while (j <= q->num && q->child[j] != p)
			{
				j++;    //
			}
			if (j == q->num)    //p��q������������p��q��p�����ֵܽ��е���
				rightAdjust(p, q, d, j);
			else               //һ������£���q��p�����ֵܽ��е���
				leftAdjust(p, q, d, j);
			p = q;          //���ϵ���
			if (p == root)break;
		}
		else
			break;   //�ؼ��������㹻֧��ɾ��
	}
	if (root->num == 0)   //�������Ϊ0
	{
		p = root->child[0];
		delete root;
		root = p;
		root->parent = NULL;
	}
	return true;

}

void BTree::leftAdjust(Bnode* p, Bnode* q, int d, int j)//���p�븸�ڵ�qһ�������d�����ٽ�����j�Ǹ��ڵ����λ��
{
	Bnode* p1 = q->child[j + 1];    //p�����ֵ�
	if (p1->num > d - 1)          //���ֵܹؼ����㹻�ָ�p
	{
		p->num++;
		p->key[p->num] = q->key[j + 1];   //���ڵ���Ӧ�ؼ����³�
		q->key[j + 1] = p1->key[1];       //p1����С�ؼ�������
		p->child[p->num] = p1->child[0];
		p1->child[0] = p1->child[1];
		compress(p1, 1);

	}
	else
		merge(p, q, p1, j + 1);
}

void BTree::rightAdjust(Bnode* p, Bnode* q, int d, int j)//������������
{
	Bnode* p1 = q->child[j - 1];    //p�����ֵ�
	if (p1->num > d - 1)          //���ֵܹؼ����㹻�ָ�p
	{
		p->num++;
		for (int i = 2; i <= p->num; i++)
		{
			p->key[i] = p->key[i - 1];
			p->child[i] = p->child[i - 1];
		}
		p->key[1] = q->key[j - 1];   //���ڵ���Ӧ�ؼ����³�
		q->key[j - 1] = p1->key[p1->num];  //���ֵ����ؼ�������
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
	compress(q, j);   //�����ڵ����ѹ����j���key��child����
	p->num = p->num + p1->num + 1;
	delete p1;
}

int main()
{
	cout << "�������ʼ�ؼ������������";
	//int n, m;//�ؼ��ָ���������
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
			//cout << "�����¸�ʽ���룺num��num��num������num#" << endl;
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
					cout << "�ظ�����" << x<<endl;
				}
				if (str[i] == '#')break;
			
				i++;

			}
		}
		else if (cmd == "MD")
		{
			//cout << "�����¸�ʽ���룺num��num��num������num#" << endl;
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
					cout << "���в�����" << x<<endl;
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