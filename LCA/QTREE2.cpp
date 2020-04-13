// Verdict      : ACCEPTED
// Source       : https://www.spoj.com/problems/QTREE2/
// DS & Algo    : LCA

#include "bits/stdc++.h"
using namespace std;

#define pb push_back 
const int mx = 1e4+5;
using vi = vector<int>;
using pii = pair<int,int>;

int n, lgn;
vector< vi > sum;
vector< vi > par;
vector< int > height;
vector< pii > adj[mx];

int query(int u, int v){
    if(height[u] < height[v]) swap(u, v);

    int ans = 0;
    for(int i=lgn-1; i>=0; i--)
        if(height[u]-(1<<i) >= height[v])
            ans += sum[u][i], u = par[u][i];
    
    if(u == v) return ans;

    for(int i=lgn-1; i>=0; i--)
        if(~par[u][i] && par[u][i] != par[v][i])
            ans += sum[u][i] + sum[v][i], u = par[u][i], v = par[v][i];
    
    ans += sum[u][0] + sum[v][0];
    return ans;
}

int kthQuery(int u, int k){
    for(int i=lgn-1; i>=0; i--)
        if(k & (1<<i))
            u = par[u][i];
    return u;
}

int queryLCA(int u, int v){
    if(height[u] < height[v]) swap(u, v);

    for(int i=lgn-1; i>=0; i--)
        if(height[u]-(1<<i) >= height[v])
            u = par[u][i];
    
    if(u == v) return u;

    for(int i=lgn-1; i>=0; i--)
        if(~par[u][i] && par[u][i] != par[v][i])
            u = par[u][i], v = par[v][i];
    
    return par[u][0];
}

void dfs(int u, int p, int h, int d){
    par[u][0] = p; height[u] = h; sum[u][0] = d;
    for(pii &v : adj[u])
        if( v.first != p )
            dfs(v.first, u, h+1, v.second);
}

void init(){
    lgn = log2(n) + 1;
    height = vector<int>(n);
    sum = vector<vi>(n, vi(lgn));
    par = vector<vi>(n, vi(lgn,-1));

    dfs(0, -1, 0, 0);

    for(int j=1; j<lgn; j++)
    for(int i=0; i<n; i++){
        int p = par[i][j-1];
        if( p != -1 ){
            par[i][j] = par[p][j-1];
            sum[i][j] = sum[i][j-1] + sum[p][j-1];
        }
    }
}

int main(int argc, const char** argv) {

    ios_base :: sync_with_stdio(false);
    cin.tie( nullptr );

    string t;
    int u, v, w, m, k;

    int tc; cin >> tc;
    while(tc--){
        cin >> n; m = n-1;
        while(m--){
            cin >> u >> v >> w; u--; v--;
            adj[u].pb({v, w});
            adj[v].pb({u, w});
        }

        init();

        while(cin >> t, t[1] != 'O'){
            if(t[0] == 'D'){
                cin >> u >> v; u--; v--;
                cout << query(u, v) << "\n";
            }
            else{
                cin >> u >> v >> k; u--; v--;
                int lca = queryLCA(u, v);
                
                int ulen = height[u] - height[lca] + 1;
                int vlen = height[v] - height[lca] + 1;
                int tot = ulen + vlen - 1;

                if(k == ulen) cout << lca+1 << "\n";
                else if(k < ulen)
                    cout << kthQuery(u, k-1)+1 << "\n";
                else cout << kthQuery(v, tot - k)+1 << "\n";
            }
        }

        cout << "\n";

        for(int i=0; i<n; i++) adj[i].clear();
    }

    return 0;
}