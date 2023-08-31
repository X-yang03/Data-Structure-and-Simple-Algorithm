#include <iostream>
#include <vector>
#include<stdio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include<queue>
#include<set>
using namespace std;
#define footPrint  "*"//就是个全角的点

int x_now = 1, y_now = 1;//初始
int Maze[39][39];
#define m 37//row
#define n 37

//定义block的direction
#define down 3
#define right 2
#define left 4
#define up 8

int cnt = 0;

#define WALL 1  //墙壁，即block
#define NOTHING 0   //可行空间

//竖着 Y 行数
#define rows 39
//横着 X 列数
#define cols 39

//距离代价10
#define PRICE 10
#define O_PRICE 14

long long headTime, tailTime, freq;

int endX = m;
int endY = n;

int price = 0;
struct myPoint {
    int row;
    int col;
    int f, g, h; 
    //f是从初始点到目标点的估值，g是从初始节点到当前节点的实际代价，h是从当前到目标节点的估计代价
    void getPriceF() {
        f =  g + 2.5*h;

    }
};

struct point {
    int x;
    int y;
    
}start,exitPos;


HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

void SetPosition(int line, int col)//更改光标的位置
{
    static COORD pos;
    pos.X = col * 2;
    pos.Y = line;
    SetConsoleCursorPosition(hout, pos);
}

struct block {
    int row, column, direction;
    block(int r, int c, int d) {
        row = r;
        column = c;
        direction = d;
    }
};

vector<block> myblock;

void init() {
    //将地图全部置为墙
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            Maze[i][j] = 1;
    }
    //定义起始点
    Maze[1][1] = NOTHING;
    start.x = start.y = 1;
}

void Map_Example1(){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            if (i == rows - 1 || i == 0 || j == 0 || j == cols - 1)
                Maze[i][j] = 1;
            else
                Maze[i][j] = 0;
    }
//定义起始点
    Maze[1][1] = NOTHING;
    start.x = start.y = 1;
    for (int i = rows/4; i < rows/4*3; i++) {
        for (int j = cols/2; j <= cols/2+1; j++) {
            Maze[i][j] = 1;
        }
    }
}

void Map_Example2() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            if (i == rows - 1 || i == 0 || j == 0 || j == cols - 1)
                Maze[i][j] = 1;
            else
                Maze[i][j] = 0;
    }
    //定义起始点
    Maze[1][1] = NOTHING;
    start.x = start.y = 1;
     for (int i = 0; i < rows/2; i++) {
         for (int j = cols/3; j <= cols / 3 + 1; j++) {
             Maze[i][j] = 1;
         }
         }

     for (int i = rows/2; i < rows; i++) {
         for (int j = cols/3*2; j <= cols/3*2+1; j++) {
             Maze[i][j] = 1;
         }
     }
}

