//将字符串进行Hash映射，实现单词搜索等功能
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#define MAXSIZE 5000
int searchcount1 = 0;
int searchcount2 = 0;
int searchcount3 = 0;
int G[7] = { 1,31, 31 * 31, 31 * 31 * 31, 31 * 31 * 31 * 31, 31 * 31 * 31 * 31 * 31, 31 * 31 * 31 * 31 * 31 * 31};

//void initialG()
//{
//
//}

struct HashNode {
	bool Flag = true;
	string value;
	vector<string> CollsionArray;
};

//int G[7] = { 0, 31, 31 * 31, 31 * 31 * 31, 31 * 31 * 31 * 31, 31 * 31 * 31 * 31 * 31, 31* 31 * 31 * 31 * 31 * 31 };
HashNode Array1[MAXSIZE];
HashNode Array2[MAXSIZE];
HashNode Array3[MAXSIZE];
int boolVowel(int key) {
	if (key == 0 || key == 4 || key == 8 || key == 14 || key == 20 || key == 24)
		return 1;
	else
		return 0;
}
int Hash1(string a) {
	int tempKey = 0;
	string temp = a;
	for (int i = 0; i < 6 && i < a.size(); i++) {
		if (a[i] >= 'a' && a[i] <= 'z') {
			//a[i] -= 97;
			tempKey += int(a[i] - 'a') * G[i];
		}
		else if (a[i] >= 'A' && a[i] <= 'Z') {
			//a[i] -= 65;
			tempKey += int(a[i] - 'A') * G[i];
		}
		else {
			return tempKey;
		}
	}
	//if (tempKey == 0) {
	//	cout << temp << endl;
	//}
	return tempKey;
}
int Hash2(string a) {
	int tempKey = 0;
	string temp = a;
	for (int i = 0; i < a.size(); i++) {
		if (a[i] >= 'a' && a[i] <= 'z') {
			a[i] -= 97;
		}
		else if (a[i] >= 'A' && a[i] <= 'Z') {
			a[i] -= 65;
		}
	}
	int tempnumber = 0;
	int j = 0;
	for (int i = 0; i < a.size(); i++) {
		if (!boolVowel(int(a[i]))) {
			tempnumber += a[i];
		}
		else {
			tempKey += (G[++j] *(int(a[i]))+tempnumber);
			tempnumber = 0;
			if (j >= 5) {
				return tempKey;
			}
		}
	}
	return tempKey;
}
int fold(string a) {
	int tempkey = 0;
	int foldLen = 4;
	string tmp;
	while (true) {
		if (a.length() < foldLen)
		{
			int j = 1;
			for (int i = a.length() - 1; i >= 0; i--)
			{
				if(a[i]>='a'&&a[i]<='z')
					tempkey += int(a[i] - 'a') * j;
				j *= 10;
			}
			break;
		}
		else
		{
			int j = 1000;
			tmp = a.substr(0, 4);
			a.erase(0, 4);
			for (int i = 0; i < 4; i++)
			{
				if(tmp[i]>='a'&&tmp[i]<='z')
				tempkey += int(tmp[i]-'a') * j;
				j /= 10;
			}
			
		}
	}
	return tempkey%5000;
}

int SearchCollsionValue1(vector<string>temp, string a, int Index) {
	for (auto it = temp.begin(); it != temp.end(); it++) {
		searchcount1 += 1;
		if ((*it) == a) {
			return Index;
		}
		/*for (int j = 0; j < a.size(); j++) {
			if ((*it)[j] != a[j]) {
				break;
			}
			if (j + 1 == a.size()) {
				return Index;
			}
		}*/
	}
	return -1;
}
int SearchCollsionValue2(vector<string>temp, string a, int Index) {
	for (auto it = temp.begin(); it != temp.end(); it++) {
		searchcount2 += 1;
		if ((*it) == a) {
			return Index;
		}
		/*for (int j = 0; j < a.size(); j++) {
			if ((*it)[j] != a[j]) {
				break;
			}
			if (j + 1 == a.size()) {
				return Index;
			}
		}*/
	}
	return -1;
}
int SearchCollsionValue3(vector<string>temp, string a, int Index) {
	for (auto it = temp.begin(); it != temp.end(); it++) {
		searchcount3 += 1;
		if ((*it) == a) {
			return Index;
		}
		/*for (int j = 0; j < a.size(); j++) {
			if ((*it)[j] != a[j]) {
				break;
			}
			if (j + 1 == a.size()) {
				return Index;
			}
		}*/
	}
	return -1;
}

