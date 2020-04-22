// Status       : 
// Source       : https://www.spoj.com/problems/DISUBSTR/
// DS & ALGO    : 

#include<bits/stdc++.h>
using namespace std;

string s; int ans;
struct node{
    int isEnd;
    unordered_map<char, node*> child;
    node():isEnd(0){child.clear();}
} *root;

void insert(int i){
    node *cur = root;
    for(; s[i]; i++){
        int v = s[i];
        if(!cur->child[v])
            cur->child[v] = new node(), ans++;
        cur = cur->child[v];
    }
    cur->isEnd++;
}

bool search(string &s){
    node *cur = root;
    for(int i=0; s[i]; i++){
        int v = s[i];
        if(!cur->child[v])
            return false;
        cur = cur->child[v];
    }
    return cur->isEnd>0;
}

bool isEmpty(node *cur){
    if(cur->isEnd) return false;
    return cur->child.empty()==true;;
}

node* remove(node *cur, string &s, int i){
    if(cur == NULL) return NULL;
    if(i == int(s.size())){
        if(cur->isEnd) cur->isEnd--;
        if(isEmpty(cur))
            delete(cur), cur = NULL;
        return cur;
    }

    int v = s[i];
    cur->child[v]
        = remove(cur->child[v], s, i+1);
    if(isEmpty(cur))
        delete(cur), cur = NULL;
    return cur;
}

void clearAll(node *cur){
    if(!cur) return;
    for(auto &it:cur->child)
        clearAll(it.second);
    cur->child.clear();
    delete(cur);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie( nullptr );

    int tc; cin >> tc;
    while(tc--){
        root = new node();
        cin >> s; ans = 0;
        for(int i=0; i<s.size(); i++)
            insert(i);
        cout << ans << "\n";
        clearAll(root);
    }

    return 0;
}