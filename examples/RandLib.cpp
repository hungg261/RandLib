#include<bits/stdc++.h>
// #define int long long
using namespace std;

namespace RandLib{
long long __seed__ = chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 __rng__(__seed__);

struct NumberGen{
    mt19937_64 rng = RandLib::__rng__;
    long long seed = RandLib::__seed__;

    NumberGen() = default;
    NumberGen(long long _seed) : seed(_seed){ rng.seed(_seed); }

    bool operator == (const NumberGen &other) const{ return this->seed == other.seed; }

    void Reset(){ rng.seed(seed = chrono::steady_clock::now().time_since_epoch().count()); }
    void setSeed(long long _seed){ seed = _seed; rng.seed(_seed); }

    //////////////////////////////////////////////////////////

    /// (Unsafe) Random integer in [L; R]
    template <typename U = int>
    typename enable_if<is_integral<U>::value, U>::type
    rand(U L, U R){
        assert(L <= R);
        return L + 1LL * rng() % (R - L + 1);
    }

    /// Random integer in [L; R]
    template <typename U = int>
    typename enable_if<is_integral<U>::value, U>::type
    Rand(U L, U R){
        assert(L <= R);
        return uniform_int_distribution<U>(L, R)(rng);
    }

    /// Returns index chosen with probability proportional to weight[i]
    template <typename U = int>
    typename enable_if<is_integral<U>::value, int>::type
    WRand(const vector<U>& weight){
        long long total = 0;
        for (U w : weight) {
            assert(w >= 0);
            total += w;
        }
        assert(total > 0);

        long long r = Rand<U>(1, total);

        long long pref = 0;
        for (int i = 0; i < (int)weight.size(); i++){
            pref += weight[i];
            if (r <= pref)
                return i;
        }
        return -1;
    }

    /// Random real in [L; R]
    template <typename U = double>
    typename enable_if<is_floating_point<U>::value, U>::type
    Rand(U L, U R){
        assert(L <= R);
        return uniform_real_distribution<U>(L, R)(rng);
    }

    /// Random between 0 and 1
    bool RandBool(){ return Rand<int>(0, 1) == 1; }

    /// Random real between [0; 1]
    template <typename U = double>
    typename enable_if<is_floating_point<U>::value, U>::type
    Rand(){ return Rand<U>(0, 1); }

    /// Random __int128 in [L; R]
    __int128 Rand128(__int128 L, __int128 R){
        unsigned __int128 range = R - L + 1;
        unsigned __int128 x = (((unsigned __int128)rng()) << 64) | rng();
        return L + (x % range);
    }


    string __BigRand__(int idx, int largerL, int smallerR, const string& Lbound, const string& Rbound){
        if(idx < 0) return "";

        char L = largerL ? '0' : Lbound[idx];
        char R = smallerR ? '9' : Rbound[idx];

        char digit = (char)Rand(L, R);
        return string(1, digit) + __BigRand__(idx - 1,
                                              largerL || (digit > L),
                                              smallerR || (digit < R),
                                              Lbound, Rbound);
    }

    /// Random big integer in [L; R]
    string BigRand(string L, string R){
        reverse(begin(L), end(L));
        reverse(begin(R), end(R));
        while(L.size() < R.size()) L.push_back('0');

        int len = R.size();
        string res = __BigRand__(len - 1, 0, 0, L, R);

        for(int i = 0; i < len; ++i){
            if(res[i] != '0'){
                res = res.substr(i);
                break;
            }
        }

        return res;
    }
};

struct StringGen{
    NumberGen NumGen;

    string LOWER_CHAR = "abcdefghijklmnopqrstuvwxyz";
    string UPPER_CHAR = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string NUM_CHAR = "0123456789";
    string SPECIAL_CHAR = "`~!@#$%^&*()_+{}|:\"<>?-=[]\\;',./";
    string ALL_CHAR = LOWER_CHAR + UPPER_CHAR + NUM_CHAR + SPECIAL_CHAR;

    bool operator == (const StringGen &other) const{ return this->NumGen == other.NumGen; }

    void Reset(){ NumGen.Reset(); }
    void setSeed(long long _seed){ NumGen.setSeed(_seed); }

    //////////////////////////////////////////////////////////