int Searchvalue1(string a) {
	searchcount1 += 1;
	int booliffind = Hash1(a) % 5000;
	bool flag1 = false;
	if (Array1[booliffind].Flag) {
		return -1;
	}
	else {
		if (Array1[booliffind].value == a) {
			flag1 = true;
		}
		//for (int i = 0; i < a.size(); i++) {
		//	if (Array[booliffind].value[i] != a[i]) {
		//		flag1 = false;
		//		break;
		//	}
		//}
		if (flag1) {
			return booliffind;
		}
		else {
			return SearchCollsionValue1(Array1[booliffind].CollsionArray, a, booliffind);
		}
		//for (auto it = Array[booliffind].CollsionArray.begin(); it != Array[booliffind].CollsionArray.end(); it++) {
		//	for (int j = 0; j < it->size(); j++) {
		//		if (Array[booliffind].value[j] != a[j]) {
		//			break;
		//		}
		//	}
		//}
	}
	return 0;
}
int Searchvalue2(string a) {
	searchcount2 += 1;
	int booliffind = Hash2(a) % 5000;
	bool flag1 = false;
	if (Array2[booliffind].Flag) {
		return -1;
	}
	else {
		if (Array2[booliffind].value == a) {
			flag1 = true;
		}
		//for (int i = 0; i < a.size(); i++) {
		//	if (Array[booliffind].value[i] != a[i]) {
		//		flag1 = false;
		//		break;
		//	}
		//}
		if (flag1) {
			return booliffind;
		}
		else {
			return SearchCollsionValue2(Array2[booliffind].CollsionArray, a, booliffind);
		}
		//for (auto it = Array[booliffind].CollsionArray.begin(); it != Array[booliffind].CollsionArray.end(); it++) {
		//	for (int j = 0; j < it->size(); j++) {
		//		if (Array[booliffind].value[j] != a[j]) {
		//			break;
		//		}
		//	}
		//}
	}
	return 0;
}
int Searchvalue3(string a) {
	searchcount3 += 1;
	int booliffind = fold(a);
	bool flag1 = false;
	if (Array3[booliffind].Flag) {
		return -1;
	}
	else {
		if (Array3[booliffind].value == a) {
			flag1 = true;
		}
		//for (int i = 0; i < a.size(); i++) {
		//	if (Array[booliffind].value[i] != a[i]) {
		//		flag1 = false;
		//		break;
		//	}
		//}
		if (flag1) {
			return booliffind;
		}
		else {
			return SearchCollsionValue2(Array2[booliffind].CollsionArray, a, booliffind);
		}
		//for (auto it = Array[booliffind].CollsionArray.begin(); it != Array[booliffind].CollsionArray.end(); it++) {
		//	for (int j = 0; j < it->size(); j++) {
		//		if (Array[booliffind].value[j] != a[j]) {
		//			break;
		//		}
		//	}
		//}
	}
	return 0;
}

