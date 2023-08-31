//根据一段文本，统计字符频率，生成哈夫曼树进行哈夫曼编码，并计算压缩比
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdio>
#include<algorithm>
#include<filesystem>
using namespace std;

struct HuffmanNode
{
	HuffmanNode * parent,* leftChild,* rightChild;
	
	int val;
	char a;
	HuffmanNode(int v,char c='0', HuffmanNode* p = NULL, HuffmanNode* l = NULL, HuffmanNode* r = NULL)
	{
		a = c; val = v; parent = p; leftChild = l; rightChild = r;
	}
	HuffmanNode(){}
	bool operator<(HuffmanNode& r)
	{
		return  val<r.val;
	}


};

bool myCmp(HuffmanNode* a, HuffmanNode* b)
{
	return a->val > b->val;
}

class HuffmanTree
{
public:
	HuffmanNode *root;

	HuffmanTree(HuffmanNode* rt=NULL)
	{
		root = rt;
	}
	HuffmanTree(HuffmanNode* rt, HuffmanTree* left, HuffmanTree* right)
	{
		root = rt;
		root->leftChild = left->root;
		root->rightChild = right->root;
	}
	HuffmanTree(vector<HuffmanNode*>& letter, int n);
	void enCode(vector<HuffmanNode*>& letter);
	void deCode(HuffmanTree* tree);
    friend bool myCmp(HuffmanNode* a, HuffmanNode* b);
	
};

HuffmanTree::HuffmanTree(vector<HuffmanNode*>& letter, int n)
{
	HuffmanNode* tmp1, * tmp2, * tmp3;
	while (letter.size() != 1)
	{
		tmp1 = letter.back();
		letter.pop_back();
		tmp2 = letter.back();
		letter.pop_back();
		tmp3 = new HuffmanNode(tmp1->val + tmp2->val, '0', NULL, tmp1, tmp2);
		tmp1->parent = tmp3;
		tmp2->parent = tmp3;
		letter.push_back(tmp3);
		sort(letter.begin(), letter.end(),myCmp);
		
	}
	tmp3 = letter.back();
	root = tmp3;	

}

void HuffmanTree::enCode(vector<HuffmanNode*> &letter)
{
	ofstream out("output.txt");
	string ch[58];
	out << "字符" << "   " << "出现次数" << "   " << "对应编码" << endl;
	while (letter.size() != 0)
	{
		HuffmanNode* tmp, * p,*tmp1;
		string code;
		tmp = letter.back();
		tmp1 = tmp;
		letter.pop_back();
		out << tmp->a << "         " << tmp->val << "       ";
		p = tmp->parent;
		while (p != NULL)
		{
			if (tmp == p->leftChild)
				code += "0";
			else
				code += "1";
			tmp = p;
			p = tmp->parent;
		}
		code.reserve();
		string code1(code.rbegin(), code.rend());
		out << code1 << endl;
		if ((int)tmp1->a > 96 && (int)tmp1->a < 123)
			ch[(int)tmp1->a - 97] = code1;
		else if (tmp1->a == ' ')
			ch[26] = code1;
		else if (tmp1->a == char(10))
			ch[27] = code1;
		else if (tmp1->a == ',')
			ch[28] = code1;
		else if (tmp1->a == '.')
			ch[29] = code1;
		else if (tmp1->a == '?')
			ch[30] = code1;
		else if ((int)tmp1->a == 39)
			ch[31] = code1;
		else if ((int)tmp1->a > 64 && (int)tmp1->a < 91)
			ch[(int)tmp1->a - 33] = code1;
	}

	ifstream in("inputfile2.txt");
	in >> noskipws;
	ofstream out01("outputfile2.dat");
	out01 << noskipws;
	char a;
	while (!in.eof())
	{
		in >> a;
		if (int(a) != 13&&!in.eof())
		{
			if ((int)a > 96 && (int)a < 123)
				 out01 << ch[int(a) - 97];
				//out01.write(ch[(int)a - 97].c_str(), sizeof(ch[(int)a - 97]));
			else if (a == ' ')
				out01 << ch[26];
			else if (int(a) == 10)
				out01 << ch[27];
			else if (a == ',')
				out01 << ch[28];
			else if (a == '.')
				out01 << ch[29];
			else if (a == '?')
				out01 << ch[30];
			else if ((int)a == 39)
				out01 << ch[31];
			else if ((int)a > 64)
				out01 << ch[(int)a - 33];
			//cout << a;
		}
	}
	out01.close();
	in.close();
	
}

