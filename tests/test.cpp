#include "../randlib.h"
#include<bits/stdc++.h>
// #define int long long
using namespace std;

using namespace RandLib;

signed main(){
    ios_base::sync_with_stdio(0); cin.tie(0);

    NumberGen NumGen;
    GraphGen Ggen;

    int n = 100, q = 100;
    cout << n << ' ' << q << '\n';

    for(int i = 1; i <= n; ++i){
        cout << NumGen.Rand(1, 2) << ' ';
    }
    cout << '\n';

    for(const pair<int, int>& p: Ggen.randomTree(n)){
        cout << p.first << ' ' << p.second << '\n';
    }
    for(int i = 1; i <= q; ++i){
        int u = NumGen.Rand(1, n), v = NumGen.Rand(1, n);
        cout << u << ' ' << v << '\n';
    }

    return 0;
}
