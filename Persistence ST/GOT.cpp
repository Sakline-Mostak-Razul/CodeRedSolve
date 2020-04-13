// Verdict      : ACCEPTED
// Source       : https://www.spoj.com/problems/GOT/
// DS & Algo    : LCA and Persistece ST

#include <bits/stdc++.h>
using namespace std;

#define ff first 
#define se second 
#define pb push_back
#define mid(l,r) (l+(r-l)/2) 
#define all(x) x.begin(),x.end()

const int mx = 1e5+5;
const int lgn = 18;

using vi = vector<int>;
using pii = pair<int,int>;

struct node{
    int v; node *l, *r;
    node(int va=0, node *le=NULL, node *ri=NULL):
        v(va), l(le), r(ri){}
} *root[mx];

int a[mx], n, q;
vector<int> adj[mx];
int depth[mx], par[mx][18];

void merge(node *p){p->v = (p->l->v) + (p->r->v);}
void build(node *p, int l, int r){
    if(l == r) p->v = 0;
    else{
        p->l = new node();
        p->r = new node();
        build(p->l, l, mid(l,r));
        build(p->r, mid(l,r)+1, r);
        merge(p);
    }
}

void update(node *pre, node *cur, int l, int r, int i, int v){
    if(l == r) {cur->v = (pre->v + v); return;}
    if(i <= mid(l,r)){
        cur->r = pre->r;
        cur->l = new node();
        update(pre->l, cur->l, l, mid(l,r), i, v);
    }
    else{
        cur->l = pre->l;
        cur->r = new node();
        update(pre->r, cur->r, mid(l,r)+1, r, i, v);
    }
    merge(cur);
}

int query(node *p, int l, int r, int i, int j){
    if(l>r || l>j || r<i) return 0;
    if(l>=i && r<=j) return p->v;
    return query(p->l, l, mid(l,r), i, j) +
           query(p->r, mid(l,r)+1, r, i, j);
}

int queryLCA(int u, int v){
    if(depth[u] < depth[v]) swap(u, v);
    for(int i=lgn-1; i>=0; i--)
        if(depth[u]-(1<<i) >= depth[v])
            u = par[u][i];
    if(u == v) return u;
    for(int i=lgn-1; i>=0; i--)
        if(par[u][i] != par[v][i])
            u = par[u][i], v = par[v][i];
    return par[u][0];
}

void dfs(int u, int p, int h){
    depth[u] = h; par[u][0] = p;

    root[u] = new node();
    update(root[p], root[u], 0, n, a[u], 1);
    for(int &v : adj[u])
        if(v != p)
            dfs(v, u, h+1);
}

int main(int argc, const char** argv) {
    
    ios_base :: sync_with_stdio( false );
    cin.tie( nullptr );
    
    int u, v, x;
    while(cin >> n >> q){
        
        root[0] = new node();
        build(root[0], 0, n);

        for(int i=1; i<=n; i++) cin >> a[i];
        for(int i=1; i<n; i++){
            cin >> u >> v;
            adj[u].pb(v);
            adj[v].pb(u);
        }

        memset(par, 0, sizeof par);
        dfs(1, 0, 0);

        for(int j=1; j<lgn; j++)
        for(int i=1; i<=n; i++)
            par[i][j] = par[ par[i][j-1] ][j-1];

        while(q--){
            cin >> u >> v >> x;
            int lca = queryLCA(u, v);

            int ans = query(root[u], 0, n, x, x);
            ans += query(root[v], 0, n, x, x);
            ans -= query(root[lca], 0, n, x, x);
            ans -= query(root[par[lca][0]], 0, n, x, x);

            cout << (ans>0 ? "Find\n" : "NotFind\n");
        }
        cout << "\n";

        for(int i=0; i<=n; i++){
            adj[i].clear();
            delete root[i];
        }
    }

    return 0;
}