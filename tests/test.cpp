#include "randlib.h"
#include<bits/stdc++.h>
// #define int long long
using namespace std;

using namespace RandLib;

signed main(){
    ios_base::sync_with_stdio(0); cin.tie(0);

    NumberGen NumGen;
    cout << NumGen.Rand(1, 10) << '\n';

    return 0;
}
