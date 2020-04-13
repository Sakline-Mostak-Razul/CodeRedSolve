// Verdict      : ACCEPTED
// Source       : https://www.spoj.com/problems/GSS7/
// DS & Algo    : HLD + Lazy ST

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
struct node{
    ll pre, suf, sum, best;
    node(ll v=0){
        sum = v;
        pre = suf = best = max(0LL, v);
    }
};

node merge(node u, node v){
    node ans(0);
    ans.sum = u.sum + v.sum;
    ans.pre = max(u.pre, u.sum+v.pre);
    ans.suf = max(v.suf, v.sum+u.suf);
    ans.best = max({u.best, v.best, u.suf+v.pre});
    return ans;
}

const ll mx = 100005;
#define pb push_back 

node st[mx << 2];
ll lazy[mx << 2];
ll flag[mx << 2];
vector<ll> adj[mx];

ll n, cp, q, a[mx];
ll depth[mx], head[mx], wt[mx];
ll par[mx], pos[mx], heavy[mx];

void build(ll p, ll l, ll r){
    lazy[p] = flag[p] = 0;

    if(l == r) st[p] = node(wt[l]);
    else{
        ll mid = (l + r)/2;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
        st[p] = merge(st[p<<1], st[p<<1|1]);
    }
}

void updateLazy(ll p, ll l, ll r){
    st[p].sum = lazy[p] * (r - l + 1);
    st[p].pre = st[p].suf = st[p].best = max(st[p].sum, 0LL);

    if(l != r) {
        flag[p<<1] = flag[p<<1|1] = 1;
        lazy[p<<1] = lazy[p<<1|1] = lazy[p];
    }
    lazy[p] = 0; flag[p] = 0;
}

void update(ll p, ll l, ll r, ll i, ll j, ll v){
    if(l>=i && r<=j){
        lazy[p] = v; flag[p] = 1;
        updateLazy(p, l, r);
        return;
    }
    if(flag[p]) updateLazy(p, l, r);
    if(l>j || r<i) return;

    int mid = (l + r) / 2;
    update(p<<1, l, mid, i, j, v);
    update(p<<1|1, mid+1, r, i, j, v);
    st[p] = merge(st[p<<1], st[p<<1|1]);
}

node query(ll p, ll l, ll r, ll i, ll j){
    if(flag[p]) updateLazy(p, l, r);
    
    if(l>j || r<i) return node();
    if(l>=i && r<=j) return st[p];

    ll mid = (l + r) / 2;
    node u = query(p<<1, l, mid, i, j);
    node v = query(p<<1|1, mid+1, r, i, j);
    st[p] = merge(st[p<<1], st[p<<1|1]);
    return merge(u, v);
}

ll query(ll u, ll v){
    node left, right;
    while(head[u] != head[v]){
        if(depth[head[u]] > depth[head[v]]){
            left = merge(query(1, 1, cp, pos[head[u]], pos[u]), left);
            u = par[ head[u] ];
        }
        else{
            right = merge(query(1, 1, cp, pos[head[v]], pos[v]), right);
            v = par[ head[v] ];
        }
    }
    if(depth[u] > depth[v]) 
        left = merge(query(1, 1, cp, pos[v], pos[u]), left);
    else right = merge(query(1, 1, cp, pos[u], pos[v]), right);
    swap(left.pre, left.suf);
    return merge(left, right).best;
}

void update(ll u, ll v, ll val){
    while(head[u] != head[v]){
        if(depth[head[u]] > depth[head[v]]) swap(u, v);
        update(1, 1, cp, pos[head[v]], pos[v], val);
        v = par[ head[v] ];
    }
    if(pos[u] > pos[v]) swap(u, v);
    update(1, 1, cp, pos[u], pos[v], val);
}

ll dfs(ll u, ll p, ll h){
    ll sz = 1, msz = 0, vsz;
    depth[u] = h; par[u] = p;
    for(ll &v : adj[u]){
        if(v == p) continue;
        sz += (vsz = dfs(v, u, h+1));
        if(msz < vsz)
            msz = vsz, heavy[u] = v;
    }
    return sz;
}

void decom(ll u, ll p){
    head[u] = p; pos[u] = ++cp; wt[cp] = a[u];
    if(~heavy[u]) decom(heavy[u], p);
    for(ll &v : adj[u])
        if(v != par[u] && v != heavy[u])
            decom(v, v);
}

void init(){
    memset(heavy, -1, sizeof heavy);

    cp = 0; 
    dfs(1, 0, 0); 
    decom(1, 1);
    build(1, 1, cp);
}

int main(int argc, const char** argv) {

    ios_base :: sync_with_stdio(false);
    cin.tie( nullptr );

    ll t, u, v, val;

    cin >> n;
    for(ll i=1; i<=n; i++)
        cin >> a[i];

    for(ll i=1; i<n; i++){
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }

    init();

    cin >> q;
    while(q--){
        cin >> t >> u >> v;
        if(t == 1)
            cout << query(u, v) << "\n";
        else{
            cin >> val;
            update(u, v, val);
        }
    }

    return 0;
}