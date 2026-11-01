#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct State {
    long long time;
    int stop;
    State(long long t, int s) : time(t), stop(s) {}
    
    inline bool operator>(const State& other) const {
        return time > other.time;
    }
};

struct BusRoute {
    int f, d, l;
    int route_id;
    int stop_index;
    long long prefix_time;

    BusRoute(int f, int d, int l, int rid, int idx, long long ptime)
        : f(f), d(d), l(l), route_id(rid), stop_index(idx), prefix_time(ptime) {}
};


int main() {
    int n, m, t0, S, T;
    cin >> n >> m >> t0 >> S >> T;

    vector<vector<int> > all_stops(m);
    vector<vector<int> > all_times(m);
    vector<vector<long long> > prefix_times(m);
    unordered_map<int, vector<BusRoute> > stop_to_routes;
    
    for (int i = 0; i < m; ++i) {
        int l, f, d;
        cin >> l >> f >> d;
        all_stops[i].resize(l);
        for (int j = 0; j < l; ++j) {
            cin >> all_stops[i][j];
        }
        
        all_times[i].resize(l-1);
        prefix_times[i].resize(l);
        prefix_times[i][0] = 0;
        
        for (int j = 0; j < l - 1; ++j) {
            cin >> all_times[i][j];
            prefix_times[i][j + 1] = prefix_times[i][j] + all_times[i][j];
        }
        
        for (int j = 0; j < l; ++j) {
            stop_to_routes[all_stops[i][j]].emplace_back(f, d, l, i, j, prefix_times[i][j]);
        }
    }

    vector<long long> dis(n + 1, LLONG_MAX);
    dis[S] = t0;

    priority_queue<State, vector<State>, greater<State> > pq;
    pq.push(State(t0, S));

    while (!pq.empty()) {
        long long current_time = pq.top().time;
        int current_stop = pq.top().stop;
        pq.pop();

        if (current_time > dis[current_stop]) continue;
        if (current_stop == T) break; 

        for (const auto& route : stop_to_routes[current_stop]) {
            const vector<int>& stops = all_stops[route.route_id];
            int i = route.stop_index;

            long long bus_time = route.f;
            for (int j = 0; j < i; ++j) {
                bus_time += all_times[route.route_id][j];
            }

            if (current_time > bus_time) {
                bus_time += ((current_time - bus_time + route.d - 1) / route.d) * route.d;
            }

            if (bus_time >= dis[T]) continue;

            long long base_time = bus_time - route.prefix_time;
            
            for (int j = i; j < route.l - 1; ++j) {
                long long travel_time = base_time + prefix_times[route.route_id][j + 1];
                int next_stop = stops[j + 1];
                
                if (travel_time >= dis[next_stop]) break;
                dis[next_stop] = travel_time;
                pq.push(State(travel_time, next_stop));
            }
        }
    }

    if (dis[T] == LLONG_MAX) {
        cout << -1 << endl;
    } else {
        cout << dis[T] << endl;
    }

    return 0;
}