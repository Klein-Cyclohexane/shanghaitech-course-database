#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>

using namespace std;
int max_w;
struct Edge
{
    int u, v;
    long long w;
    bool isSpecial;
    Edge(int _u, int _v, long long _w, int _s) : u(_u), v(_v), w(_w), isSpecial(_s) {}
};

inline bool cmp(const Edge& a, const Edge& b) {
    return a.w < b.w;
}

struct UnionFind
{
    vector<int> parent, rank;
    UnionFind(int n) : parent(n+1), rank(n+1, 0)
    {
        for(int i=0; i<=n; i++) parent[i] = i;
    }
    inline int find(int x)
    {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    }
    inline void unite(int x, int y)
    {
        x = find(x), y = find(y);
        if(x == y) return;
        if(rank[x] < rank[y]) swap(x,y);
        parent[y] = x;
        if(rank[x] == rank[y]) rank[x]++;
        return;
    }
};

struct MST_Result
{
    long long total_weight;
    int special_count;
    MST_Result(long long t, int s) : total_weight(t), special_count(s) {}
};

inline MST_Result runKruskal(int n, const vector<Edge>& normal, const vector<Edge>& special, long long mid)
{
    UnionFind dsu(1e5+5);
    long long totalW = 0;
    int used = 0, spCount = 0;
    size_t i = 0, j = 0;
    while(used < n-1 && (i < normal.size() || j < special.size()))
    {
        long long costN = LLONG_MAX, costS = LLONG_MAX;
        if(i < normal.size()) 
        {
            while (i < normal.size() && dsu.find(normal[i].u) == dsu.find(normal[i].v))
            {
                i++;
            }
            costN = i == normal.size() ? LLONG_MAX : normal[i].w;
        }
        else costN = LLONG_MAX;
        if(j < special.size())
        {
            while (j < special.size() && dsu.find(special[j].u) == dsu.find(special[j].v))
            {
                j++;
            }
            costS = j == special.size() ? LLONG_MAX : special[j].w + mid;
        }
        else costS = LLONG_MAX;
        int u, v;
        long long realW;
        if (costN <= costS)
        {
            u = normal[i].u;
            v = normal[i].v;
            realW = normal[i].w;
            i++;
        }
        else
        {
            u = special[j].u;
            v = special[j].v;
            realW = special[j].w;
            j++;
        }
        dsu.unite(u, v);
        totalW += realW;
        used++;
        if(costS < costN) spCount++;
    }
    return MST_Result(totalW, spCount);
}

inline MST_Result runKruskal2(int n, const vector<Edge>& normal, const vector<Edge>& special, long long mid)
{
    UnionFind dsu(1e5+5);
    long long totalW = 0;
    int used = 0, spCount = 0;
    size_t i = 0, j = 0;
    while(used < n-1 && (i < normal.size() || j < special.size()))
    {
        long long costN = LLONG_MAX, costS = LLONG_MAX;
        if(i < normal.size()) 
        {
            while (i < normal.size() && dsu.find(normal[i].u) == dsu.find(normal[i].v))
            {
                i++;
            }
            costN = i == normal.size() ? LLONG_MAX : normal[i].w;
        }
        else costN = LLONG_MAX;
        if(j < special.size())
        {
            while (j < special.size() && dsu.find(special[j].u) == dsu.find(special[j].v))
            {
                j++;
            }
            costS = j == special.size() ? LLONG_MAX : special[j].w + mid;
        }
        else costS = LLONG_MAX;
        int u, v;
        long long realW;
        if (costN < costS)
        {
            u = normal[i].u;
            v = normal[i].v;
            realW = normal[i].w;
            i++;
        }
        else
        {
            u = special[j].u;
            v = special[j].v;
            realW = special[j].w;
            j++;
        }
        dsu.unite(u, v);
        totalW += realW;
        used++;
        if(costS <= costN) spCount++;
    }
    return MST_Result(totalW, spCount);
}

long long findSpecialMST(int n, int k, const vector<Edge> &normal, const vector<Edge> &special)
{
    long long ans = 1e18;
    long long left = -2*max_w, right = 2*max_w;
    while(left <= right)
    {
        long long mid = (left + right) / 2;
        MST_Result mst = runKruskal(n, normal, special, mid);
        if (mst.special_count == k)
        {
            ans = std::min(ans, mst.total_weight);
            break;
        } else if (mst.special_count > k) {
            left = mid + 1;
        } else {
            MST_Result mst2 = runKruskal2(n, normal, special, mid);
            if(min(mst.special_count, mst2.special_count) <= k && max(mst.special_count, mst2.special_count) >= k)
            {
                ans = std::min(ans, mst.total_weight - (k - mst.special_count) * mid);
                break;
            }
            else if(min(mst.special_count, mst2.special_count) > k)
            {
                left = mid + 1;
            }
            else if(max(mst.special_count, mst2.special_count) < k)
            {
                right = mid - 1;
            }
        }
    }
    return ans;
}

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);
    
    int n, m, k;
    cin >> n >> m >> k;
    vector<Edge> normal, special;
    max_w = 0;
    for (int i = 0; i < m; i++)
    {
        int u,v,w,s;
        cin >> u >> v >> w >> s;
        max_w = max(max_w, w);
        if(s == 1) special.push_back(Edge(u, v, w, s));
        else normal.push_back(Edge(u, v, w, s));
    }
    sort(normal.begin(), normal.end(), cmp);
    sort(special.begin(), special.end(), cmp);

    
    cout << findSpecialMST(n, k, normal, special);
    return 0;
}