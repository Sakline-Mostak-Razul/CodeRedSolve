// Status       : ACCEPTED
// Source       : http://codeforces.com/problemsets/acmsguru/problem/99999/411
// DS & ALGO    : Suffix Array

#include "bits/stdc++.h"
using namespace std;
#define rint register int

#define ff first
#define se second
const int mx  = 2e3+5;
const int mx1 = 4e3+5;
using ii = pair<int,int>;

int n, m;
string s, p;
int dp[mx][mx];
int dp2[mx][mx];

int sa[mx1],lcp[mx1],pn[mx1];
int c[mx1], cn[mx1], cnt[mx1];

void build(){
    register int i, j, k;
    s += "$"; n = s.size();

    memset(cnt, 0, mx1<<2);
    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<256; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n; i++) sa[--cnt[s[i]]] = i;

    c[sa[0]] = 0; int cls = 1;
    for(i=1; i<n; c[sa[i++]]=cls-1)
        if(s[sa[i]] != s[sa[i-1]]) cls++;
    
    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){
            pn[i] = sa[i] - k;
            if(pn[i] < 0) pn[i] += n;
        }
        
        fill(cnt, cnt+cls, 0);
        for(i=0; i<n; i++) cnt[c[pn[i]]]++;
        for(i=1; i<cls; i++) cnt[i] += cnt[i-1];
        for(i=n-1; ~i; i--) sa[--cnt[c[pn[i]]]] = pn[i];

        cn[sa[0]] = 0; cls = 1;
        for(i=1; i<n; cn[sa[i++]]=cls-1){
            ii cur = {c[sa[i]],   c[(sa[i]   + k)%n]};
            ii pre = {c[sa[i-1]], c[(sa[i-1] + k)%n]};
            if(cur != pre) cls++; 
        }
        for(int i=0; i<n; i++) c[i] = cn[i];
    }
}

void buildLCP(){
    int n = s.size(); 
    memset(lcp, 0, n<<2);

    register int i, j = 0;
    for(int i=0; i<n; i++) pn[sa[i]] = i;
    for(int i=0; i<n; i++){
        if(pn[i]==n-1){j=0; continue;}

        int k = sa[pn[i] + 1];
        while(i+j<n && j+k<n && s[i+j]==s[j+k]) j++;
        lcp[ pn[i] ] = j; if( j ) j--;
    }
}

int id(int x){return x>=m;}

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
    int ans=0, len, pos;
    for(int i=1; i<n; i++){
        if(id(sa[i]) != id(sa[i-1])){
            int lp = lcp[i-1];
            int ps = max(sa[i-1], sa[i]) - m - 1;
            int res = fun(ps, ps+lp-1);
            if(res > ans)
                ans = res, pos = ps, len = ps+lp-1;
        }
    }

    for(int i=pos; i+ans-1<=len; i++)
        if(dp[i][i+ans-1] == ans){
            cout << p.substr(i, ans) << "\n";
            return;
        }
}

int main(int argc, const char** argv) {

    ios::sync_with_stdio(0);
    cin.tie( nullptr );

    cin >> s >> p; 
    m = s.size();
    s += "#" + p;
    
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
    
    build(); 
    buildLCP();
    lcs();

    return 0;
}
