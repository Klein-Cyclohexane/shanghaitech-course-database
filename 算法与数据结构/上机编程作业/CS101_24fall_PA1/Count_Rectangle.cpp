#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

struct Point {
    int x, y;
    int id;
};

inline bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

inline bool compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

long long countRectangles(std::vector<Point>& points, int l, int r) {
    if (r - l <= 1) return 0;
    int m = (l + r) / 2;
    long long count = countRectangles(points, l, m) + countRectangles(points, m, r);

    std::vector<Point> rights(points.begin() + m, points.begin() + r);
    std::vector<Point> lefts(points.begin() + l, points.begin() + m);
    sort(lefts.begin(), lefts.end(), compareY);
    sort(rights.begin(), rights.end(), compareY);

    int nearest[100005];
    std::stack<Point> st;
    for (size_t i = 0; i < rights.size(); ++i) {
        rights[i].id = i;
        while (!st.empty() && st.top().x > rights[i].x) {
            st.pop();
        }
        nearest[i] = st.empty() ? -1 : st.top().id;
        st.push(rights[i]);
    }
    int sumright[100005];
    
    std::vector<Point> left_point;
    size_t j = 0;
    for (size_t i = 0; i < rights.size(); ++i)
    {
        while(j < lefts.size() && lefts[j].y < rights[i].y)
        {
            while (!left_point.empty() && lefts[j].x > left_point.back().x) {
                left_point.pop_back();
            }
            left_point.push_back(lefts[j]);
            j++;
        }
        sumright[i] = left_point.size();
        if (nearest[i] != -1) {
            count += left_point.end() - std::upper_bound(left_point.begin(), left_point.end(), rights[nearest[i]], compareY) + 1;
        }
        else
        {
            count += sumright[i];
        }
    }
    return count;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }

    sort(points.begin(), points.end(), compareX);
    std::cout << countRectangles(points, 0, n);
    return 0;
}