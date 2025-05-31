#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

class TimeFormat {
    public:
        string intToDTime(int num) {
            int hours = num%24;
            num/=24;
            int days = num%31;
            num/=31;
            int month = num;
            
            string answer = (hours<10? "0": "") + to_string(hours) +
                            (days<10? "0": "") + to_string(days) + 
                            (month< 10? "0": "") + to_string(month);
            return answer;
        } 
        int DTimeToInt(const string nwtime) {
            string time = nwtime.substr(4,2) + nwtime.substr(2,2)+ nwtime.substr(0,2);
            int answer = 24*31*stoi(time.substr(0,2)) + 24*stoi(time.substr(2,2)) + stoi(time.substr(4,2));
            return answer;
        }
        void print(string str) {
            cout << str.substr(0,2) + ":00 " + str.substr(2,2) + "/" + str.substr(4,2)<< endl;
        }
        void print(int num) {
            this->print(intToDTime(num));
        }
};

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
                //# the property of segment Tree is here 
                tree[idx] = operation(tree[2*idx],tree[2*idx+1]);    //addition
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
                //# the property of segment Tree is here 
                tree[idx] = operation(tree[2*idx], tree[2*idx+1]);    //addition

            }
        }
        int query(int node, int start,int end, int l,int r){
            if (n==0 || start>end || r<l) return 0;
            propagate(node,start,end);
            
            if (start>r || end<l ) return neutralValue;
            if (l<=start && r>=end) {
                return tree[node];
            }
            int mid = start + (end-start)/2;
            int p = query(2*node, start, mid, l, r);
            int q = query(2*node+1, mid+1, end, l, r);
            //# the property of segment Tree is here 
            
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
                lazy[idx] = 0;
        
        }

        void rangeUpdate(int idx, int start,int end, int l,int r,int value) {
            propagate(idx, start,end);
            if (start>r || end<l) {
                //no overlap, exit
                return;
            }
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
            applyLazy(tree[idx], tree[idx*2],tree[idx*2+1]);
            // tree[idx] = operation(tree[idx*2],tree[idx*2+1]);
        }
        void collector(int idx, int start,int end, int l,int r,vector<int> & output) {
            propagate(idx,start,end);
            if (start>end || l>r || end<l || start>r) return;
            if (start==end) output.push_back(tree[idx]);
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
        void fillChart(int l,int r,vector<int> & output) {
            output.clear();
            collector(1,0,n-1,l,r,output);
        }

};


class Room {
    private:
    TimeFormat *timeConverter;
    SegmentTree *SegTree;
    Treap* Trp;
    public:
    Room() {
        vector<int> arr(24*31*12, 0);
        function<int(int,int)> fn1 = [] (int a,int b) {
            return a|b;
        };
        function<void(int&, int,int)> fn2 = [] (int& a,int b,int c) {
            a = b;
        };
        int neutralValue = 0;
        SegTree = new SegmentTree(arr,fn1,neutralValue,fn2);
        Trp = new Treap();
        Trp->insertInterval(0,24*31*12-1);

    }
    void prettyPrint(vector<int> values) {
        const int valuesPerRow = 8; // Number of values per row
        const int boxWidth = 5;     // Width of each box
    
        int totalValues = values.size();
        int rows = (totalValues + valuesPerRow - 1) / valuesPerRow; // Calculate total rows needed
    
        // Print top border
        for (int i = 0; i < valuesPerRow; i++) {
            cout << "+-----";
        }
        cout << "+" << endl;
    
        // Print values row by row
        for (int i = 0; i < totalValues; i++) {
            if (i % valuesPerRow == 0 && i != 0) {
                cout << endl;
                // Print bottom border for the previous row
                for (int j = 0; j < valuesPerRow; j++) {
                    cout << "+-----";
                }
                cout << "+" << endl;
            }
    
            // Print value inside the box
            if (i % valuesPerRow == 0) {
                cout << "|"; // Start of a new row
            }
            cout << " " << setw(3) << values[i] << " |"; // Print value with padding
        }
    
        // Print bottom border for the last row
        cout << endl;
        for (int i = 0; i < valuesPerRow; i++) {
            cout << "+-----";
        }
        cout << "+" << endl;
    }
    
    void show(string a,string b) {
        
        vector<int> ids;
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        SegTree->fillChart(x,y-1,ids);
        
        this->prettyPrint(ids);
    }
    bool Available(string a,string b) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        return SegTree->query(x,y) == 0;
    }
    void book(string a,string b,int id = 1) {
        
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);

        y--;
        
        cout << x << " " << y << endl;
        int occupied = SegTree->query(x,y);
        cout << occupied << endl;
        if (occupied) {

            cout << "slot is already occupied" << endl;

        }else {
            SegTree->update(x,y,id);
            Trp->deleteInterval(x,y);
            
        }
       
        

    }
    void del(string a,string b,int id = 1) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        
        y--;
        int occupied = SegTree->query(x,y);

        if (occupied==id) {
            Trp->insertInterval(x,y);
            SegTree->update(x,y,0);
            
        }else {
            cout << "it was empty or you do not have permission" << endl;
        }
        

    }
    void suggest(string a,string b) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        vector<pair<int,int>> vec = Trp->getFreeIntervalsBetween(x,y);
        for (auto i:vec) {
            cout << "["<< timeConverter->intToDTime(i.first) << " " << timeConverter->intToDTime(i.second+1) << "]" << endl;
        }
    }
    

    
};

int main() {
    Room *M1 = new Room();

    M1->book("000000","000100",5);
    M1->del("000000","040000",5);
    M1->del("040000" , "070000",5);
    M1->suggest("000000","000100");
    

    return 0;
}