#include <algorithm>
#include <cstdio>
#include <vector>

std::vector<int> values;
// The struct for nodes in AVL.
struct Node {
    int size, height;
    int s, sum;
    Node *parent, *child[2];
    ~Node();
} dummy{0, -1, 0, 0, nullptr, nullptr, nullptr};

// We can handle fewer boundary conditions if we implement AVL with a dummy node.
Node* dummyptr = &dummy;

// Node destructor.
Node::~Node() {
    if (this == dummyptr)
        return;
    if (child[0] != dummyptr)
        delete child[0];
    if (child[1] != dummyptr)
        delete child[1];
}

// The struct for an AVL tree.
struct Tree {
    Node* root;
    ~Tree() {
        if (root != dummyptr)
            delete root;
    }
} tree{dummyptr};

// Update size, height and sum of node u.
void update(Node* u) {
    u->size = 1 + u->child[0]->size + u->child[1]->size;
    u->height = 1 + std::max(u->child[0]->height, u->child[1]->height);
    u->sum = u->s + u->child[0]->sum + u->child[1]->sum;
}

// Perform a single rotation on node u.
void rotate(Node* u) {
    Node *p = u->parent;
    int d = u == p->child[1];
    p->child[d] = u->child[!d];
    p->child[d]->parent = p;
    u->child[!d] = p;
    u->parent = p->parent;
    if (p->parent != dummyptr)
        p->parent->child[p == p->parent->child[1]] = u;
    p->parent = u;
    update(p);
    update(u);
    if (p == tree.root)
        tree.root = u;
}

// Check and correct imbalances from u to root.
// If an imbalance is Case 1 (LL or RR), Perform the single rotation once.
// If an imbalance is Case 2 (LR or RL), Perform the single rotation twice.
// If an imbalance is Case 3 (L- or R-), Perform the single rotation once.
void update_and_balance(Node* u) {
    while (u != dummyptr) {
        update(u);
        if (std::abs(u->child[0]->height - u->child[1]->height) <= 1) {
            u = u->parent;
            continue;
        }
        int d = u->child[0]->height < u->child[1]->height;
        if (u->child[d]->child[d]->height >= u->child[d]->child[!d]->height) {
            rotate(u->child[d]);
        } else {
            rotate(u->child[d]->child[!d]);
            rotate(u->child[d]);
        }
    }
}

// Subtask 1: Insert x into the AVL, and maintain the balance.
// Insertion is stable. In other words, the last inserted node should be the last node in in-order traversal among those who have the same value.
void insert(int x) {
    // Your code here
    Node *u = tree.root, *p = dummyptr;
    while (u != dummyptr) {
        p = u;
        if (x < u->s) u = u->child[0];
        else if (x >= u->s) u = u->child[1];
    }
    u = new Node{1, 0, x, x, p, dummyptr, dummyptr};
    if (p == dummyptr) tree.root = u;
    else p->child[x >= p->s] = u;
    update_and_balance(p);
}

// Subtask 2: Find x in the AVL.
// If x not found, return dummyptr.
// If there are multiple nodes with node.s==x, return the node with the smallest depth.
Node* find(int x) {
    // Your code here
    Node *u = tree.root;
    while (u != dummyptr) {
        if (x < u->s) u = u->child[0];
        else if (x > u->s) u = u->child[1];
        else break;
    }
    return u;
}