void PrintMap() {
    for (int i = 0; i <= m + 1; i++) {
        for (int j = 0; j <= n + 1; j++) {
            if (i == start.x && j == start.y) {
                cout << "★";
            }
            else if (i == endX && j == endY)
                cout << "※";
            else if (Maze[i][j] == WALL) {
                cout << "■";
            }
            else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

void FindBlock() {
    //找出与当前位置相邻的墙
    if (x_now + 1 <= m && Maze[x_now + 1][y_now] == WALL) {//down
        myblock.push_back(block(x_now + 1, y_now, down));
    }
    if (y_now + 1 <= n && Maze[x_now][y_now + 1] == WALL) {//right
        myblock.push_back(block(x_now, y_now + 1, right));
    }
    if (x_now - 1 >= 1 && Maze[x_now - 1][y_now] == WALL) {//up
        myblock.push_back(block(x_now - 1, y_now, up));
    }
    if (y_now - 1 >= 1 && Maze[x_now][y_now - 1] == WALL) {//left
        myblock.push_back(block(x_now, y_now - 1, left));
    }
}

//随机prime算法生成地图
void Map() {  //生成随机地图样例
    init();
    srand((unsigned)time(NULL));   
    //srand(1);
    FindBlock();
    //先压入起始点右边和下方的两堵墙，进入循环
    while (myblock.size()) {
      
        int BlockSize = myblock.size();

       
        int randnum = rand() % BlockSize;
        block SelectBlock = myblock[randnum]; //随机选择一堵墙

        x_now = SelectBlock.row;//
        y_now = SelectBlock.column;
        
       
        switch (SelectBlock.direction) {//打通
        case down: {
            x_now++;
            break;
        }
        case right: {
            y_now++;
            break;
        }
        case left: {
            y_now--;
            break;
        }
        case up: {
            x_now--;
            break;
        }
        }
    
        if (Maze[x_now][y_now] == WALL) {//目标块如果是墙

            //打通墙和目标块
            Maze[SelectBlock.row][SelectBlock.column] = Maze[x_now][y_now] = NOTHING;
           
            FindBlock(); //再次找出当前位置相邻的墙
        }
   
        myblock.erase(myblock.begin() + randnum);   //将已经打通的墙删除
    }
    
    x_now = 1, y_now = 1;
    Maze[endX][endY] = NOTHING;

}

//树的节点类型
struct TreeNode {
    myPoint pos;//当前点坐标
    vector <TreeNode*> childNodes;//存储当前点的孩子节点指针的数组
    TreeNode* parentNode;//储存当前点的父节点指针的变量
};

enum direct { p_up, p_down, p_left, p_right, p_lup, p_ldown, p_rup, p_rdown };

class cmp {
public:
    bool operator() ( myPoint &a,  myPoint &b)
    {
        return a.f > b.f;
    }
};

priority_queue<myPoint,vector<myPoint>,cmp> Open;
bool ifOpen[rows][cols] = { 0 };
bool Path[rows][cols] = { 0 };
TreeNode* OpenNode[rows][cols];

//multiset<TreeNode*, cmp> Open;

//计算H(x), endPos:终点坐标 pos:当前点坐标
int H(myPoint endPos, myPoint pos) {    //曼哈顿距离
    int x = ((endPos.col > pos.col) ? (endPos.col - pos.col) : (pos.col - endPos.col));
    int y = ((endPos.row > pos.row) ? (endPos.row - pos.row) : (pos.row - endPos.row));
    return (x + y) * PRICE;
}

int H_O(myPoint endPos, myPoint pos) {   //
    int x =endPos.row - pos.row;
    int y = endPos.col- pos.col;

    if (x <= y) {
        return x * O_PRICE + (y - x) * PRICE;
    }
    else {
        return y * O_PRICE + (x - y) * PRICE;
    }

}

bool ifLegal(myPoint pos, int map[rows][cols], bool Path[rows][cols]) {
    
    if (pos.row >= rows || pos.row < 0 || pos.col >= cols || pos.col < 0)
    {
        return false;
    }
    if (map[pos.row][pos.col])    //在地图上
        return false;

    if (Path[pos.row][pos.col])   //相当于close
        return false;
    

    return true;
}


void Dijkstra()//实际上退化为广度搜索，此版本只能上下左右四个方向扩展
{
    queue<TreeNode*> Que;

    int Map[rows][cols] = { 0 };
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
        {
            Map[i][j] = Maze[i][j];

        }

    }

    myPoint beginPos{ 1,1 };
    myPoint endPos{ endX,endY };

    //标记起点走过
    Path[beginPos.row][beginPos.col] = 1;

    //创建一棵树，起点是树的根节点
    TreeNode* root = new TreeNode;

    memset(root, 0, sizeof TreeNode);
    //root一直指向树的根节点

    //根节点为起点
    root->pos = beginPos;

    Que.push(root);

    TreeNode* CurrentNode = NULL;
    TreeNode* child = NULL;
    bool ifExit = false;
    QueryPerformanceCounter((LARGE_INTEGER*)&headTime);
    while (Que.size()) {
        CurrentNode = Que.front();
        Que.pop();
        cnt++;

        if (CurrentNode->pos.row == endPos.row && CurrentNode->pos.col == endPos.col) {
            ifExit = true;
            break;
        }
        SetPosition(CurrentNode->pos.row, CurrentNode->pos.col);
        SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);   //蓝色是切实走过的currentNode的痕迹
        cout << footPrint;
        //Sleep(10);

        for (int i = 0; i < 4; i++) {
            myPoint pos = CurrentNode->pos;
            switch (i) {
            case p_up:
                pos.row--;
                break;
            case p_down:
                pos.row++;

                break;
            case p_left:
                pos.col--;

                break;

            case p_right:
                pos.col++;

                break;
            }
            if (ifLegal(pos, Map, Path)) {
                Path[pos.row][pos.col] = true;
                child = new TreeNode;
                child->pos = pos;
                Que.push(child);
                child->parentNode = CurrentNode;
                CurrentNode->childNodes.push_back(child);

            }

        }
    }

    if (ifExit == true) {
        QueryPerformanceCounter((LARGE_INTEGER*)&tailTime);

        while (CurrentNode) {     //沿路返回，找到路线
            SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            SetPosition(CurrentNode->pos.row, CurrentNode->pos.col);
            cout << footPrint;
            if (CurrentNode->parentNode != nullptr && abs(CurrentNode->pos.row - CurrentNode->parentNode->pos.row) == 1 &&
                abs(CurrentNode->pos.col - CurrentNode->parentNode->pos.col) == 1)
                price += 14;
            else
                price += 10;
            CurrentNode = CurrentNode->parentNode;
        }
        SetPosition(1, 1);
        cout << "★";
        SetPosition(endX, endY);
        cout << "※";
        SetPosition(m + 2, 0);
        cout << "find time:" << (tailTime - headTime) * 1000 / freq << "ms" << endl;
    }

    SetPosition(m + 2, 0);

}

void Dijkstra_diag()//实际上退化为广度搜索
{
    queue<TreeNode*> Que;

    int Map[rows][cols] = { 0 };
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
        {
            Map[i][j] = Maze[i][j];

        }

    }

    myPoint beginPos{ 1,1 };
    myPoint endPos{ endX,endY };

    //标记起点走过
    Path[beginPos.row][beginPos.col] = 1;

    //创建一棵树，起点是树的根节点
    TreeNode* root = new TreeNode;

    memset(root, 0, sizeof TreeNode);
    //root一直指向树的根节点

    //根节点为起点
    root->pos = beginPos;

    Que.push(root);

    TreeNode* CurrentNode=NULL;
    TreeNode* child=NULL;
    bool ifExit = false;
    QueryPerformanceCounter((LARGE_INTEGER*)&headTime);
    while (Que.size()) {
        CurrentNode = Que.front();
        Que.pop();
        cnt++;

        if (CurrentNode->pos.row == endPos.row && CurrentNode->pos.col==endPos.col) {
            ifExit = true;
            break;
        }
        SetPosition(CurrentNode->pos.row, CurrentNode->pos.col);
        SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);   //蓝色是切实走过的currentNode的痕迹
        cout << footPrint;
        //Sleep(10);//取消注释后，能看见整个搜索拓展格点的流程

        for (int i = 0; i < 8; i++) {
            myPoint pos = CurrentNode->pos;
            switch (i) {
            case p_up:
                pos.row--;
                break;
            case p_down:
                pos.row++;

                break;
            case p_left:
                pos.col--;

                break;

            case p_right:
                pos.col++;

                break;
            case p_lup:
                pos.col--;
                pos.row--;

                break;

            case p_ldown:
                pos.col--;
                pos.row++;

                break;
            case p_rup:
                pos.col++;
                pos.row--;

                break;
            case p_rdown:
                pos.col++;
                pos.row++;

                break;
            }
            if (ifLegal(pos, Map, Path)) {
                    Path[pos.row][pos.col] = true;
                    child = new TreeNode;
                    child->pos = pos;
                    Que.push(child);
                    child->parentNode = CurrentNode;
                    CurrentNode->childNodes.push_back(child);
                
           }

        }
    }

    if (ifExit == true) {
        QueryPerformanceCounter((LARGE_INTEGER*)&tailTime);

        while (CurrentNode) {     //沿路返回，找到路线
            SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            SetPosition(CurrentNode->pos.row, CurrentNode->pos.col);
            cout << footPrint;
            if (CurrentNode->parentNode!=nullptr&&abs(CurrentNode->pos.row - CurrentNode->parentNode->pos.row) == 1 &&
                abs(CurrentNode->pos.col - CurrentNode->parentNode->pos.col) == 1)
                price += 14;
            else
                price += 10;
            CurrentNode = CurrentNode->parentNode;
        }
        SetPosition(1, 1);
        cout << "★";
        SetPosition(endX, endY);
        cout << "※";
        SetPosition(m + 2, 0);
    }

    SetPosition(m + 2, 0);
}