    /// Random character in a string
    char RandChar(const string& range){ return range[NumGen.Rand(0, (int) range.size() - 1)]; }
    char RandChoice(const string& object){ return RandChar(object); }

    /// Random string with custom characters
    string RandString(int len, const string& range){
        string res;
        while(len--) res = res + RandChar(range);
        return res;
    }

    /// Shuffle string
    string Shuffle(const string& original){
        string res = original;
        shuffle(begin(res), end(res), NumGen.rng);
        return res;
    }
};

struct VectorGen{
    NumberGen NumGen;

    VectorGen() = default;
    VectorGen(long long _seed){ NumGen.setSeed(_seed); }

    bool operator == (const VectorGen &other) const{ return this->NumGen == other.NumGen; }

    void Reset(){ NumGen.Reset(); }
    void setSeed(long long _seed){ NumGen.setSeed(_seed); }

    //////////////////////////////////////////////////////////

    /// Pick a random element
    template<typename T>
    T Choice(const vector<T>& cont){
        return cont[NumGen.Rand(0, (int) cont.size() - 1)];
    }

    /// Pick a random element from index L to R
    template<typename T>
    T Choice(const vector<T>& cont, int L, int R){
        return cont[NumGen.Rand(L, R)];
    }

    /// Shuffle vector
    template<typename T>
    vector<T> Shuffle(const vector<T>& cont){
        vector<T> res = cont;
        shuffle(res.begin(), res.end(), NumGen.rng);
        return res;
    }

    /// Random normal vector
    template<typename T>
    vector<T> GenArr(int length, T L, T R){
        vector<T> res(length);
        for(int i = 0; i < length; ++i){
            res[i] = NumGen.Rand<T>(L, R);
        }
        return res;
    }
};

struct GraphGen {
    NumberGen NumGen;

    GraphGen() = default;
    GraphGen(NumberGen& _rng) : NumGen(_rng){}

    void Reset(){ NumGen.Reset(); }

    // ================= GRAPH =================
    vector<pair<int,int>> randomGraph(int n, int m,
                                      bool directed = false,
                                      bool allowSelfLoop = false){
        long long maxEdges;
        if(directed){
            maxEdges = 1LL * n * (n - 1);
            if(allowSelfLoop) maxEdges += n;
        }
        else{
            maxEdges = 1LL * n * (n - 1) / 2;
            if(allowSelfLoop) maxEdges += n;
        }
        assert(m <= maxEdges);

        set<pair<int,int>> edges;
        while ((int)edges.size() < m){
            int u = NumGen.Rand<int>(1, n);
            int v = NumGen.Rand<int>(1, n);

            if(!allowSelfLoop && u == v) continue;
            if(!directed && u > v) swap(u, v);

            edges.insert({u, v});
        }
        return vector<pair<int,int>>(edges.begin(), edges.end());
    }

    // ================= DAG =================
    vector<pair<int, int>> randomDAG(int n, int m){
        assert(m <= 1LL * n * (n - 1) / 2);

        vector<int> topo(n);
        iota(topo.begin(), topo.end(), 1);
        shuffle(topo.begin(), topo.end(), NumGen.rng);

        set<pair<int, int>> edges;
        while ((int)edges.size() < m){
            int i = NumGen.Rand<int>(0, n - 1);
            int j = NumGen.Rand<int>(0, n - 1);
            if(i >= j) continue;
            edges.insert({topo[i], topo[j]});
        }
        return vector<pair<int, int>>(edges.begin(), edges.end());
    }

    // ================= TREE =================
    vector<pair<int, int>> randomTree(int n){
        vector<int> nodes(n);
        iota(nodes.begin(), nodes.end(), 1);
        shuffle(nodes.begin(), nodes.end(), NumGen.rng);

        vector<pair<int, int>> edges;
        for (int i = 1; i < n; ++i){
            int parent = NumGen.Rand<int>(0, i - 1);
            edges.push_back({nodes[parent], nodes[i]});
        }
        shuffle(edges.begin(), edges.end(), NumGen.rng);
        return edges;
    }

    // ================= PRINT =================
    void printEdges(const vector<pair<int, int>>& edges){
        for (auto &e : edges)
            cout << e.first << " " << e.second << "\n";
    }
};

}
using namespace RandLib;

signed main(){
    ios_base::sync_with_stdio(0); cin.tie(0);


    return 0;
}
