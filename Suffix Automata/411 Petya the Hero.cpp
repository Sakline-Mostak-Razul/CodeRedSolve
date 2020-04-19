// Status       : ACCEPTED
// Source       : http://codeforces.com/problemsets/acmsguru/problem/99999/411
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;
#define rint register int

#define ff first
#define se second
const int mx = 2e3+5;
using ii = pair<int,int>;

string s, p;
int dp[mx][mx];
int dp2[mx][mx];

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

void build(){
    for(rint i=0; i<sz; i++) 
        cnt[i]=0, st[i].next.clear();

    st[0] = {0,-1}; sz = 1; last = 0;
    for(rint i=0; i<s.size(); i++) 
        sa_extend(s[i]);
}

int fun(int i, int j){
    if(i > j) return 0;
    if(i == j) return 1;
    if(i+1 == j)
        return p[i]==p[j]?2:1;
    if(~dp2[i][j]) return dp2[i][j];
    int ans = dp[i][j];
    ans = max(ans, fun(i+1,j));
    ans = max(ans, fun(i, j-1));
    return dp2[i][j] = ans;
}

void lcs(){
    string str;
    int u, l, ans, pos, len;
    u = l = ans = pos = 0;
    for(int i=0; i<p.size(); i++){
        while(u && !st[u].next.count(p[i]))
            u = st[u].link, l = st[u].len;
        if(st[u].next.count(p[i]))
            u = st[u].next[p[i]], l++;
        int res = fun(i-l+1, i);
        if(res > ans)
            ans = res, len = l, pos = i;
    }

    // cout << ans << " " << len << " " << pos << "\n";
    for(int i=pos-len+1; i+ans-1<=pos; i++)
        if(dp[i][i+ans-1] == ans){
            cout << p.substr(i, ans) << "\n";
            return;
        }
}

int main(int argc, const char** argv) {

    cin >> s >> p; 
    
    for(int i=0; i<p.size(); i++) dp[i][i] = 1;
    for(int i=1; i<p.size(); i++)
        if(p[i-1] == p[i]) dp[i-1][i] = 2;
    for(int len=3; len<=p.size(); len++){
        for(int i=0; i<=p.size()-len; i++){
            int j = i + len - 1;
            if(p[i] == p[j] && dp[i+1][j-1]>0)
                dp[i][j] = 2 + dp[i+1][j-1];
        }
    }

    memset(dp2, -1, sizeof dp2);
    
    build(); lcs();

    return 0;
}
