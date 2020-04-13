// Verdict      : ACCEPTED
// Source       : https://www.codechef.com/problems/RRTREE
// Solution     : https://www.codechef.com/viewsolution/31595366
// DS & Algo    : LCA 


#include <bits/stdc++.h>
using namespace std;

const int lgn = 18;
const int mx = 1e5+5;

int ans[mx];
int depth[mx];
int par[mx][lgn];

int lca(int u, int v){
    if(depth[u] < depth[v]) swap(u, v);
    for(int i=lgn-1; i>=0; i--)
        if(depth[u]-(1<<i) >= depth[v])
            u = par[u][i];
    if(u == v) return u;
    for(int i=lgn-1; i>=0; i--)
        if(~par[u][i] && par[u][i]!=par[v][i])
            u = par[u][i], v = par[v][i];
    return par[u][0];
}

int dist(int u, int v){
    int lc = lca(u, v);
    return depth[u]+depth[v]-(depth[lc]<<1);
}

int main(int argc, const char** argv) {

    ios_base :: sync_with_stdio( false );
    cin.tie( nullptr );

    int n, d1, d2, u;
    int tc; cin >> tc;
    while(tc--){
        memset(par, -1, sizeof par);
        
        cin >> n;
        d1 = d2 = 1;
        depth[1] = 1;

        for(int i=2; i<=n; i++){
            cin >> u;
            par[i][0] = u;
            depth[i] = depth[u] + 1;

            for(int j=1; j<lgn; j++)
                if(~par[i][j-1])
                    par[i][j] = par[ par[i][j-1] ][j-1];
            
            int preAns = ans[i-1];
            int ans1 = dist(d1, i);
            int ans2 = dist(d2, i);

            if(ans1 > ans2){
                if(ans1 > preAns)
                    ans[i] = ans1, d2 = i;
                else ans[i] = preAns;
            }
            else if(ans2 > preAns)
                ans[i] = ans2, d1 = i;
            else ans[i] = preAns;
        }

        for(int i=2; i<=n; i++)
            cout << ans[i] << "\n";
    }

    return 0;
}