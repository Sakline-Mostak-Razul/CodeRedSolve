// Status       : ACCEPTED
// Source       : https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3358
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;
#define rint register int

#define ff first
#define se second
const int mx = 5e4+5;
using ii = pair<int,int>;

int sz, last;
struct state {
    int len,link,fpos;
    map<char,int>next;
} st[mx << 1];

int cnt[mx<<1];
set < ii > base;
// number of occurence helper

void sa_extend(char c) {
    int cur = sz++, p = last; cnt[cur] = 1;

    base.insert({st[p].len+1, cur});
    st[cur] = {st[p].len+1, -1, st[p].len+1};

    for(; ~p && !st[p].next.count(c); p=st[p].link)
        st[p].next[c] = cur;
    if(p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if(st[p].len+1 == st[q].len) st[cur].link = q;
        else {
            int cln = sz++;
            st[cln] = st[q];

            cnt[cln] = 0;
            st[cln].len = st[p].len+1;
            base.insert({st[cln].len, cln});

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

void build(string &s){
    base.clear();
    for(int i=0; i<sz; i++) 
        cnt[i]=0, st[i].next.clear();

    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<s.size(); i++) sa_extend(s[i]);
    for(auto it=base.rbegin(); it!=base.rend(); it++){
        cnt[st[it->se].link] += cnt[it->se];
        st[st[it->se].link].fpos = max(
            st[it->se].fpos,
            st[ st[it->se].link ].fpos
        );
    }
}

/*
3
baaaababababbababbab
11
baaaababababbababbab
3
cccccc
0
*/

int main(int argc, const char** argv) {

    cin.tie( nullptr );
    ios_base::sync_with_stdio(false);

    int m; string s;
    while(cin >> m, m){
        cin >> s; build(s);

        int ans = 0, pos = 0;
        for(int i=1; i<sz; i++)
            if(cnt[i]>=m)
                ans = max(ans, st[i].len);
        for(int i=1; i<sz; i++)
            if(cnt[i]>=m && ans==st[i].len)
                pos = max(pos, st[i].fpos-ans);

        if(ans == 0) cout << "none\n";
        else cout << ans << ' ' << pos << '\n';
    }

    return 0;
}