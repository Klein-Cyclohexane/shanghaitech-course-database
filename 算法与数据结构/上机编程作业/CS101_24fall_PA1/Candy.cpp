#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class T>

class my_queue {
    int* arr;
    int capacity;
    int fron;
    int rear;
    int count;

public:
    my_queue(int size = 10000000) {
        arr = new int[size];
        capacity = size;
        fron = 0;
        rear = -1;
        count = 0;
    }

    ~my_queue() {
        delete[] arr;
    }

    void push(int item) {
        if (count == capacity) {
            cout << "Queue Overflow" << endl;
            return;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = item;
        count++;
    }

    void pop() {
        if (count == 0) {
            cout << "Queue Underflow" << endl;
            return;
        }
        fron = (fron + 1) % capacity;
        count--;
    }

    const int& front() {
        return arr[fron];
    }

    const int& size() {
        return count;
    }
};

int n, m, p;
int main() {
    cin >> n >> m >> p;
    int jar = 1;
    my_queue<int> q;
    vector<int> ans;
    for (int i = 1; i <= n * m; i++) {
        q.push(i);
    }
    while (q.size() > 0) {
        if (jar == 0) ans.push_back(q.front());
        q.pop();
        for (int i = 0; i < p; i++) {
            q.push(q.front());
            q.pop();
        }
        jar = (jar + 1) % n;
    }
    sort(ans.begin(), ans.end());
    for (size_t i = 0; i < ans.size(); i++) {
        cout << ans[i] << " ";
    }
    return 0;
}