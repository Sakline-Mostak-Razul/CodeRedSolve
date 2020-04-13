// Status : ACCEPTED
// Source : https://www.spoj.com/problems/GOT
// DS & ALGO : LCA & DP

#include "bits/stdc++.h"
using namespace std;
using vi = vector<int>;
using pii = pair<int,int>;

#define ff first 
#define se second 
#define pb push_back 
const int lgn = 18;
const int mx = 1e5+5;

int n, q, a[mx];
int ans[mx], depth[mx];
int par[mx][18], cnt[mx];

vector<pii> vc[mx];
vector<int> adj[mx];

void dfs(int u, int p, int h){
    par[u][0] = p; depth[u] = h;

    for(int j=1; j<lgn; j++)
        par[u][j] = par[ par[u][j-1] ][j-1];

    for(int &v : adj[u])
        if( v != p )
            dfs(v, u, h+1);
}

int LCA(int u, int v){
    if(depth[u] < depth[v]) swap(u, v);
    for(int i=lgn-1; ~i; i--)
        if(depth[ par[u][i] ] >= depth[v])
            u = par[u][i];
    if(u == v) return u;

    for(int i=lgn-1; ~i; i--)
        if(par[u][i] != par[v][i])
            u = par[u][i], v = par[v][i];
    return par[u][0];
}

void dfs(int u, int p){
    cnt[ a[u] ]++;
    for(pii &pi : vc[u])
        if(pi.ff < 0) ans[-pi.ff] -= cnt[pi.se];
        else ans[pi.ff] += cnt[pi.se];
    for(int &v : adj[u])
        if( v != p )
            dfs(v, u);
    cnt[ a[u] ]--;
}

int main(int argc, const char** argv) {

    ios_base :: sync_with_stdio( false );
    cin.tie( nullptr );

    int u, v, x;
    while(cin >> n >> q){
        for(int i=1; i<=n; i++) cin >> a[i];
        for(int i=1; i<n; i++){
            cin >> u >> v;
            adj[u].pb(v);
            adj[v].pb(u);
        }
        
        dfs(1, 0, 0);

        for(int i=0; i<q; i++){
            cin >> u >> v >> x;

            ans[i] = 0;
            int lca = LCA(u, v);

            vc[u].pb({i, x});
            vc[v].pb({i, x});
            vc[lca].pb({-i, x});
            vc[ par[lca][0] ].pb({-i, x});
        }

        dfs(1, 0);
        for(int i=0; i<q; i++)
            cout << (ans[i] ? "Find\n" : "NotFind\n");
        cout << "\n";

        for(int i=0; i<=n; i++)
            adj[i].clear(), vc[i].clear();
    }

    return 0;
}