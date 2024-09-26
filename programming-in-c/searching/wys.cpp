#include <unordered_map>
#include <vector>
#include <cstdint>
#include "wys.h"

// custom hash function for vectors

struct vec_to_int{
    uint64_t operator()(const std::vector<int>& key) const{
        uint64_t res = 0;
        for(uint32_t i = 0; i < key.size(); ++i){
            res += key[i] * ((uint64_t)1 << (i * 3));
        }
        return res;
    }
};

// unordered_map of game states

std::unordered_map<std::vector<int>, int, vec_to_int> dp;

int n, k, g;

// precalculating the dp table

int prepare(std::vector<int> x){
    if(dp.count(x)) return dp[x];
    dp[x] = n * 5;
    for(int i = 1; i < n; ++i){
        std::vector<int> less = x, geq = x;
        for(int j = 0; j < i; ++j) less[j] = std::min(less[j] + 1, k + 1);
        for(int j = i; j < n; ++j) geq[j] = std::min(geq[j] + 1, k + 1);
        dp[x] = std::min(dp[x], std::max(prepare(less), prepare(geq)) + 1);
    }
    return dp[x];
}

// retrieving result based on the answers from the library and previously calculated dp table

int solve(std::vector<int> x){
    int cnt = 0, pos = 0;
    for(int i = 0; i < n; ++i){
        if(x[i] <= k){
            ++cnt;
            pos = i;
        }
    }
    if(cnt == 1) return pos + 1;
    int best = n * 5, which = 0;
    for(int i = 1; i < n; ++i){
        std::vector<int> less = x, geq = x;
        for(int j = 0; j < i; ++j) less[j] = std::min(less[j] + 1, k + 1);
        for(int j = i; j < n; ++j) geq[j] = std::min(geq[j] + 1, k + 1);
        int tmp = std::max(dp[less], dp[geq]);
        if(tmp < best){
            best = tmp;
            which = i;
        }
    }
    bool ans = mniejszaNiz(which + 1);
    if(!ans){
        for(int i = 0; i < which; ++i) x[i] = std::min(x[i] + 1, k + 1);
    }
    else{
        for(int i = which; i < n; ++i) x[i] = std::min(x[i] + 1, k + 1);
    }
    return solve(x);
}

// executing functions and answering queries

int main()
{
    dajParametry(n, k, g);
    std::vector<int> tmp(n, k + 1);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j <= k; ++j){
            tmp[i] = j;
            dp[tmp] = 1;
        }
        tmp[i] = k + 1;
    }
    tmp = std::vector<int>(n, 0);
    prepare(tmp);
    for(int i = 0; i < g; ++i) odpowiedz(solve(tmp));
}
