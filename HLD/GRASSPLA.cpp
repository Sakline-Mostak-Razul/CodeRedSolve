// STATUS : ACCEPTED
// Source : https://www.spoj.com/problems/GRASSPLA/
// DS & ALGO : HLD + ST

#include <cstdio>
#include <vector>
#include <utility>
#include <cstring>
#include <limits.h>
#include <algorithm>
using namespace std;

const int mx=100005;
using pii = pair<int,int>;
using iii = pair<int,pii>;

#define ff first 
#define se second
#define pb push_back 

int n, q, cp;
vector< int > adj[mx];
int height[mx], heavy[mx], pos[mx];
int par[mx], head[mx];

int st[3*mx+10];
int lazy[3*mx+10];

void updateLazy(int p, int l, int r){
    st[p] += (r - l + 1) * lazy[p];
    if(l != r){
        lazy[p<<1] += lazy[p];
        lazy[p<<1|1] += lazy[p];
    }
    lazy[p] = 0;
}

void update(int p, int l, int r, int i, int j, int v){
    if(l>=i && r<=j){
        lazy[p] += v;
        updateLazy(p, l, r);
        return;
    }
    if(lazy[p]) updateLazy(p, l, r);
    if(l>j || r<i) return;

    int mid = (l + r) / 2;
    update(p<<1, l, mid, i, j, v);
    update(p<<1|1, mid+1, r, i, j, v);
    st[p] = st[p<<1] + st[p<<1|1];
}

int query(int p, int l, int r, int i, int j){
    if(lazy[p]) updateLazy(p, l, r);
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    return (
        query(p<<1, l, mid, i, j) +
        query(p<<1|1, mid+1, r, i, j)
    );
}

// Heavy-Light Decompostion Query 
int query(int u, int v){
    if(u == v) return 0;
    int ans = 0;
    while(head[u] != head[v]){
        if(height[head[u]] > height[head[v]]) swap(u, v);
        ans += query(1, 1, n, pos[head[v]], pos[v]);
        v = par[head[v]];
    }
    if(u == v) return ans;
    if(pos[u] > pos[v]) swap(u, v);
    ans += query(1, 1, n, pos[u]+1, pos[v]);
    return ans;
}

void update(int u, int v){
    if(u == v) return;
    while(head[u] != head[v]){
        if(height[head[u]] > height[head[v]]) swap(u, v);
        update(1, 1, n, pos[head[v]], pos[v], 1);
        v = par[head[v]];
    }
    if(u == v) return;
    if(pos[u] > pos[v]) swap(u, v);
    update(1, 1, n, pos[u]+1, pos[v], 1);
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

    char s[5];
    int u, v, w;
    
    int tc = 1;
    while(tc--){
        scanf("%d %d", &n, &q);

        for(int i=1; i<n; i++){
            scanf("%d %d", &u, &v);
            adj[u].pb(v);
            adj[v].pb(u);
        }

        memset(heavy, -1, sizeof(heavy));
        cp = 1; dfs(1, 0, 0); decom(1, 1);


        while(q--){
            scanf("%s %d %d", s, &u, &v);
            if(s[0] == 'Q')
                printf("%d\n", query(u,v));
            else update(u, v);
        }

        for(int i=0; i<=n; i++) adj[i].clear();
    }
    return 0;
}