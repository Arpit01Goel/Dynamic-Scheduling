#include <bits/stdc++.h>
using namespace std;

class SegmentTree {
    private:
        vector<int> tree,lazy,toProp;
        int n;

        // [](int a, int b) { return a + b; }
        // [](int a, int b) { return min(a, b); }
        function<int(int,int)> operation;
        int neutralValue;
        function<void(int&,int,int)>  applyLazy;

        void build(vector<int> &arr,int idx, int start, int end) {
            if (arr.size()==0) return;
            if (start==end) {
                
                tree[idx] = arr[start];
            }else {
                int mid = start + (end-start)/2;
                build(arr,2*idx, start, mid);
                build (arr, 2*idx+1, mid+1, end);
                
                tree[idx] = operation(tree[2*idx],tree[2*idx+1]);    
            }
        }
        void update(int idx, int start, int end, int pos, int val) {
            propagate(idx,start,end);
            if (start==end) {
                tree[idx] = val;
                
            }else {
                int mid = start + (end-start)/2;
                if (pos <= mid) {
                    update(2 * idx, start, mid, pos, val);
                } else {
                    update(2 * idx + 1, mid + 1, end, pos, val);
                }
                
                tree[idx] = operation(tree[2*idx], tree[2*idx+1]);   

            }
        }
        int query(int node, int start,int end, int l,int r){
            if ( start>end || r<l) return neutralValue;
            if (start>r || end<l ) return neutralValue;
            propagate(node,start,end);
            
            if (l<=start && r>=end) {
                return tree[node];
            }
            int mid = start + (end-start)/2;
            int p = query(2*node, start, mid, l, r);
            int q = query(2*node+1, mid+1, end, l, r);
                        
            return operation(p,q);
        }
        
        
        void propagate(int idx, int start, int end) {
            if (toProp[idx]==0) return ; 
                applyLazy(tree[idx], lazy[idx], end-start+1);

                if (start!=end) {
                    toProp[2*idx] =1;
                    toProp[2*idx+1] = 1;
                    applyLazy(lazy[2*idx], lazy[idx],1);
                    applyLazy(lazy[2*idx+1], lazy[idx],1);
                }
                toProp[idx] = 0;
                lazy[idx] = neutralValue;
        
        }

        void rangeUpdate(int idx, int start,int end, int l,int r,int value) {
            if (start>r || end<l) {
                //no overlap, exit
                return;
            }
            propagate(idx, start,end);
            if (l<=start && end<=r) {

                //complete overlap
                toProp[idx] = 1;
                applyLazy(lazy[idx], value,1); 
                propagate(idx, start, end);
                return;

            }
            //partial overlap
            int mid = (start+end)/2;
            rangeUpdate(2*idx, start,mid,l,r,value);
            rangeUpdate(2*idx+1, mid+1, end, l, r, value);
            tree[idx] = operation( tree[idx*2],tree[idx*2+1]);
            
        }
        void collector(int idx, int start,int end, int l,int r,vector<vector<int>> & output) {
            if (start>end || l>r || end<l || start>r) return;
            propagate(idx,start,end);
            if (tree[idx]==0) return;
            if (start==end) {output.push_back({start,end,tree[idx]}); return;}
            if (tree[idx]%(end-start+1)==0) {
                output.push_back({start,end,tree[idx]/(end-start+1)});
                return;
            }
            int mid = start + (end-start)/2;
            collector(2*idx,start,mid,l,r,output);
            collector(2*idx+1,mid+1,end,l,r,output);
        }

    public:
        SegmentTree(vector<int> &arr, function<int(int,int)> opr, int neutral,function<void(int&,int,int)>opr2) {
            if (arr.empty()) {
                n = 0;
                return;
            }
            this->operation = opr;
            this->neutralValue = neutral;
            this->applyLazy = opr2;
            n = arr.size();
            this->lazy= vector<int> (4*n,0);
            tree = vector<int> (4*n,0);
            toProp = vector<int> (4*n,0);
            build(arr,1,0,n-1);
        }
        void update(int idx, int val) {
            update(1,0,n-1,idx, val);
        }
        void update(int l,int r,int val) {
            rangeUpdate(1,0,n-1,l,r,val);
        }
        int query(int l,int r){
            return query(1,0,n-1,l,r);
        }
        void fillChart(int l,int r,vector<vector<int>> & output) {
            output.clear();
            collector(1,0,n-1,l,r,output);
        }

};