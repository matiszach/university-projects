#include <vector>
#include <climits>
#include <memory>

// node struct

struct node{
    int val;
    std::shared_ptr<node> left, right;
};

//node creation

std::shared_ptr<node> make_node(){
    std::shared_ptr<node> temp = std::make_shared<node>();
    temp->left = temp->right = NULL;
    return temp;
}

//vector of tree roots

std::vector<std::shared_ptr<node>> versions;

//persistent tree functions | update & query

void update(std::shared_ptr<node> v, std::shared_ptr<node> pre, int start, int end, int pos, int val){
    if(start == end){
        v->val = std::max(v->val, val);
        return;
    }
    int mid = start + ((uint32_t)end - start) / 2;
    if(pos <= mid){
        v->left = make_node();
        v->right = (pre ? pre->right : NULL);
        update(v->left, (pre ? pre->left : NULL), start, mid, pos, val);
    }
    else{
        v->left = (pre ? pre->left : NULL);
        v->right = make_node();
        update(v->right, (pre ? pre->right : NULL), mid + 1, end, pos, val);
    }
    v->val = std::max((v->left ? v->left->val : -1), (v->right ? v->right->val : -1));
}

int query(std::shared_ptr<node> v, int start, int end, int l, int r){
    if(!v || start > r || end < l || start > end) return -1;
    if(start >= l && end <= r) return v->val;
    int mid = start + ((uint32_t)end - start) / 2;
    return std::max(query(v->left, start, mid, l, r), query(v->right, mid + 1, end, l, r));
}

// library functions

void pushBack(int v){
    versions.push_back(make_node());
    update(versions.back(), versions[versions.size() - 2], INT_MIN, INT_MAX, v, (int)versions.size() - 2);
}

void init(const std::vector<int> &x){
    versions = {make_node()};
    for(int v:x) pushBack(v);
}

int prevInRange(int i, int lo, int hi){
    int res = query(versions[i + 1], INT_MIN, INT_MAX, lo, hi);
    return res;
}

void done(){
    versions.clear();
}
