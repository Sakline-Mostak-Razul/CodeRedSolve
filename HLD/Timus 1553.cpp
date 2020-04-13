// STATUS : ACCEPTED
// Source : https://acm.timus.ru/problem.aspx?space=1&num=1553
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
int par[mx], head[mx], st[3*mx+10];

void update(int p, int l, int r, int idx, int v){
    if(l == r){st[p] += v; return;}
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, v);
    else update(p<<1|1, mid+1, r, idx, v);
    st[p] = max(st[p<<1], st[p<<1|1]);
}

int query(int p, int l, int r, int i, int j){
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    return max(
        query(p<<1, l, mid, i, j),
        query(p<<1|1, mid+1, r, i, j)
    );
}

// Heavy-Light Decompostion Query 
int query(int u, int v){
    int ans = 0;
    while(head[u] != head[v]){
        if(height[head[u]] > height[head[v]]) swap(u, v);
        ans = max(ans, query(1, 1, n, pos[head[v]], pos[v]));
        v = par[head[v]];
    }
    if(pos[u] > pos[v]) swap(u, v);
    ans = max(ans, query(1, 1, n, pos[u], pos[v]));
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
        scanf("%d", &n);

        for(int i=1; i<n; i++){
            scanf("%d %d", &u, &v);
            adj[u].pb(v);
            adj[v].pb(u);
        }

        memset(heavy, -1, sizeof(heavy));
        cp = 1; dfs(1, 0, 0); decom(1, 1);

        scanf("%d", &q);
        while(q--){
            scanf("%s %d %d", s, &u, &v);
            if(s[0] == 'G')
                printf("%d\n", query(u,v));
            else update(1, 1, n, pos[u], v);
        }

        for(int i=0; i<=n; i++) adj[i].clear();
    }
    return 0;
}