void HuffmanTree::deCode(HuffmanTree* tree)
{
	ifstream dat("outputfile2.dat");
	ofstream out("outputfile2.txt");
	//out << noskipws;
	//dat >> noskipws;
	char a;
	HuffmanNode* tmp;
	while (dat.peek()!=EOF)
	{
		tmp = tree->root;
		while (tmp->leftChild != NULL || tmp->rightChild != NULL)
		{
			dat >> a;
			
			if (a=='1')
				tmp = tmp->rightChild;
			else
				tmp = tmp->leftChild;
		}
		//if((int)tmp->a!=10)
		  out << tmp->a;
		//else
			//out<<'\n';
		cout << tmp->a;
		
	}
	
	//out << '\n' << "as";
}

int main()
{
	int tim[58];//0-25为小写字母26是空格27换行س28逗号 29 句号 30问号31'32-57大写字母

	vector<HuffmanNode*> s;
	vector<HuffmanNode*> s1;
	HuffmanNode* tmp;
	for (int i = 0; i < 58; i++)
		tim[i] = 0;
	ifstream infile("inputfile1.txt");
	infile >> noskipws;
	char a;
	double leng = 0;
	while (!infile.eof())
	{
		//infile >> a;
		a = infile.get();
		leng++;
		if ((int)a<123&&(int)a>96)
		{
			tim[(int)a - 97]++;
		}
		else if (a == ' ')
			{
			tim[26]++;
			}
		else if (int(a) == 10)
		{
			tim[27]++;
		}
		else if (a == ',')
		{
			tim[28]++;
		}
		else if (a == '.')
		{
			tim[29]++;
		}
		else if (a == '?')
		{
			tim[30]++;
		}
		else if ((int)a == 39)
		{
			tim[31]++;
		}
		else if (a > 64 && a < 91)
		{
			tim[(int)a -33]++;
		}
	}
	//tim[29]--;
	int num = 0;

	for (int i = 0; i < 58; i++)
	{
		if (tim[i] != 0)
		{
			if (i < 26)
			{
				tmp = new HuffmanNode(tim[i], char(i + 97));
		    }
			else if(i==26)
			{
				tmp = new HuffmanNode(tim[i], ' ');
			}
			else if (i == 27)
			{
				tmp = new HuffmanNode(tim[i], char(10));
			}
			else if (i == 28)
			{
				tmp = new HuffmanNode(tim[i], ',');
			}
			else if (i == 29)
			{
				tmp = new HuffmanNode(tim[i], '.');
			}
			else if (i == 30)
			{
				tmp = new HuffmanNode(tim[i], '?');
			}
			else if (i == 31)
			{
				tmp=new HuffmanNode(tim[i],char(39));
			}
			else if (i > 31)
			{
				tmp = new HuffmanNode(tim[i], char(i + 33));
			}
			
			s.push_back(tmp);
			s1.push_back(tmp);
			num++;
		}
	}
	sort(s.begin(), s.end(),myCmp);

	HuffmanTree* tree = new HuffmanTree(s, num);
	tree->enCode(s1);
	tree->deCode(tree);

	double leng1 = 0;
	ifstream dat("outputfile2.dat");
	char d='c';
	while (!dat.eof())
	{
		dat >> d;
		leng1++;
	}
	cout <<endl<<"压缩比："<< leng1 / (8 * leng);
}