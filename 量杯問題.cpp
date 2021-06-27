#include<iostream>
#include<vector>
#include<queue>
#include<list>
#include<stack>
#include<iomanip>
#include<time.h>
using namespace std;

#define condition vector<int>

struct Node;

int n, t;//n為杯子數量，t為期望得到的水量
int m[6];//儲存每個杯子的容量 m[1]為第一個杯子的容量
list<Node> nodes;//所有的節點儲存在這裡
queue<condition> q;

Node* get_node(const condition& c);//傳回代表該狀態的節點指標
bool check(const condition& c);//確認該狀態是否含有期望的水量
bool abc(const condition& pre_c, const condition& c);//做水量狀態的檢查和建立狀態之間的連結，我也不知道要取什麼名字
void infer_operation(const condition& pre_c, const condition& c);

struct Node{
    Node* pre; //代表此狀態上一步的節點
    vector<Node*> children;//索引值代表下一個水杯的水量
    int level;
    condition water;//代表的杯子狀態，只有最底下的節點會用到

    Node(int l){
        pre = nullptr;
        level = l;
        if(level < n){ //最底下的節點沒有子
            children.reserve(m[level+1]+1);
            for(int i = 0; i <= m[level+1]; i++){
                children.push_back(nullptr);
            }
        }
            
    }

    bool has_child(int i){
        return children[i] != nullptr;
    }

    bool has_check(){ //有讀過這個結果的話，此節點一定會與它上一步的節點關聯
        return pre != nullptr;
    }
};

int main(){
    double START,END; START = clock();
    cin>>n;
    for(int i = 1; i <= n; i++){
        cin>>m[i];
    }
    nodes.push_back(Node(0));//樹的根節點
    cin>>t;
    q.push(condition(n, 0));//初始狀態所有杯子水量均為0

    bool has_answer = 0;
    condition answer_condition;
    while(true){
        condition pre_c = q.front();
        q.pop();

        //將某杯子裝滿水
        for(int i = 0; i < n; i++){
            if(pre_c[i] == m[i+1]){ //如果已經滿水就裝不了了
                continue;
            }
            condition c = pre_c;
            c[i] = m[i+1];//將第i個杯子裝滿水
            if(abc(pre_c, c)){
                answer_condition = c;
                has_answer = 1;
                break;
            }
        }
        if(has_answer){
            break;
        }

        //將某杯子水倒掉
        for(int i = 0; i < n; i++){
            if(pre_c[i] == 0){ //如果已經沒水就倒不了了
                continue;
            }
            condition c = pre_c;
            c[i] = 0;
            if(abc(pre_c, c)){
                answer_condition = c;
                has_answer = 1;
                break;
            }
        }
        if(has_answer){
            break;
        }

        //將某杯水的水倒到某個杯子
        for(int i = 0; i < n; i++){ //要倒水的杯子
            for(int j = 0; j < n; j++){ //要被倒的杯子
                if(i == j){ //同一個杯子
                    continue;
                }
                if(pre_c[i] == 0){ //沒水可倒
                    continue;
                }
                if(pre_c[j] == m[j+1]){ //滿的倒不了
                    continue;
                }
                condition c = pre_c;
                c[j]+=c[i];//倒水
                if(c[j] > m[j+1]){ //如果倒超出容量
                    c[i] = c[j]-m[j+1];//把多的放回去
                    c[j] = m[j+1];
                }else{
                    c[i] = 0;//水倒完了
                }
                if(abc(pre_c, c)){
                    answer_condition = c;
                    has_answer = 1;
                    break;
                }
            }
            if(has_answer){
                break;
            }
        }
        if(has_answer || q.empty()){
            break;
        }
    }

    if(!has_answer){
        cout<<"你在逗我\n";
        return 0;
    }
    Node* node = get_node(answer_condition);
    Node* initial_condition = get_node(condition(n, 0));
    stack<condition> con;
    do{ //由最後狀態反向搜索到初始狀態
        con.push(node->water);
        node = node->pre;
    }while(node != initial_condition);

    cout<<"需要"<<con.size()<<"步\n";
    condition pre_c = initial_condition->water;
    while(!con.empty()){
        for(int i = 0; i < n; i++){
            cout<<setw(2)<<con.top()[i];
            cout<<"/"<<m[i+1]<<" ";
        }
        infer_operation(pre_c, con.top());
        pre_c = con.top();
        con.pop();
    }

    END = clock();
    cout << (END - START) / CLOCKS_PER_SEC << endl;
}

Node* get_node(const condition& c){ //傳回代表該狀態的節點指標
    Node* n = &nodes.front();
    for(int i = 0; i < c.size(); i++){
        if(!n->has_child(c[i])){
            nodes.push_back(Node(n->level+1));
            n->children[c[i]] = &nodes.back();
        }
        n = n->children[c[i]];
    }
    n->water = c;//儲存該節點所代表的水杯狀態
    return n;
}

bool check(const condition& c){ //確認該狀態是否含有期望的水量
    for(int i : c){
        if(i == t){
            return true;
        }
    }
    return false;
}

bool abc(const condition& pre_c, const condition& c){ //做水量狀態的檢查和建立狀態之間的連結，我也不知道要取什麼名字
    Node* node = get_node(c);
    if(node->has_check()){ //如果已經跑過這個組合就不用再跑一次了
        return false;
    }
    node->pre = get_node(pre_c);//建立順序的關聯
    q.push(c);
    return check(c);
}

void infer_operation(const condition& pre_c, const condition& c){
    int position = -1;
    int difference = 0;
    for(int i = 0; i < n; i++){
        if(pre_c[i] == c[i]){//杯子水量沒變，看下一個
            continue;
        }
        if(position == -1){//是遇到的第一個水量變了的杯子
            position = i;//儲存是哪一個杯子水量變了
            difference = c[i] - pre_c[i];//儲存杯子的水量差
        }else{//有兩個水量變了的杯子，代表所做的操作一定是把一個杯子的水倒倒另一個杯子
            if(difference > 0){//先前存的杯子是被倒水的
                cout<<"將第"<<i+1<<"個杯子的水倒到第"<<position+1<<"個杯子裡\n";
            }else{
                cout<<"將第"<<position+1<<"個杯子的水倒到第"<<i+1<<"個杯子裡\n";
            }
            return;
        }
    }
    if(difference > 0){//那個杯子是被倒水的
        cout<<"將第"<<position+1<<"個杯子裝滿\n";
    }else{
        cout<<"將第"<<position+1<<"個杯子倒空\n";
    }
    return;
}