void A_star() {
    // 地图 0：空地  1：地图
    int Map[rows][cols] = { 0 };
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
        {
            Map[i][j] = Maze[i][j];

        }

    }

    //起点  终点
    myPoint beginPos{ 1,1 };
    myPoint endPos{ endX,endY };

    //标记起点走过
    Path[beginPos.row][beginPos.col] = 1;

    //创建一棵树，起点是树的根节点
    TreeNode* root = new TreeNode;

    memset(root, 0, sizeof TreeNode);
    //root一直指向树的根节点

    //根节点为起点
    root->pos = beginPos;

    Open.push(root->pos); //初始将root加入Open列表
    OpenNode[1][1] = root;
    //寻路
    TreeNode* currentNode = root;//当前点
    TreeNode* child = NULL;
    //迭代器
    bool ifExit = false;

    QueryPerformanceCounter((LARGE_INTEGER*)&headTime);
    while (Open.size() != 0) {

        myPoint currentPos = Open.top();
        Open.pop();
        while (Path[currentPos.row][currentPos.col] && currentNode != root) {
            //出于可能会对最小堆中节点的F值动态调整的考虑，由于只会不断调整使得F更小，更小的元素必将更早pop出并使得相应位置Path为true
            currentPos = Open.top();
            Open.pop();
        }

        currentNode = OpenNode[currentPos.row][currentPos.col];   //获取最小Open点
        cnt++;

        if (currentNode->pos.row == endPos.row
            && currentNode->pos.col == endPos.col) {
            ifExit = true;     //为终点，则找到，跳出循环
            break;
        }

        for (int i = 0; i < 4; i++) {     //曼哈顿距离中
            myPoint pos = currentNode->pos;  
            switch (i) {
            case p_up:
                pos.row--;
                pos.g += PRICE;
                break;
            case p_down:
                pos.row++;
                pos.g += PRICE;
                break;
            case p_left:
                pos.col--;
                pos.g += PRICE;
                break;

            case p_right:
                pos.col++;
                pos.g += PRICE;
                break;
           
            default:
                break;
            }

            if (ifLegal(pos, Map, Path)) {//合理范围内，且非close

                if (ifOpen[pos.row][pos.col]) {//open
                    child = OpenNode[pos.row][pos.col];   //获取该open节点
                    int preG = child->pos.g;
                    int newG = currentNode->pos.g + PRICE;
                     
                    if (newG < preG) {     //如果最新的g值更小，则更新它的F,和parent
                        
                        child->pos.g = newG;
                        child->parentNode = currentNode;
                        currentNode->childNodes.push_back(child);
                        child->pos.getPriceF();
                        Open.push(child->pos);  //Open列表更新调整,将更小F的pos传入，会更早pop出来，避免了对priority_queue的内部节点动态修改，引发错误
                    }

                }
                else {//不在open中
                    child = new TreeNode;
                    child->pos = pos;
                    ifOpen[pos.row][pos.col] = true;
                    OpenNode[pos.row][pos.col] = child;
                   
                    child->pos.h = H(endPos, child->pos); //计算h(x)
                    child->pos.getPriceF();
                    currentNode->childNodes.push_back(child);
                    child->parentNode = currentNode;
                    Open.push(child->pos);                    //记录该节点
                }
               
            }

        }
        Path[currentNode->pos.row][currentNode->pos.col] = true;

        SetPosition(currentNode->pos.row, currentNode->pos.col);
        SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);   //蓝色是切实走过的currentNode的痕迹
        cout << footPrint;
        //Sleep(10);//取消注释后，能看见整个搜索拓展格点的流程

    }
    if (ifExit == true) {
        QueryPerformanceCounter((LARGE_INTEGER*)&tailTime);
       
        while (currentNode) {     //沿路返回，找到路线
            SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            SetPosition(currentNode->pos.row, currentNode->pos.col);
            cout << footPrint;
            if (currentNode->parentNode != nullptr&&abs(currentNode->pos.row - currentNode->parentNode->pos.row) == 1 &&
                abs(currentNode->pos.col - currentNode->parentNode->pos.col) == 1)
                price += 14;
            else
                price += 10;
            currentNode = currentNode->parentNode;
            
        }
        SetPosition(1, 1);
        cout << "★";
        SetPosition(endX, endY);
        cout << "※";
        SetPosition(m + 2, 0);
        cout << "find time:" << (tailTime - headTime) * 1000 / freq << "ms" << endl;
    }

}