void createTable1()
{
	fstream src("input.txt", ios::out || ios::in);
	if (src.is_open()) {
		while (!src.eof()) {
			bool flag = true;
			string a;
			src >> a;
			if (src.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << " ";
			int temp = Hash1(a) % 5000;
			if (Array1[temp].Flag) {
				Array1[temp].value = a;
				Array1[temp].Flag = false;
			}
			else {
				if (Array1[temp].value == a) {
					flag = false;
					//cout << endl;
					continue;
				}
				else {
					for (auto it = Array1[temp].CollsionArray.begin(); it != Array1[temp].CollsionArray.end(); it++) {
						if (*it == a) {
							flag = false;
							break;
						}
					}
				}
				if(flag) Array1[temp].CollsionArray.push_back(a);
			}

			//cout << temp << endl;
		}
	}
	src.close();
}

void createTable2()
{
	fstream src1("input.txt", ios::out || ios::in);
	if (src1.is_open()) {
		while (!src1.eof()) {
			bool flag = true;
			string a;
			src1 >> a;
			if (src1.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << " ";
			int temp = Hash2(a) % 5000;
			if (Array2[temp].Flag) {
				Array2[temp].value = a;
				Array2[temp].Flag = false;
			}
			else {
				if (Array2[temp].value == a) {
					flag = false;
					//cout << endl;
					continue;
				}
				else {
					for (auto it = Array2[temp].CollsionArray.begin(); it != Array2[temp].CollsionArray.end(); it++) {
						if (*it == a) {
							flag = false;
							break;;
						}
					}
				}
				if (flag)
					Array2[temp].CollsionArray.push_back(a);
			}

			//cout<<Hash2(a)%5000 << endl;
		}
	}
	src1.close();
}

void createByfold()
{
	fstream src1("input.txt", ios::out || ios::in);
	if (src1.is_open()) {
		while (!src1.eof()) {
			bool flag = true;
			string a;
			src1 >> a;
			if (src1.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << " ";
			int temp = fold(a);
			if (Array3[temp].Flag) {
				Array3[temp].value = a;
				Array3[temp].Flag = false;
			}
			else {
				if (Array3[temp].value == a) {
					flag = false;
					//cout << endl;
					continue;
				}
				else {
					for (auto it = Array3[temp].CollsionArray.begin(); it != Array3[temp].CollsionArray.end(); it++) {
						if (*it == a) {
							flag = false;
							break;;
						}
					}
				}
				if (flag)
					Array3[temp].CollsionArray.push_back(a);
			}

			//cout <<fold(a) << endl;
		}
	}
	src1.close();
}

void getResult() {
	int numofwords = 0;
	cout << "第一种按字母：" << endl << endl;
	fstream src2("test.txt", ios::out || ios::in);
	if (src2.is_open()) {
		while (!src2.eof()) {
			string a;
			src2 >> a;
			numofwords++;
			if (src2.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << endl;
			cout << a << "的位置是" << Searchvalue1(a) << endl;;

		}
		//cout << Searchvalue("the") << endl;;
		//cout << Array[Searchvalue("the")].value << endl;;
		cout << "总共查找了" << searchcount1 << "次" << endl;
		double time = searchcount1;
		cout << "ASL=" << double(time/numofwords)<<endl;
	}
	src2.close();
	cout << endl << endl << "第二种按读音:     " << endl << endl;
	fstream src3("test.txt", ios::out || ios::in);
	if (src3.is_open()) {
		while (!src3.eof()) {
			string a;
			src3 >> a;
			if (src3.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << endl;
			cout << a << "的位置是" << Searchvalue2(a) << endl;;

		}
		//cout << Searchvalue("the") << endl;;
		//cout << Array[Searchvalue("the")].value << endl;;
		cout << "总共查找了" << searchcount2 << "次" << endl;
		double time = searchcount2;
		cout << "ASL=" << float(time/numofwords) << endl;
	}
	src3.close();

	cout << endl << endl << "第三种折叠:     " << endl << endl;
	fstream src4("test.txt", ios::out || ios::in);
	if (src4.is_open()) {
		while (!src4.eof()) {
			string a;
			src4 >> a;
			if (src4.fail()) {
				break;
			}
			if (a[a.size() - 1] >= 'z' || a[a.size() - 1] <= 'A') {
				a.erase(a.size() - 1);
			}
			//cout << a << endl;
			cout << a << "的位置是" << Searchvalue3(a) << endl;;

		}
		//cout << Searchvalue("the") << endl;;
		//cout << Array[Searchvalue("the")].value << endl;;
		cout << "总共查找了" << searchcount3 << "次" << endl;
		double time = searchcount3;
		cout << "ASL=" << float(time/numofwords) << endl;
		
	}
}

int main() {
	createTable1();
	createTable2();
	createByfold();
	getResult();
	
	

}