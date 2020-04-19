// Status       : ACCEPTED
// Source       : https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3358
// DS & ALGO    : Suffix Array

#include "bits/stdc++.h"
using namespace std;
#define rint register int

using vi = vector<int>;
using ii = pair<int,int>;

const int mx = 1e5+5;
const int lgn= __lg(mx)+5;

string s; int n;
int par[lgn][mx];
int sa[mx], lcp[mx];

int tpar[mx], tsa[mx], cnt[mx];
void build(){

    s+="$"; n=s.size();
    register int i, k;
    int cls = 1; ii x, y;
    memset(cnt, 0, mx<<2);
    
    for(i=0; i<n;   i++) cnt[ s[i] ]++;
    for(i=1; i<256; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n;   i++) sa[ --cnt[ s[i] ] ] = i;
    for(i=0; i<n; par[0][sa[i++]] = cls-1)
        if(i && s[sa[i]] != s[sa[i-1]]) cls++;

    for(k=0; (1<<k)<n; k++){
        for(i=0; i<n; i++){
            tsa[i] = sa[i] - (1<<k);
            if(tsa[i]<0) tsa[i] += n;}
        
        memset(cnt, 0, cls<<2);
        for(i=0; i<n;   i++) cnt[ par[k][tsa[i]] ]++;
        for(i=1; i<cls; i++) cnt[i] += cnt[i-1];
        for(i=n-1; ~i;  i--) sa[--cnt[par[k][tsa[i]]]] = tsa[i];
        
        tpar[sa[0]] = 0; cls = 1;
        for(i=1; i<n; tpar[sa[i++]] = cls-1){
            x = {par[k][sa[i]],   par[k][(sa[i]   + (1<<k))%n]};
            y = {par[k][sa[i-1]], par[k][(sa[i-1] + (1<<k))%n]};
            if(x != y) cls++;
        }
        for(i=0; i<n; i++) par[k+1][i] = tpar[i];
    }
}

int LCP(int i, int j){
    if(i == j) return n-j-1; // make sure what u return
    int ans = 0, k = __lg(n);
    for(int v=(1<<k); k>=0 && i<n && j<n; k--,v>>=1)
        if(par[k][i] == par[k][j])
            ans+=v, i+=v, j+=v;
    return ans;
}

int st[mx][lgn];
void buildST(){
    for(rint i=0; i<n; i++) st[i][0] = sa[i];
    for(rint j=1; (1<<j)<n; j++)
    for(rint i=0; i+(1<<j)<=n; i++)
        st[i][j] = max(
            st[i][j-1],
            st[i+(1<<(j-1))][j-1]
        );
}

int query(int i, int j){
    int k = __lg(j - i + 1);
    return max(
        st[i][k],
        st[j-(1<<k)+1][k]
    );
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

    int m;
    while(cin >> m, m){
        cin >> s; 

        build(); buildST();

        int ans = 0, pos = 0;
        for(rint i=1; i<=n-m; i++)
            ans = max(ans, LCP(sa[i],sa[i+m-1]));
        for(rint i=1; i<=n-m; i++)
            if(LCP(sa[i],sa[i+m-1]) == ans)
                pos = max(pos, query(i, i+m-1));
        
        if(ans == 0) cout << "none\n";
        else cout << ans << ' ' << pos << "\n";
    }

    return 0;
}