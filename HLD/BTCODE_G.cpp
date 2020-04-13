// Status : ACCEPTED
// Source : https://www.spoj.com/problems/BTCODE_G/
// DS & ALGO : HLD + ST

#include "bits/stdc++.h"
using namespace std;
using vi = vector<int>;
using pii = pair<int,int>;

#define ff first 
#define se second 
#define pb push_back 
const int lgn = 18;
const int mx = 1e5+5;

struct node{
    int MAX, MIN;
    node(int v=0):
        MAX(v),MIN(v){}
} st[mx << 2];

int n, q, cp;
vector<int> adj[mx];
int height[mx], heavy[mx];
int par[mx], head[mx], pos[mx];

node merge(node u, node v){
    node ans;
    ans.MAX = max(u.MAX, v.MAX);
    ans.MIN = min(u.MIN, v.MIN);
    return ans;
}

void build(int p, int l, int r){
    if(l == r) st[p] = node(0);
    else{
        int mid = (l + r) / 2;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
        st[p] = merge(st[p<<1], st[p<<1|1]);
    }
}

void update(int p, int l, int r, int idx, int v){
    if(l == r) {st[p] = node(v); return;}
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, v);
    else update(p<<1|1, mid+1, r, idx, v);
    st[p] = merge(st[p<<1], st[p<<1|1]);
}

node query(int p, int l, int r, int i, int j){
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    if(j<=mid) 
        return query(p<<1, l, mid, i, j);
    else if(i >= mid)
        return query(p<<1|1, mid+1, r, i, j);
    return merge(
        query(p<<1, l, mid, i, j),
        query(p<<1|1, mid+1, r, i, j)
    );
}

// Heavy-Light Decompostion Query 
bool query(int u, int v){
    node ans; ans.MIN = INT_MAX; ans.MAX = INT_MIN;
    while(head[u] != head[v]){
        if(height[head[u]] > height[head[v]]) swap(u, v);
        ans = merge(ans, query(1, 0, n-1, pos[head[v]], pos[v]));
        v = par[head[v]];
    }
    if(pos[u] > pos[v]) swap(u, v);
    ans = merge(ans, query(1, 0, n-1, pos[u], pos[v]));
    return ans.MAX == ans.MIN && ans.MIN>0;
}

// Heavy-Light Decompostion Part
int dfs(int u, int p, int h){
    int sz=1, msz=0, vsz;
    height[u]=h; par[u]=p;
    for(int &v : adj[u]){
        if(v == p) continue;
        sz += (vsz = dfs(v, u, h+1));
        if(msz < vsz) msz = vsz, heavy[u] = v;
    }
    return sz;
}

void decom(int u, int p){
    head[u] = p; pos[u] = cp++;
    if(~heavy[u]) decom(heavy[u], p);
    for(int &v : adj[u])
        if(v != par[u] && v != heavy[u])
            decom(v, v);
}

int main(int argc, const char** argv) {

    ios_base :: sync_with_stdio( false );
    cin.tie( nullptr );

    int u, v, x;
    while(cin >> n){
        
        for(int i=1; i<n; i++){
            cin >> u >> v;
            adj[u].pb(v);
            adj[v].pb(u);
        }

        memset(heavy, -1, sizeof heavy);
        
        cp = 0;
        dfs(0, -1, 0);
        decom(0, 0);
        build(1, 0, n-1);

        cin >> q;
        for(int i=0; i<q; i++){
            cin >> x >> u >> v;
            if(x == 1)
                update(1, 0, n-1, pos[u], v);
            else if(query(u, v))
                cout << "YES\n";
            else cout << "NO\n";
        }

        for(int i=0; i<n; i++)
            adj[i].clear();
    }

    return 0;
}