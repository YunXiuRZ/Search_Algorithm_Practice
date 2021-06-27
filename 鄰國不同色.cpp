#include<stdio.h>//輸入輸出
#include<unordered_map>
#include<vector>
#include<algorithm>//提供尋找的函式

using namespace std;

unordered_map<int, int> country_color;//儲存國家對應的顏色 {國家代號：顏色代號}
int map[30][30];//儲存地塊的國家所屬
vector<int> country_list;
int color_limit = 1;
int DIR[4][2] = {{1, 0},
               {-1, 0},
               {0, 1},
               {0, -1}};//檢查顏色是否衝突時用的位置差

void print_map();
bool check_color(int x, int y);
bool dfs(int n);


int main(){
    freopen("color900.txt", "r", stdin);
    freopen("color_out900.txt", "w", stdout);

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            scanf("%2d", &map[i][j]);
            if (map[i][j] == 0) {
                continue;
            }
            if (find(country_list.cbegin(), country_list.cend(), map[i][j]) == country_list.cend()){
                country_list.push_back(map[i][j]);//如果國家列表內無該國家就添加
            }
        }
    }
    if (country_list.size() == 0) {
        print_map();//如果沒有國家就輸出空地圖
        return 0;
    }
    for (; color_limit <= country_list.size(); color_limit++) {
        if (dfs(0)) {
            print_map();
            break;
        }
        country_color.clear();
    } 
}

void print_map(){
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            char c = static_cast<char>(country_color[map[i][j]]+96);
            printf("%c ", c);
        }
        printf("\n");
    }
}

bool check_color(int x, int y){
    for (int i = 0; i < 4; i++) {
        int checkX = x + DIR[i][0], checkY = y + DIR[i][1];//旁邊的區塊座標
        if (checkX < 0 || checkX >= 30 || checkY < 0 || checkY >= 30) {
            continue;//超出地圖範圍
        }
        if (map[checkX][checkY] == map[x][y]) {
            continue;//同國家的地塊
        }
        if (country_color[map[checkX][checkY]] == country_color[map[x][y]]) {
            return false;
        }
    }
    return true;
}

bool dfs(int n){
    country_color[country_list[n]]++;//推進顏色
    if (country_color[country_list[n]] > color_limit) {//超出顏色限制
        if (n == 0) {
            return false;
        }else{
            country_color[country_list[n]] = 0;//顏色淨空
            return dfs(n-1);
        }
    }
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (map[i][j] == country_list[n]) {
                if (!check_color(i, j)) {//檢查顏色是否有干涉
                    return dfs(n);
                }
            }
        }
    }

    if(n == country_list.size()-1){//所有國家都填好色了
        return true;
    }else{
        return dfs(n+1);
    }
}