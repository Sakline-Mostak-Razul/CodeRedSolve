// Status       : ACCEPTED
// Source       : https://www.codechef.com/problems/INSQ16F
// DS & ALGO    : Suffix Array

#include "bits/stdc++.h"
using namespace std;
using vi = vector<int>;
using ii = pair<int,int>;

const int lgn=23; // log2(mx);
const int mx=5e6+5;

vector<string> vs;
string s="",t; int n;

int par[lgn][mx];
int sa[mx], pos[mx];
int tpar[mx], tsa[mx], cnt[mx];

int min3(int a, int b, int c){return min(a, min(b,c));}
void build(){n=s.size();
    register int i, j, k, cls = 1; ii x, y;

    for(i=0; i<n;   i++) cnt[s[i]]++;
    for(i=1; i<256; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n;   i++) sa[--cnt[s[i]]] = i;
    for(i=0; i<n; par[0][sa[i++]] = cls-1)
        if(i && s[sa[i]] != s[sa[i-1]]) cls++;

    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){tsa[i] = sa[i] - k;
            if(tsa[i]<0) tsa[i] += n;}
        
        fill(cnt, cnt+cls, 0);
        for(i=0; i<n;   i++) cnt[par[j][tsa[i]]]++;
        for(i=1; i<cls; i++) cnt[i] += cnt[i-1];
        for(i=n-1; ~i;  i--) sa[--cnt[par[j][tsa[i]]]] = tsa[i];
        
        tpar[sa[0]] = 0; cls = 1;
        for(i=1; i<n; tpar[sa[i++]] = cls-1){
            x = {par[j][sa[i]],   par[j][(sa[i]   + k)%n]};
            y = {par[j][sa[i-1]], par[j][(sa[i-1] + k)%n]};
            if(x != y) cls++;
        }
        for(i=0; i<n; i++) par[j+1][i] = tpar[i];
    }
}

int compare(int i, int j, int len){
    int k = __lg(len);
    ii x = {par[k][i], par[k][(i+len-(1<<k))%n]};
    ii y = {par[k][j], par[k][(j+len-(1<<k))%n]};
    return (x < y) ? -1 : (x > y);
}

int LCP(int i, int j){n=s.size();
    int ans = 0, k = __lg(n)-1; 
    for(int v=(1<<k); k>=0 && i<n && j<n; k--, v>>=1)
        if(par[k][i] == par[k][j])
            ans+=v, i+=v, j+=v;
    return ans;
}

int stringMatch(int idx, int len){
    if(len == 0) return 0; n = s.size();
    int lw = 0, hh = s.size()-1, ans1 = 0;
    while( lw <= hh ){
        int m = (lw + hh) / 2;
        if(compare(sa[m], idx, len) >= 0)
            ans1 = m, hh = m - 1;
        else lw = m + 1;
    }
    if(compare(sa[ans1], idx, len) != 0)
        return 0;

    lw = 0; hh = s.size()-1; int ans2=0;
    while( lw <= hh ){
        int m = (lw + hh) / 2;
        if(compare(sa[m], idx, len) <= 0)
            ans2 = m, lw = m + 1;
        else hh = m - 1;
    }
    return ans2-ans1+1;
}

int main(int argc, const char** argv) {

    // ios_base::sync_with_stdio(0);
    // cin.tie(nullptr);
    
    int m; cin >> m;
    for(int i=0; i<m; i++){
        pos[i] = s.size();
        cin >> t; vs.push_back(t);
        s += t + "$";
    }

    build();

    int q, i, j; cin >> q; 
    while(q--){
        cin >> i >> j; i--; j--;
        int lcp = (i==j) ? vs[i].size() : LCP(pos[i], pos[j]);
        lcp = min3(lcp, vs[i].size(), vs[j].size());
        cout << stringMatch(pos[i], lcp) << "\n";
    }

    return 0;
}