void A_star_diag() {
    // 地图 0：空地  1：地图
    int Map[rows][cols] = { 0 };
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
        {
            Map[i][j] = Maze[i][j];

        }

    }

   
    //起点  终点
    myPoint beginPos{ 1,1 };
    myPoint endPos{ endX,endY };

    //标记起点走过
    Path[beginPos.row][beginPos.col] = 1;

    //创建一棵树，起点是树的根节点
    TreeNode* root = new TreeNode;

    memset(root, 0, sizeof TreeNode);


    //根节点为起点
    root->pos = beginPos;

    Open.push(root->pos);
    OpenNode[1][1] = root;
   // open.push_back(root);
    //寻路
    TreeNode* currentNode = root;//当前点
    TreeNode* child = NULL;
    //迭代器
    bool ifExit = false;

    QueryPerformanceCounter((LARGE_INTEGER*)&headTime);
    
    while (Open.size() != 0) {

        myPoint currentPos = Open.top();
        Open.pop();
        while (Path[currentPos.row][currentPos.col]&&currentNode!=root) {
            //出于可能会对最小堆中节点的F值动态调整的考虑，由于只会不断调整使得F更小，更小的元素必将更早pop出并使得相应位置Path为true
            currentPos = Open.top();
            Open.pop();
        }

        currentNode = OpenNode[currentPos.row][currentPos.col];   //获取最小Open点
        cnt++;

        if (currentNode->pos.row == endPos.row
            && currentNode->pos.col == endPos.col) {
            ifExit = true;
            break;
        }


        for (int i = 0; i < 8; i++) {
            
            myPoint pos = currentNode->pos;
            
            switch (i) {
            case p_up:
                pos.row--;
                pos.g += PRICE;
                break;
            case p_down:
                pos.row++;
                pos.g += PRICE;
                break;
            case p_left:
                pos.col--;
                pos.g += PRICE;
                break;

            case p_right:
                pos.col++;
                pos.g += PRICE;

                break;
            case p_lup:
                pos.col--;
                pos.row--;
                pos.g += O_PRICE;
                break;

            case p_ldown:
                pos.col--;
                pos.row++;
                pos.g += O_PRICE;
                break;
            case p_rup:
                pos.col++;
                pos.row--;
                pos.g += O_PRICE;
                break;
            case p_rdown:
                pos.col++;
                pos.row++;
                pos.g += O_PRICE;
                break;

            default:
                break;
            }

            if (ifLegal(pos, Map, Path)) {//合理范围内，且非close

                if (ifOpen[pos.row][pos.col]) {//open
                    child = OpenNode[pos.row][pos.col];
                    int preG = child->pos.g;
                    int newG;
                    if (abs(child->pos.row - currentNode->pos.row) == 1 &&
                        abs(child->pos.col - currentNode->pos.col) == 1)
                    {
                        newG = currentNode->pos.g + O_PRICE;
                    }
                    else {
                        newG = currentNode->pos.g + PRICE;
                    }

                    if (newG < preG) {
                       

                        child->pos.g = newG;
                        child->parentNode = currentNode;
                        currentNode->childNodes.push_back(child);
                        child->pos.getPriceF();

                        Open.push(child->pos);  //更新调整
                    }
                }
                else {//不在open中
                    child = new TreeNode;
                    child->pos = pos;
                    ifOpen[pos.row][pos.col] = true;
                    OpenNode[pos.row][pos.col] = child;
                    
                    child->pos.h = H_O(endPos, child->pos); //计算h(x)
                    child->pos.getPriceF();
                    currentNode->childNodes.push_back(child);
                    child->parentNode = currentNode;
                    Open.push(child->pos);
                }


            }

        }

        Path[currentNode->pos.row][currentNode->pos.col] = true;


        SetPosition(currentNode->pos.row, currentNode->pos.col);
        SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);  //蓝色代表检索过的节点
        cout << footPrint;
        //Sleep(20);   取消注释后，能看见整个搜索拓展格点的流程

    }


    if (ifExit == true) {
        QueryPerformanceCounter((LARGE_INTEGER*)&tailTime);
       
        while (currentNode) {
            SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            SetPosition(currentNode->pos.row, currentNode->pos.col);
            cout << footPrint;
            if (currentNode->parentNode != nullptr&&abs(currentNode->pos.row - currentNode->parentNode->pos.row) == 1 &&
                abs(currentNode->pos.col - currentNode->parentNode->pos.col) == 1)
                price += 14;
            else
                price += 10;
            currentNode = currentNode->parentNode;
        }
        SetPosition(1, 1);
        cout << "★";
        SetPosition(endX, endY);
        cout << "※";
        SetPosition(m + 2, 0);
    }

}


int main()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    Map();
    //Map_Example2();

    PrintMap();
    A_star_diag();
    //Dijkstra();

    cout << "find time: " << (tailTime - headTime) * 1000 / freq << "ms" << endl;
    cout << "length of path: " << price << endl << "checked num: " << cnt << endl;
    return 0;

}
