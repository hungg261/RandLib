#include "../RandLib.h"
#include <bits/stdc++.h>
using namespace std;
using namespace RandLib;

signed main() {
    ios_base::sync_with_stdio(0); cin.tie(0);

    // ---------------- NumberGen example ----------------
    NumberGen rng;
    rng.setSeed(12345);  // optional fixed seed for reproducibility

    cout << "Random integer [1, 10]: " << rng.Rand(1, 10) << "\n";
    cout << "Random double [0, 1]: " << rng.Rand<double>() << "\n";
    cout << "Random boolean: " << rng.RandBool() << "\n";

    // ---------------- StringGen example ----------------
    StringGen strGen;
    cout << "Random string of length 8 (letters only): "
         << strGen.RandString(8, strGen.LOWER_CHAR + strGen.UPPER_CHAR) << "\n";

    // ---------------- VectorGen example ----------------
    VectorGen vecGen;
    vector<int> v = {10, 20, 30, 40, 50};
    cout << "Random choice from vector: " << vecGen.Choice(v) << "\n";

    vector<int> shuffled = vecGen.Shuffle(v);
    cout << "Shuffled vector: ";
    for (int x : shuffled) cout << x << " ";
    cout << "\n";

    // ---------------- GraphGen example ----------------
    GraphGen graphGen(rng);
    int n = 5, m = 6;
    auto edges = graphGen.randomGraph(n, m);
    cout << "Random undirected graph with " << n << " nodes and " << m << " edges:\n";
    graphGen.printEdges(edges);

    // ---------------- Random Tree example ----------------
    auto treeEdges = graphGen.randomTree(5);
    cout << "Random tree with 5 nodes:\n";
    graphGen.printEdges(treeEdges);

    return 0;
}
