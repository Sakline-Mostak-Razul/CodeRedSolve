// Status       : ACCEPTED
// Source       : https://www.codechef.com/problems/TASTR
// DS & ALGO    : Suffix Array


#include "bits/stdc++.h"
using namespace std;

using ll = long long;
#define int ll

using vi = vector<int>;
using pii = pair<int,int>;

const int mx = 2e5+5;

string s;
int sa[mx],lcp[mx],pn[mx];
int c[mx], cn[mx], cnt[mx];

// constructing suffix array by sort cyclic shifts
void build(){s += "$";

    register int i, j, k;
    int n = s.size(), m = 256; // maximum charecter

    memset(cnt, 0, mx<<2);
    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<m; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n; i++) sa[--cnt[s[i]]] = i;

    c[sa[0]] = 0; int cls = 1;
    for(i=1; i<n; c[sa[i++]]=cls-1)
        if(s[sa[i]] != s[sa[i-1]]) cls++;
    
    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){
            pn[i] = sa[i] - k;
            if(pn[i] < 0) pn[i] += n;
        }
        
        fill(cnt,cnt+cls,0);
        for(i=0; i<n; i++) cnt[c[pn[i]]]++;
        for(i=1; i<cls; i++) cnt[i] += cnt[i-1];
        for(i=n-1; ~i; i--) sa[--cnt[c[pn[i]]]] = pn[i];

        cn[sa[0]] = 0; cls = 1;
        for(i=1; i<n; cn[sa[i++]]=cls-1){
            pii cur = {c[sa[i]],   c[(sa[i]   + k)%n]};
            pii pre = {c[sa[i-1]], c[(sa[i-1] + k)%n]};
            if(cur != pre) cls++; 
        }
        for(int i=0; i<n; i++) c[i] = cn[i];
    }
}

void buildLCP(){
    int n = s.size(); 
    memset(lcp, 0, mx<<2);

    register int i, j = 0;
    for(int i=0; i<n; i++) pn[sa[i]] = i;
    for(int i=0; i<n; i++){
        if(pn[i]==n-1){j=0; continue;}

        int k = sa[pn[i] + 1];
        while(i+j<n && j+k<n && s[i+j]==s[j+k]) j++;
        lcp[ pn[i] ] = j; if( j ) j--;
    }
}

ll fun(string str){
    s = str; build(); buildLCP();

    ll n = s.size();
    ll ans = (n * (n - 1)) / 2;
    for(int i=0; i<n; i++)
        ans -= lcp[i];
    // cout << ans << "\n";
    return ans;
}

signed main() {

    ios_base :: sync_with_stdio(0);
    cin.tie( nullptr );

    string x, y; cin >> x >> y;
    ll xl = x.size()+1, yl = y.size()+1;

    ll ansx = fun(x), ansy = fun(y);
    ll ans  = fun(x + "#" + y) - (xl * yl); 

    // cout << ansx << ' '
    //      << ansy << ' '
    //      << ans << "\n";

    ans = 2*ans - ansx - ansy;
    cout << ans << "\n";

    return 0;
}