// Status       : ACCEPTED
// Source       : https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3358
// DS & ALGO    : String Hashing

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

using ull = unsigned long long;
#define rint register int

const int mx = 1e5+5;
struct HASH{
    int n; ull base = 31;
    ull pw[mx], H[mx], RH[mx];

    inline void build(string &s){
        pw[0] = 1; n = s.size();
        for(rint i=1; i<=n; i++)
            pw[i] = (pw[i-1] * base);
        for(rint i=0; i<n; i++){
            H[i+1]  = (i ? H[i]  * base : 0) + s[i];
            RH[i+1] = (i ? RH[i] * base : 0) + s[n-i-1];
        }
    }

    inline ull getH(int i, int sz){
        return H[i+sz] - H[i] * pw[sz];}
    inline ull getRH(int i, int sz){
        return RH[i+sz] - RH[i] * pw[sz];}
    inline ull delChar(int i){
        ull h = getH(i+1, n-i-1);
        if(i) h += getH(0, i) * pw[n-i-1];
        return h;
    }
}HS;

struct chash{
    int operator() (ull x) const{
        return (x ^ (x >> 32) ^ (x >> 64));}
};

int n, m; string s;
gp_hash_table<ull, int, chash> table;

bool isOK(int sz){
    table.clear();
    for(rint i=n-sz; i>=0; i--){
        ull hs = HS.getH(i, sz);
        table[hs]++;
        if(table[hs] == m)
            return true;
    }
    return false;
}

int main(int argc, const char** argv) {

    cin.tie( nullptr );
    ios_base::sync_with_stdio(false);

    while(cin >> m, m){
        cin >> s; n = s.size();
        
        HS.build(s);

        int lw=1, hh=n, ans=-1;
        while(lw <= hh){
            int mid = (lw + hh) / 2;
            if(isOK(mid))
                ans = mid, lw = mid+1;
            else hh = mid - 1;
        }

        if(ans == -1){
            cout << "none\n";
            continue;
        }

        table.clear(); int pos = 0;
        for(rint i=0; i<=n-ans; i++){
            ull h = HS.getH(i, ans);
            table[h]++;
            if(table[h] >= m)
                pos = i;
        }

        cout << ans << ' ' << pos << '\n';
    }

    return 0;
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