// Subtask 3: Erase x from the AVL, and maintain the balance.
// If x not found, just do nothing and return.
// If there are multiple nodes with node.s==x, erase the node with the smallest depth.
// If the erased node has two children, replace it with the first node of in-order traversal of its right subtree.
void erase(int x) {
    // Your code here
    Node *u = find(x);
    if (u == dummyptr) return;
    Node *p = u->parent;
    if (u->child[0] == dummyptr) {
        if (u->child[1] != dummyptr) {
            u->child[1]->parent = p;
            if (p == dummyptr) tree.root = u->child[1];
            else p->child[u == p->child[1]] = u->child[1];
            update_and_balance(p);
            u -> child[0] = u->child[1] = dummyptr;
            delete u;
        } else {
            if (p == dummyptr) tree.root = dummyptr;
            else p->child[u == p->child[1]] = dummyptr;
            update_and_balance(p);
            u -> child[0] = u->child[1] = dummyptr;
            delete u;
        }
    } else {
        if (u->child[1] == dummyptr) {
            u->child[0]->parent = p;
            if (p == dummyptr) tree.root = u->child[0];
            else p->child[u == p->child[1]] = u->child[0];
            update_and_balance(p);
            u -> child[0] = u->child[1] = dummyptr;
            delete u;
        } else {
            Node *v = u->child[1];
            while (v->child[0] != dummyptr) v = v->child[0];
            u->s = v->s;
            p = v->parent;
            if (v->child[1] != dummyptr) {
                v->child[1]->parent = p;
                if (p == dummyptr) tree.root = v->child[1];
                else p->child[v == p->child[1]] = v->child[1];
                update_and_balance(p);
            } else {
                if (p == dummyptr) tree.root = dummyptr;
                else p->child[v == p->child[1]] = dummyptr;
                update_and_balance(p);
            }
            v -> child[0] = v->child[1] = dummyptr;
            delete v;
        }
    }
}

// Subtask 4: Query the k-th node in the AVL.
int kth(int k) {
    // Your code here
    Node *u = tree.root;
    while (u != dummyptr) {
        if (k <= u->child[0]->size) u = u->child[0];
        else if (k == u->child[0]->size + 1) break;
        else {
            k -= u->child[0]->size + 1;
            u = u->child[1];
        }
    }
    return u->s;
}

void storeInOrder(Node* root, std::vector<int>& values) {
    if (root == dummyptr) {
        return;
    }
    storeInOrder(root->child[0], values); // 访问左子树
    values.push_back(root->s);            // 存储当前节点的值
    storeInOrder(root->child[1], values); // 访问右子树
}

// Subtask 5: Return the minimum ans, as described in problem statement: 
int ans(int k) {
    // Your code here
    //you need to find a position t such that (k \sum_{i=1}^{t} s_i) - \sum_{i=t+1}^{m} s_i >= 0 meanwhile minimize ans = (k \sum_{i=1}^{t} s_i) - \sum_{i=t+1}^{m} s_i . Return ans.
    Node *u = tree.root;
    int total = u->sum;
    int ans = 1e9;
    int cnt = 0;
    values.clear();
    storeInOrder(u, values);
    for (size_t i = 0; i < values.size(); i++) {
        cnt += values[i];
        if (k * cnt - total + cnt >= 0) {
            ans = std::min(ans, k * cnt - total + cnt);
            break;
        }
    }
    return ans;
}

// Print the pre-order of AVL to check whether you maintain the tree structure correctly.
void preorder(Node* u) {
    if (u == dummyptr)
        return;
    printf("%d %d %d %d\n", u->s, u->parent->s, u->child[0]->s, u->child[1]->s);
    preorder(u->child[0]);
    preorder(u->child[1]);
}

// YOU DON'T NEED AND SHOULD NOT MODIFY main()!
int main() {
#ifndef ONLINE_JUDGE
    freopen("avl.in", "r", stdin);
   freopen("avl.out", "w", stdout);
#endif
    int n, opt;
    int x;
    Node* u;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d %d", &opt, &x);
        printf("----------#%d: %d %d----------\n", i, opt, x);
        switch (opt) {
            case 1:
                insert(x);
                preorder(tree.root);
                break;
            case 2:
                u = find(x);
                if (u == dummyptr)
                    printf("Not found!\n");
                else
                    printf("%d\n", u->height);
                break;
            case 3:
                erase(x);
                preorder(tree.root);
                break;
            case 4:
                if (x < 1 or x > tree.root->size)
                    printf("Invalid query!\n");
                else
                    printf("%d\n", kth(x));
                break;
            case 5:
                if (tree.root->size == 0)
                    printf("Invalid query!\n");
                else
                    printf("%d\n", ans(x));
        }
    }
    return 0;
}