#include <iostream>
#include <algorithm>
using namespace std;

int arry[] = {1, 4, 8, 3, 5, 2, 7, 6};
int n = 8;
int main() {
    for(int i = n-1; i > 0; i--){
        int max_t = arry[0];
        bool sorted = true;
        for(int j = 1; j <= i; j++){
            if(arry[j] < max_t){
                arry[j-1] = arry[j];
                sorted = false;
            }
            else{
                arry[j-1] = max_t;
                max_t = arry[j];
            }
        }
        arry[i] = max_t;
        for (int k = 0; k < n; k++) cout << arry[k] << " ";
        if(sorted) break;
        cout << endl;
    }
    return 0;
}