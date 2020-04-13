// Status : ACCEPTED
// Source : http://lightoj.com/volume_showproblem.php?problem=1348
// DS & ALGO : HLD + ST

#include <cstdio>
#include <vector>
#include <utility>
#include <cstring>
#include <limits.h>
#include <algorithm>
using namespace std;

const int mx = 30005;
using pii = pair<int,int>;
using iii = pair<int,pii>;

#define ff first 
#define se second
#define pb push_back 

int n, cp;
vector< int > adj[mx];
int height[mx], wt[mx], heavy[mx], a[mx];
int par[mx], head[mx], pos[mx], st[3*mx+10];

int merge(int u, int v){return u+v;}
void build(int p, int l, int r){
    if(l == r) st[p] = wt[l];
    else{
        int mid = (l + r) / 2;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
        st[p] = merge(st[p<<1], st[p<<1|1]);
    }
}

void update(int p, int l, int r, int idx, int v){
    if(l == r) {st[p] = v; return;}
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, v);
    else update(p<<1|1, mid+1, r, idx, v);
    st[p] = merge(st[p<<1], st[p<<1|1]);
}

int query(int p, int l, int r, int i, int j){
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    return merge(
        query(p<<1, l, mid, i, j),
        query(p<<1|1, mid+1, r, i, j)
    );
}

// Heavy-Light Decompostion Query 
int query(int u, int v){
    int ans = 0;
    while(head[u] != head[v]){
        if(height[head[u]] > height[head[v]]) swap(u, v);
        ans = merge(ans, query(1, 0, n-1, pos[head[v]], pos[v]));
        v = par[head[v]];
    }
    if(pos[u] > pos[v]) swap(u, v);
    ans = merge(ans, query(1, 0, n-1, pos[u], pos[v]));
    return ans;
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
    head[u] = p; pos[u] = cp; wt[cp++] = a[u];
    if(~heavy[u]) decom(heavy[u], p);
    for(int &v : adj[u])
        if(v != par[u] && v != heavy[u])
            decom(v, v);
}

int main(int argc, const char** argv) {

    int t, u, v;
    int tc, cs=1; 
    scanf("%d", &tc);

    while(tc--){
        scanf("%d", &n);
        for(int i=0; i<n; i++) scanf("%d", &a[i]);
        for(int i=1; i<n; i++){
            scanf("%d %d", &u, &v);
            adj[u].pb( v );
            adj[v].pb( u );
        }

        memset(heavy, -1, sizeof heavy);
        cp = 0; dfs(0, -1, 0); decom(0, 0);
        build(1, 0, n-1);

        int q; scanf("%d", &q);
        printf("Case %d:\n", cs++);
        while(q--){
            scanf("%d %d %d", &t, &u, &v);
            if(t == 0)
                printf("%d\n", query(u,v));
            else update(1, 0, n-1, pos[u], v);
        }

        for(int i=0; i<n; i++) adj[i].clear();
    }

    return 0;
}