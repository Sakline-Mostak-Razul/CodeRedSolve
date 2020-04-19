// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/SUBLEX/
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;

#define ff first
#define se second
const int mx = 1e5+5;
using ii = pair<int,int>;

int sz, last;
struct state {
    int len,link,fpos;
    map<char,int>next;
} st[mx << 1];

void sa_extend(char c) {
    int cur = sz++, p = last; 
    st[cur] = {st[p].len+1, -1, st[p].len};

    for(; ~p && !st[p].next.count(c); p=st[p].link)
        st[p].next[c] = cur;
    if(p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if(st[p].len+1 == st[q].len) st[cur].link = q;
        else {
            int cln = sz++;
            st[cln] = st[q];
            st[cln].len = st[p].len+1;

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

int dss[mx<<1]; /// distinct substring
int dssFun(int u){
    if(dss[u]) return dss[u];

    dss[u] = 1;
    for(char i='a'; i<='z'; i++)
        if(st[u].next.count(i))
            dss[u] += dssFun(st[u].next[i]);
    return dss[u];
}

void build(string &s){
    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<s.size(); i++) 
        sa_extend(s[i]);
    dssFun(0);
}

void solve(int u, int k){
    if(k == 0) return;
    for(auto p : st[u].next){
        int v = p.second;
        if(dss[v] >= k){
            cout << p.first;
            solve(v, k-1);
            return;
        }
        k -= dss[v];
    }
}

void solve2(int k){
    int u = 0;
    while(k > 0){
        for(auto p : st[u].next){
            int v = p.second;
            if(dss[v] >= k){
                cout << p.first;
                u = v; k--;
                break;
            }
            k -= dss[v];
        }
    }
}

int main(int argc, const char** argv) {

    ios_base::sync_with_stdio(0);
    cin.tie( nullptr );

    string s;
    cin >> s; 
    build(s);

    int tc, k; cin >> tc;
    while(tc--){
        cin >> k;
        solve2(k);
        cout << "\n";
    }

    return 0;
}
