#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_N = 100005;
const int MAX_MONSTER_ID = 400005;

int n;
int res = 0;
int event_index = 0;
int cnt1 = 0, cnt2 = 0;
int k;
int vis[MAX_MONSTER_ID + 1] = {0};
int login_index[MAX_N], logout_index[MAX_N];
int c;
int ans[MAX_N];
vector<int> res_values;
vector<int> monsters[MAX_N];

struct SegmentTree {
    int size;
    vector<int> max_value;

    void init(int n) {
        size = 1;
        while (size < n) size <<= 1;
        max_value.assign(size * 2, 0);
    }

    void build(const vector<int>& data) {
        for (size_t i = 0; i < data.size(); i++) {
            max_value[size + i] = data[i];
        }
        for (int i = size - 1; i > 0; --i) {
            max_value[i] = max(max_value[i << 1], max_value[i << 1 | 1]);
        }
    }

    int query(int l, int r) {
        l += size;
        r += size;
        int res = 0;
        while (l <= r) {
            if (l % 2 == 1) res = max(res, max_value[l++]);
            if (r % 2 == 0) res = max(res, max_value[r--]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

inline int readInt() {
    int x = 0, c = getchar_unlocked();
    while (c < '0' || c > '9') c = getchar_unlocked();
    for (; c >= '0' && c <= '9'; c = getchar_unlocked())
        x = x * 10 + c - '0';
    return x;
}

int main() {
    n = readInt();
    for (int i = 0; i < 2 * n; i++) {
        k = readInt();
        if (k == 1) {
            // Player login
            login_index[cnt1] = event_index;
            c = readInt();
            monsters[cnt1].reserve(c);
            for (int j = 0; j < c; j++) {
                int x = readInt();
                monsters[cnt1].push_back(x);
                if (vis[x] == 0)
                    res++;
                vis[x]++;
            }
            cnt1++;
        } else {
            // Player logout
            logout_index[cnt2] = event_index;
            for (int x : monsters[cnt2]) {
                vis[x]--;
                if (vis[x] == 0)
                    res--;
            }
            cnt2++;
        }
        res_values.push_back(res);
        event_index++;
    }

    SegmentTree st;
    st.init(event_index);
    st.build(res_values);

    for (int i = 0; i < n; i++) {
        ans[i] = st.query(login_index[i], logout_index[i]);
    }

    for (int i = 0; i < n; i++) {
        printf("%d\n", ans[i]);
    }

    return 0;
}