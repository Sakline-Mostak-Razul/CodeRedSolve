// Status : ACCEPTED
// Source : https://www.codechef.com/problems/PSHTTR
// DS & ALGO : HLD + ST ( Offline )

#include <vector>
#include <stdio.h>
#include <utility>
#include <string.h>
#include <algorithm>

#define ff first 
#define se second 
#define pb push_back 

const int mx=1e5+5;
using pii = std::pair<int,int>;

int ans[mx];
int st[mx<<2], pos[mx];
int n, q, cp, head[mx];
std :: vector< int > adj[mx];
int par[mx], dep[mx], havy[mx];

void swap(int &u, int &v){u^=v; v^=u; u^=v;}
int  gcd(int a, int b){return a^b;}
void pull(int p){st[p] = gcd(st[p<<1], st[p<<1|1]);}

int dfs(int u, int p, int h){
    par[u] = p; dep[u] = h;
    int msz = 0, sz = 1, vsz;
    for(int &v : adj[u]){
        if(v == p) continue;
        sz += (vsz = dfs(v, u, h+1));
        if(msz < vsz)
            msz = vsz, havy[u] = v;
    }
    return sz;
}

void decom(int u, int p){
    head[u] = p; pos[u] = cp++;
    if(~havy[u]) decom(havy[u], p);
    for(int &v : adj[u])
        if(v!=par[u] && v!=havy[u])
            decom(v, v);
}

void update(int p, int l, int r, int idx, int val){
    if(l == r) {st[p] = val; return;}

    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, val);
    else    
        update(p<<1|1, mid+1, r, idx, val);
    pull(p);
}

int qgcd(int p, int l, int r, int i, int j){
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    return gcd(
        qgcd(p<<1, l, mid, i, j),
        qgcd(p<<1|1, mid+1, r, i, j)
    );
}

int query(int u, int v){
    int ans = 0;
    while(head[u] != head[v]){
        if(dep[head[u]] > dep[head[v]]) swap(u, v);
        ans = gcd(ans, qgcd(1, 1, n, pos[head[v]], pos[v]));
        v = par[head[v]];
    }
    if( u == v ) return ans;

    if(pos[u] > pos[v]) swap(u, v);
    ans = gcd(ans, qgcd(1, 1, n, pos[u]+1, pos[v]));
    return ans;
}

void read(int &x){scanf("%d", &x);}

struct edges{
    int u, v, w;
    edges(int _u, int _v, int _w):
        u(_u), v(_v), w(_w){}
    bool operator < (const edges &ot){
        return w < ot.w;
    }
};

struct queries{
    int u, v, w, id;
    queries(int _u, int _v, int _w, int _id):
        u(_u), v(_v), w(_w), id(_id){}
    bool operator < (const queries &ot){
        return w < ot.w;
    }
};

int main(int argc, const char** argv) {

    int tc, u, v, w;

    read(tc);
    while(tc--){

        read(n);
        std :: vector< edges > ed;
        for(int i=1; i<n; i++){
            read(u); read(v); read(w);
            adj[u].push_back(v);
            adj[v].push_back(u);
            ed.pb({u, v, w});
        }
        sort(ed.begin(), ed.end());

        memset(st, 0, sizeof st);
        memset(havy, -1, sizeof havy);
        cp=1; dfs(1, 0, 0); decom(1,1);

        read(q);
        std :: vector< queries > qu;
        for(int i=0; i<q; i++){
            read(u); read(v); read(w);
            qu.pb({u, v, w, i});
        }
        sort(qu.begin(), qu.end());

        for(int i=0,j=0; i<q; i++){
            int u = qu[i].u;
            int v = qu[i].v;
            int w = qu[i].w;
            while(j<ed.size() && ed[j].w<=w){
                int l = ed[j].u;
                int r = ed[j].v;
                if(l == par[r])
                    update(1, 1, n, pos[r], ed[j].w);
                else 
                    update(1, 1, n, pos[l], ed[j].w);
                j++;
            }
            ans[ qu[i].id ] = query(u, v);
        }

        for(int i=0; i<q; i++)
            printf("%d\n", ans[i]);
        
        for(int i=1; i<=n; i++)
            adj[i].clear();
    }

    return 0;
}