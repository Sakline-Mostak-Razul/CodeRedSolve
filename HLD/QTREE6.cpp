// Status : ACCEPTED
// Source : https://www.spoj.com/problems/QTREE6/en/
//          https://www.codechef.com/problems/QTREE6
// DS & ALGO : HLD + ST

#include "set"
#include "cstdio"
#include "vector"
#include "cstring"
using namespace std;

const int mx = 1e5+5;

#define ff first 
#define se second 
#define pb push_back 
#define all(x) x.begin(),x.end() 

int n, q, cp, cur;
vector< int > adj[mx];
set<int, greater<int> > st[2][mx];
int tree[2][mx<<2], lazy[2][mx<<2];
int sz[mx], cid[mx], col[mx], a[2][mx];
int par[mx], head[mx], pos[mx], id[mx];

void dfs(int u, int p){
    par[u] = p; sz[u] = 1;
    for(int &v : adj[u]) if(v != p)
        dfs(v, u), sz[u] += sz[v];
}

void hld(int u, int p){
    if(head[cur] == -1) head[cur] = u;
    cid[u] = cur; pos[u] = ++cp; id[cp] = u;
    a[0][cp] = sz[u]; st[0][cur].insert(cp);

    int x = -1; for(int &v : adj[u])
        if((v!=p) && (x==-1 || sz[x]<sz[v])) x = v;
    if(~x) hld(x, u);

    for(int &v : adj[u]) if(v!=x && v!=p)
        cur++, hld(v, u);
}

void build(int p, int l, int r, int t){
    if(l == r) tree[t][p] = a[t][l];
    else{
        int mid = (l + r) / 2;
        build(p<<1, l, mid, t);
        build(p<<1|1, mid+1, r, t);
        tree[t][p] = tree[t][p<<1] + tree[t][p<<1|1];
    }
}

void push(int p, int l, int r, int t){
    if(lazy[t][p]){
        tree[t][p] += (r-l+1) * lazy[t][p];
        if( l != r ){
            lazy[t][p<<1] += lazy[t][p];
            lazy[t][p<<1|1] += lazy[t][p];
        }
        lazy[t][p] = 0;
    }
}

int query(int p, int l, int r, int i, int j, int t){
    push(p, l, r, t);
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) 
        return tree[t][p];
    int mid = (l + r) / 2;
    return (
        query(p<<1, l, mid, i, j, t) +
        query(p<<1|1, mid+1, r, i, j, t)
    );
}

int ancestor(int u){
    int uc = col[u] ^ 1;
    while(u > 1){
        int cn = cid[u];
        auto it = st[uc][cn].lower_bound(pos[u]);
        if(it != st[uc][cn].end()) return id[*it + 1];

        u = head[cn];
        if(u==1 || col[par[u]]==uc) return u;
        u = par[u];
    }
    return 1;
}

void update(int p, int l, int r, int i, int j, int v, int t){
    push(p, l, r, t);
    if(l>j || r<i) return;
    if(l>=i && r<=j){
        lazy[t][p] += v;
        push(p, l, r, t);
        return;
    }
    int mid = (l + r) / 2;
    update(p<<1, l, mid, i, j, v, t);
    update(p<<1|1, mid+1, r, i, j, v, t);
    tree[t][p] = tree[t][p<<1] + tree[t][p<<1|1];
}

void update(int u, int v, int val, int t){
    while(cid[u] != cid[v]){
        update(1, 1, n, pos[head[cid[u]]], pos[u], val, t);
        u = par[head[cid[u]]];
    }
    update(1, 1, n, pos[v], pos[u], val, t);
}

void update(int u){
    int p = ancestor(u);
    int v = query(1, 1, n, pos[u], pos[u], col[u]);
    update(1, 1, n, pos[u], pos[u], -1, col[u]);

    if(~par[p]) p = par[p];
    if(~par[u]) update(par[u], p, -v, col[u]);
    st[ col[u] ][ cid[u] ].erase( pos[u] );

    col[u] ^= 1;
    p = ancestor(u);
    update(1, 1, n, pos[u], pos[u], 1, col[u]);
    v = query(1, 1, n, pos[u], pos[u], col[u]);

    if(~par[p]) p = par[p];
    if(~par[u]) update(par[u], p, v, col[u]);
    st[ col[u] ][ cid[u] ].insert( pos[u] );
}

int query(int u){
    int p = ancestor(u);
    return query(1, 1, n, pos[p], pos[p], col[u]);
}

int main(int argc, const char** argv) {

    cur = 1;
    memset(head, -1, sizeof head);
    
    int t, u, v; scanf("%d", &n);
    for(int i=1; i<n; i++){
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, -1); hld(1, -1);
    build(1, 1, n, 0);

    scanf("%d", &q);
    while(q--){
        scanf("%d %d", &t, &u);
        if(t == 1) update(u);
        else printf("%d\n", query(u));
    }

    return 0;
}