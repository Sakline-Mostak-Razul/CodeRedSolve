// Verdict      : Not Judged
// Source       : https://www.spoj.com/problems/OTOCI/
// DS & Algo    : HLD + ST + DSU

#include <bits/stdc++.h>
using namespace std;

const int mx = 3e4+5;
#define pb push_back 

class UnionFind{
private:
    int numSets;
    vector<int> p, r;
    vector<int> setSize;
    
public:
    UnionFind(int n){
        numSets = n;
        r.assign(n, 0);
        p.assign(n, -1);
        setSize.assign(n, 1);
    }
    int findSet(int i){return (p[i]==-1)?i:p[i] = findSet(p[i]);}
    bool isSameSet(int i, int j){return findSet(i) == findSet(j);}
    void unionSet(int i, int j){
        if(!isSameSet(i, j)){
            int x = findSet(i);
            int y = findSet(j);
            if(r[x] > r[y]){
                setSize[x] += setSize[y];
                p[y] = x;
            }
            else{
                setSize[y] += setSize[x];
                p[x] = y;
                if(r[x] == r[y])
                    r[y]++;
            }
            numSets--;
        }
    }
    int numDisJointSets(){return numSets;}
    int setSizeOf(int x){return setSize[findSet(x)];}
};

int st[mx*3+10];
int n, cp, a[mx];
vector< int > adj[mx];
int par[mx], pos[mx], depth[mx];
int heavy[mx], head[mx], wt[mx], vis[mx];

void build(int p, int l, int r){
    if(l == r) st[p] = wt[l];
    else{
        int mid = (l + r)/2;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
        st[p] = st[p<<1] + st[p<<1|1];
    }
}

void update(int p, int l, int r, int idx, int val){
    if(l == r) {st[p] = val; return;}
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(p<<1, l, mid, idx, val);
    else update(p<<1|1, mid+1, r, idx, val);
    st[p] = st[p<<1] + st[p<<1|1];
}

int query(int p, int l, int r, int i, int j){
    if(l>j || r<i) return 0;
    if(l>=i && r<=j) return st[p];

    int mid = (l + r) / 2;
    return query(p<<1, l, mid, i, j) +
           query(p<<1|1, mid+1, r, i, j);
}

int query(int u, int v){
    int ans = 0;
    while(head[u] != head[v]){
        if(depth[head[u]] > depth[head[v]]) swap(u, v);
        ans += query(1, 0, cp-1, pos[head[v]], pos[v]);
        v = par[ head[v] ];
    }
    if(pos[u] > pos[v]) swap(u, v);
    ans += query(1, 0, cp-1, pos[u], pos[v]);
    return ans;
}

int dfs(int u, int p, int h){
    int sz = 1, msz = 0, vsz;
    depth[u] = h+1; par[u] = p; vis[u] = 1;
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

void init(){
    memset(vis, 0, sizeof vis);
    memset(heavy, -1, sizeof heavy);
    
    cp = 0; 
    for(int i=1; i<=n; i++)
    if(vis[i] == 0)
        dfs(i, 0, 0), decom(i, i);

    build(1, 0, cp-1);
}

int main(int argc, const char** argv) {
    ios_base :: sync_with_stdio(0);
    cin.tie( nullptr );

    string s;
    int u, v;

    cin >> n;
    for(int i=1; i<=n; i++)
        cin >> a[i];
    
    int q; cin >> q;
    struct node{
        int t, l, r; bool ok;
        node(){}
        node(int _t, int _l, int _r, bool _ok):
            t(_t), l(_l), r(_r), ok(_ok){};
    };
    vector< node > qq;

    UnionFind uf(n + 5);
    for(int i=0; i<q; i++){
        cin >> s >> u >> v;
        if(s[0] == 'e')
            qq.push_back(node(0, u, v, uf.isSameSet(u,v)));
        else if(s[0] == 'p')
            qq.push_back(node(2, u, v, true));
        else if(!uf.isSameSet(u, v)){
            adj[u].pb(v);
            adj[v].pb(u);
            uf.unionSet(u, v);
            qq.push_back(node(1, u, v, true));
        }
        else qq.push_back(node(1, u, v, false));
    }

    init();

    for(int i=0; i<q; i++){
        int t = qq[i].t;
        int u = qq[i].l;
        int v = qq[i].r;
        bool ok = qq[i].ok;

        // cout << t << ' ' << u <<' ' << v << ' ' << ok << "\n";

        if(t == 0){
            if(ok) cout << query(u, v) << "\n";
            else cout << "impossilbe\n";
        }
        else if(t == 2)
            update(1, 0, cp-1, pos[u], v);
        else{
            if(ok) cout << "yes\n";
            else cout << "no\n";
        }
    }

    return 0;
}