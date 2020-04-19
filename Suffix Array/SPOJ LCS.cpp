// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/LCS/
// DS & ALGO    : Suffix Array

#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using pii = pair<int,int>;

const int mx = 500000+5;

string s, t;
int sa[mx],lcp[mx],pn[mx];
int c[mx], cn[mx], cnt[mx];

// constructing suffix array by sort cyclic shifts
void build(){s += "$";

    register int i, j, k;
    int n = s.size(), m = 256; // maximum charecter

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
        
        fill(begin(cnt),begin(cnt)+cls,0);
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

    register int i, j = 0;
    for(int i=0; i<n; i++) pn[sa[i]] = i;
    for(int i=0; i<n; i++){
        if(pn[i]==n-1){j=0; continue;}

        int k = sa[pn[i] + 1];
        while(i+j<n && j+k<n && s[i+j]==s[j+k]) j++;
        lcp[ pn[i] ] = j; if( j ) j--;
    }
}

int main(int argc, char const *argv[]){
    
    ios_base :: sync_with_stdio(false);
    cin.tie ( nullptr );

    cin >> s >> t;
    int n = s.size();

    s += "#" + t;
    int m = s.size();
    build(); buildLCP();

    auto id = [&](int x){return x>=n;};
    
    int ans = 0;
    for(int i=1; i<m; i++)
        if(id(sa[i-1]) != id(sa[i]))
            ans = max(ans, lcp[i-1]);
    cout << ans << "\n";

    return 0;
}
