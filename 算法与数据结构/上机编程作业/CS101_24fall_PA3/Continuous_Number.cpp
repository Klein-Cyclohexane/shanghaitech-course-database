#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MOD = 1e9+7;
const int MAXK = 1e6+5;

int s, t, k, m;
long long dp1[MAXK+5][10];
long long dp2[MAXK+5][10];

string constructNumber(int start, int length, long long rank) {
    string result;
    
    int currentDigit = start;
    
    for (int i = 1; i <= length; i++) {
        result += to_string(currentDigit);

        int nextDigit = -1;

        if (currentDigit == 0) {
            nextDigit = 1;
            currentDigit = nextDigit;
            continue;
        }
        else if (currentDigit == 9) {
            nextDigit = 8;
            currentDigit = nextDigit;
            continue;
        }

        if (rank <= dp2[length - i][currentDigit - 1]) {
            nextDigit = currentDigit - 1;
        } else {
            rank -= dp2[length - i][currentDigit - 1];
            if (rank <= dp2[length - i][currentDigit + 1]) {
                nextDigit = currentDigit + 1;
            }
        }
        
        currentDigit = nextDigit;
    }
    return result;
}

int main() {
    cin >> s >> t >> k >> m;

    for (int i = 0; i < 10; i++) {
        dp1[1][i] = dp2[1][i] = 0;
    }
    dp1[1][t] = dp2[1][t] = 1;

    for (int i = 2; i <= k; i++) {
        for (int digit = 0; digit <= 9; digit++) {
            if (digit == 0) {
                dp1[i][digit] += dp1[i-1][digit+1];
                dp1[i][digit] %= MOD;
                dp2[i][digit] += dp2[i - 1][digit + 1];
            }
            else if (digit == 9) {
                dp1[i][digit] += dp1[i - 1][digit - 1];
                dp1[i][digit] %= MOD;
                dp2[i][digit] += dp2[i - 1][digit - 1];
            }
            else {
                dp1[i][digit] += dp1[i - 1][digit - 1] + dp1[i - 1][digit + 1];
                dp1[i][digit] %= MOD;
                dp2[i][digit] += dp2[i - 1][digit - 1] + dp2[i - 1][digit + 1];
            }
            dp2[i][digit] = std::min(dp2[i][digit], (long long) MAXK);
        }
    }
    
    cout << dp1[k][s] << "\n";
    
    for (int i = 0; i < m; i++) {
        int query;
        cin >> query;
        if (query > dp2[k][s]) {
            cout << -1 << "\n";
        } else {
            cout << constructNumber(s, k, query) << "\n";
        }
    }
    
    return 0;
}