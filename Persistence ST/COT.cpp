// Verdict      : ACCEPTED
// Source       : https://www.spoj.com/problems/COT/en/
// DS & Algo    : Persistence ST

#include <map>
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
using ll = long long;

#define sz size() 
#define pb push_back 
#define mid(l,r) (l+(r-l)/2) 

const int lgn = 18;
const int mx = 2e5+5;

ll rmp[mx];
map<ll, int> mp;

int n, q;
ll a[mx];
int N = 0;
int depth[mx];
int par[mx][lgn];
vector<int> adj[mx];

struct node{
    int c; node *l, *r;
    node(int va=0, node *le=NULL, node *ri=NULL):
        c(va), l(le), r(ri){}
} *root[mx];

// build for main root
void build(node *p, int l, int r){
    p->l = new node();
    p->r = new node();
    if(l != r){
        build(p->l, l, mid(l,r));
        build(p->r, mid(l,r)+1, r);
    }
}

// persistence segment tree update
void update(node *pre, node *cur, int l, int r, int idx){
    if(l == r) {cur->c = pre->c + 1; return;}
    if(idx <= mid(l,r)){
        cur->r = pre->r;
        cur->l = new node();
        update(pre->l, cur->l, l, mid(l,r), idx);
    }
    else{
        cur->l = pre->l;
        cur->r = new node();
        update(pre->r, cur->r, mid(l,r)+1, r, idx);
    }
    cur->c = cur->l->c + cur->r->c;
}

// gather info and builing tree for all [1, n]
void dfs(int u, int p, int h){
    par[u][0] = p; depth[u] = h;

    root[u] = new node();
    update(root[p], root[u], 0, N-1, mp[a[u]]);

    for(int &v : adj[u])
        if(v != p)
            dfs(v, u, h+1);
}

// finding lca
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

// find kth minimum node
// for finding the number of element under a tree exist is 
// formula : count(u) + count(v) - count(lca) - count(parent of lca);
int query(node *u, node *v, node *lca, node *p, int l, int r, int k){
    if(l == r) return l;
    int count = (u->l->c) + (v->l->c) - (lca->l->c) - (p->l->c);
    if(count >= k)
        return query(u->l, v->l, lca->l, p->l, l, mid(l,r), k);
    return query(u->r, v->r, lca->r, p->r, mid(l,r)+1, r, k-count);
}

int main(int argc, const char** argv) {

    int u, v, k;

    // reading input
    scanf("%d %d", &n, &q);
    for(int i=1; i<=n; i++)
        scanf("%lld", &a[i]), mp[a[i]];

    // for compressing into [0, N)
    for(auto &v : mp)
        mp[v.first] = N, rmp[N++] = v.first;

    // reading edges
    for(int i=1; i<n; i++){
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // setting root tree
    root[0] = new node();
    build(root[0], 0, N-1);

    // gathering info
    dfs(1, 0, 0);

    // binary lifting
    for(int j=1; j<lgn; j++)
    for(int i=1; i<=n; i++)
        par[i][j] = par[ par[i][j-1] ][j-1];

    // query
    while(q--){
        scanf("%d %d %d", &u, &v, &k);
        int lca = queryLCA(u, v);                   // finding lca
        node *proot = root[par[lca][0]];            // parent of lca
        int ans = query(root[u], root[v], root[lca], proot, 0, N-1, k);
        printf("%lld\n", rmp[ans]);                 // rmp for orginal value
    }

    return 0;
}