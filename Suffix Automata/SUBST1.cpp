// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/SUBST1/
// DS & ALGO    : Suffix Automata

#include<bits/stdc++.h>
using namespace std;

#define rint register int

#define ff first
#define se second
const int mx = 1e5+5;
using ii = pair<int,int>;

struct state {
    int len,link,fpos;
    map<char,int>next;
} st[mx << 1];
int sz, last, cnt[mx<<1];

void sa_extend(char c) {
    int cur = sz++, p = last; cnt[cur] = 1;
    st[cur] = {st[p].len+1, -1, st[p].len+1};

    for(; ~p && !st[p].next.count(c); p=st[p].link)
        st[p].next[c] = cur;
    if(p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if(st[p].len+1 == st[q].len) st[cur].link = q;
        else {
            int cln = sz++;
            cnt[cln] = 0;
            st[cln] = st[q];
            st[cln].len = st[p].len+1;

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

void build(string &s){
    for(rint i=0; i<sz; i++) 
        cnt[i]=0, st[i].next.clear();

    st[0] = {0,-1}; sz = 1; last = 0;
    for(rint i=0; i<s.size(); i++) 
        sa_extend(s[i]);
}

int dss[mx<<1]; /// distinct substring
int dssFun(int u){
    if(dss[u]) return dss[u];

    dss[u] = 1;
    for(auto p : st[u].next)
        dss[u] += dssFun(p.second);
    return dss[u];
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie( nullptr );

    string s;
    int tc; cin >> tc;
    while(tc--){
        memset(dss, 0, sizeof dss);
        cin >> s; build(s);
        cout << dssFun(0)-1 << "\n";
    }

    return 0;
}