#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

using std::vector;

int n, W, k;  // n: number of equipment, W: capacity, k: mission number
struct eqp {
    int l, w, v, ratio;  // l: limit, w: weight, v: value
    eqp(int _l, int _w, int _v)
        : l(_l), w(_w), v(_v), ratio(_v / _w) {}
};
inline bool cmp(const eqp& a, const eqp& b) {
    return a.ratio > b.ratio;
}
vector<eqp> equipment;

/*
You can share a public variable(function) sets since only 1 mission will be executed once.
*/

// MISSION1: all equipment with l = -1
int Mission1() {
    vector<vector<int> > dp(n + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        const auto& item = equipment[i-1];
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i-1][w];
            
            if (w >= item.w) {
                dp[i][w] = std::max(dp[i][w],
                    dp[i][w - item.w] + item.v);
            }
        }
    }
    return dp[n][W];
}

// MISSION2: all equipment with l = 1
int Mission2() {
    vector<vector<int> > dp(n + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i-1][w];

            if (w >= equipment[i-1].w && equipment[i-1].l == 1) {
                dp[i][w] = std::max(dp[i][w], 
                    dp[i-1][w - equipment[i-1].w] + equipment[i-1].v);
            }
        }
    }
    return dp[n][W];
}

// MISSION3: Liquid tanks: Can bring any fractional unit of liquid, w|v
int Mission3() {
    int ans = 0;
    std::sort(equipment.begin(), equipment.end(), cmp);
    for (int i = 1; i <= n; i++)
    {
        //Here we use greedy algorithm to solve the fractional knapsack problem
        for (int j = 0; j < equipment[i-1].l; j++) {
            if (W >= equipment[i-1].w) {
                W -= equipment[i-1].w;
                ans += equipment[i-1].v;
            } else {
                ans += W * equipment[i-1].ratio;
                W = 0;
                break;
            }
        }
    }
    return ans;
}

// Input Part, you can modify this.
void getInput() {
    scanf("%d%d%d", &k, &n, &W);
    for (int i = 1; i <= n; ++i) {
        int l, w, v;
        scanf("%d%d%d", &l, &w, &v);
        equipment.emplace_back(l, w, v);
    }
}

// You'd better not to modify things below.
int (*MISSION_FUNC[])() = {nullptr, Mission1, Mission2, Mission3};

int main() {
    getInput();
    int ans = MISSION_FUNC[k]();
    std::cout << ans;
    return 0;
}