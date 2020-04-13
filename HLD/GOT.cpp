// Verdict      : ACCEPTED
// Source       : https://www.spoj.com/problems/GOT/
// DS & Algo    : HLD + ST ( Offline )

#include <bits/stdc++.h>
using namespace std;

#define ff first 
#define se second 
#define pb push_back
#define all(x) x.begin(),x.end()

const int mx = 1e5+5;
using vi = vector<int>;
using pii = pair<int,int>;

int n, q, cp;
vector<int> adj[mx]; pii a[mx];
int height[mx], heavy[mx], ans[mx];
int par[mx], head[mx], pos[mx], st[3*mx+10];

void update(int p, int l, int r, int idx, int v){
    if(l == r) {st[p] = v; return;}
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, v);
    else update(p<<1|1, mid+1, r, idx, v);
    st[p] = max(st[p<<1], st[p<<1|1]);
}

int query(int p, int l, int r, int i, int j){
    if(l>j || r<i) return INT_MIN;
    if(l>=i && r<=j) return st[p];
    int mid = (l + r) / 2;
    return max(
        query(p<<1, l, mid, i, j),
        query(p<<1|1, mid+1, r, i, j)
    );
}

// Heavy-Light Decompostion Query 
int query(int u, int v){
    int ans = INT_MIN;
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

void init(){
    memset(st, -1, sizeof st);
    memset(heavy, -1, sizeof heavy);

    cp = 1; dfs(1, 0, 0); decom(1, 1);
}

struct node{
    int l, r, val, id;
    node(int _l, int _r, int _val, int _id):
        l(_l), r(_r), val(_val), id(_id){}
    bool operator < (const node &ot){
        if(val != ot.val) 
            return val < ot.val;
        return id < ot.id;
    }
};

int main(int argc, const char** argv) {
    
    ios_base :: sync_with_stdio( false );
    cin.tie( nullptr );
    
    int u, v, val;
    while(cin >> n >> q){
        for(int i=1; i<=n; i++){
            cin >> a[i].ff;
            a[i].se = i;
        }
        for(int i=1; i<n; i++){
            cin >> u >> v; 
            adj[u].pb(v);
            adj[v].pb(u);
        }

        init();

        sort(a+1, a+n+1);
        
        vector<node> qq;
        for(int i=0; i<q; i++){
            cin >> u >> v >> val;
            qq.push_back(node(u, v, val, i));
        }
        sort( all(qq) );

        for(int j=1, i=0; i<q; i++){
            u = qq[i].l, v = qq[i].r, val = qq[i].val;
            while(j<=n && a[j].ff<=val){
                update(1, 1, n, pos[ a[j].se ], a[j].ff);
                j++;
            }
            ans[ qq[i].id ] = (query(u, v) == val);
        }

        for(int i=0; i<q; i++)
            cout << (ans[i] ? "Find\n" : "NotFind\n");
        cout << "\n";

        for(int i=0; i<=n; i++)
            adj[i].clear();
    }

    return 0;
}