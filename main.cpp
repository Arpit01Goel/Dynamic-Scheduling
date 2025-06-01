#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

class TimeFormat {
    public:
        string intToDTime(int num) {
            int min = num%60;
            num/=60;
            int hours = num%24;
            num/=24;
            int days = num%31;
            num/=31;
            int month = num;
            
            string answer = (hours<10? "0": "") + to_string(hours) +
                            (min< 10? "0":"") + to_string(min) +
                            (days<10? "0": "") + to_string(days) + 
                            (month< 10? "0": "") + to_string(month);
            return answer;
        } 
        int DTimeToInt(const string nwtime) {
            string time = nwtime.substr(6,2) + nwtime.substr(4,2)+ nwtime.substr(0,2)+nwtime.substr(2,2);
            int answer =  24*31*60*stoi(time.substr(0,2)) 
                         +24*60*   stoi(time.substr(2,2)) 
                         + 60*    stoi(time.substr(4,2))
                         + stoi(time.substr(6,2));
            return answer;
        }
        string print(string str) {
            cout << str.substr(0,2) + ":"+ str.substr(2,2) + "  "  + str.substr(4,2) + "/" + str.substr(6,2);
            return "";
        }
        string print(int num) {
            this->print(intToDTime(num));
            return "";
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
        ~ SegmentTree() {
            this->lazy.clear();
            this->tree.clear();
            this->toProp.clear();
        }

};

class Node {
    public:
        int a,b;
        int priority;
        Node* left,*right;
        Node(int a,int b) {
            this->a = a;
            this->b = b;
            this->left = nullptr;
            this->right = nullptr;
            this->priority = rand();
        }
        ~Node() {
            this->left = nullptr;
            this->right = nullptr;
        }
};

class Treap {
    private:
        Node* root;
        Node* rotateRight(Node* node) {
            Node* temp = node->left;
            node->left = temp->right;
            temp->right = node;
            return temp;
        }
        Node* rotateLeft(Node* node) {
            Node*temp = node->right;
            node->right = temp->left;
            temp->left = node;
            return temp;
        }

        Node* insert(Node* node, int a,int b) {
            if (node==nullptr) return new Node(a,b);
            if (a<node->a) {
                node->left = insert(node->left, a,b);
                if (node->left->priority > node->priority) node = rotateRight(node);
            }else {
                node->right = insert(node->right,a,b);
                if (node->right->priority > node->priority)
                node = rotateLeft(node);
            }
            return node;
        }
        Node* del(Node* node, int a, int b) {
            if (!node) return nullptr;
    
            if (a < node->a) {
                node->left = del(node->left, a, b);
            } else if (a > node->a) {
                node->right = del(node->right, a, b);
            } else if (node->b == b) {
                if (!node->left) return node->right;
                if (!node->right) return node->left;
    
                if (node->left->priority > node->right->priority) {
                    node = rotateRight(node);
                    node->right = del(node->right, a, b);
                } else {
                    node = rotateLeft(node);
                    node->left = del(node->left, a, b);
                }
            }
            return node;
        }
        Node* search(Node* node,int a,int b) {

            if (!node) return nullptr;
            if (b<node->a) return search(node->right, a,b);
            if (a>node->b) return search(node->left, a,b);
            if (a>=node->a && b<=node->b) return node;
            
            return nullptr;
        }
        Node* searchRight(Node* node,int b) {
            if (node==nullptr) return nullptr;
            if (node->b<b) return searchRight(node->right,b);
            if (node->a <= b && b<=node->b) return node;

            Node* temp = searchRight(node->left, b);
            return ( temp==nullptr? node:temp);
        }
        Node* searchLeft(Node* node, int a) {
            if (node==nullptr) return nullptr;
            if (node->a >a) return searchLeft(node->left, a);
            if (node->a <=a && node->b >=a) return node;

            Node* temp = searchLeft(node->right,a);
            return (temp==nullptr? node:temp);
        }
        Node* searchBetween(Node* node, int a,int b) {
            if (node==nullptr) return nullptr;
            if (node->a <= a && node->b >=b) return node;
            if (node->a > b) return searchBetween(node->left,a,b);
            if (node->b<a) return searchBetween(node->right,a,b);
            
            return nullptr;
        }
        void getOverlap(Node* node, int a,int b,vector<pair<int,int>> & output) {
            if (node==nullptr) return;
            if (node->a > b || node->b < a) return;
            //there is overlap
            getOverlap(node->left,a,b,output);
            output.push_back({node->a,node->b});
            getOverlap(node->right, a,b,output);
        }
        void clear(Node* node) {
            if (node==nullptr) return;
            clear(node->left);
            clear(node->right);
            delete node;
        }
    public:
        Treap(): root(nullptr) {}
        void suggestLeft(int a) {
            Node* temp = searchLeft(this->root,a);
            
            cout << temp->a << " " << temp->b << endl;
        }
        ~Treap() {
            clear(this->root);
        }
        void suggestRight(int b) {
            Node* temp = searchRight(this->root, b) ;
            cout << temp->a << " " << temp->b << endl;
        }
        void suggestDuring(int a,int b) {
            Node* temp = searchBetween(this->root ,a,b);
            cout << temp->a << " " << temp->b << endl;
        }
        void del(Node* node) {
            // simply delete node from root
            root = del(root,node->a,node->b);

        }
        void insert(int a,int b) {
            Node* left= searchLeft(root,a);
            Node*right = searchRight(root,b);
            if (left!=nullptr&& left->b +1 == a) {
                //merge them 
                a = left->a;
                del(left);
            }
            if (right!=nullptr && right->a-1 == b) {
                b = right->b;
                del(right);
            }
            this->root = insert(root,a,b);
        }
        void del(int a,int b) {

            Node* node = searchBetween(root,a,b);
            if (node) {
                if (node->a == a && node->b == b) del(node);
                else {
                    vector<pair<int,int>> vec;
                    if (node->a < a) {
                        vec.push_back({node->a,a-1});
                    }
                    if (node->b > b) {
                        vec.push_back({b+1,node->b});
                    }
                    del(node);
                    for (auto i:vec) {
                        insert(i.first,i.second);
                    }
                    
                }
            }
        }
        vector<pair<int,int>> getFreeIntervals(int a,int b) {
            //return all intervals overlap with a and b
            vector<pair<int,int>> answer;
            getOverlap(this->root,a,b,answer);
            return answer;
        }
};



class Room {
    private:
    TimeFormat *timeConverter;
    SegmentTree *SegTree;
    Treap* Trp;
    public:
    Room() {
        timeConverter = new TimeFormat();
        vector<int> arr(24*31*12*60, 0);
        function<int(int,int)> fn1 = [] (int a,int b) {
            return a + b;
        };
        function<void(int&, int,int)> fn2 = [] (int& a,int b,int c) {
            a += b*c;
        };
        int neutralValue = 0;
        SegTree = new SegmentTree(arr,fn1,neutralValue,fn2);
        Trp = new Treap();
        Trp->insert(0,24*31*12*60-1);

    }
    ~Room() {
        delete SegTree;
        delete Trp;
    }
    int book(string a,string b,int id = 1) {
        
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        int occupied = SegTree->query(x,y);
        
        if (occupied) {
            
            return 0;
        }else {
            SegTree->update(x,y,id);
            Trp->del(x,y);  
            return 1;
        }
    }
    int del(string a,string b,int id = 1) {
        //0 not available, 1 done, -1 no access
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        int occupied = SegTree->query(x,y);

        if (occupied% (y-x+1)==0 && occupied/(y-x+1)==id) {
            //id will be prime number so no hacking
            Trp->insert(x,y);
            SegTree->update(x,y,0);
            return 1;
        }else {
            
            return 0;
        }
        
    }
    bool isAvailabe(string a,string b) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        return (SegTree->query(x,y)==0);
    }
    vector<pair<int,int>>  suggest(string a,string b) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        vector<pair<int,int>> vec = Trp->getFreeIntervals(x,y);
        for (auto i:vec) {
            cout << timeConverter->print(i.first) << " to " << timeConverter->print(i.second+1) << endl;
        }
        return vec;
        
    }    
    vector<vector<int>> listBooking(string a, string b) {
        int x = timeConverter->DTimeToInt(a), y = timeConverter->DTimeToInt(b);
        y--;
        vector<vector<int>> output, answer;
        SegTree->fillChart(x,y,output);
        if (output.size()==0) return output;
        int start = output[0][0], last = output[0][1], id = output[0][2];
        for (int i=1;i<output.size();i++) {
            if (last+1 == output[i][0] && id==output[i][2]) {
                last = output[i][1];continue;
            }
            cout << timeConverter->print(start) << " to " << timeConverter->print(last+1) << endl;
            answer.push_back({start,last});
            start = output[i][0];
            last = output[i][1];
            id= output[i][2];
        }
            cout << timeConverter->print(start) << " to " << timeConverter->print(last+1) << endl;
            answer.push_back({start,last});
        return answer;
    }
    void save(string filename) {
        int x = 0, y = 24*31*12*60;
        y--;
        vector<vector<int>> answer;
        SegTree->fillChart(x,y,answer);
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }
        for (auto i:answer) {
            outFile << "book " << timeConverter->intToDTime(i[0])  << " " << timeConverter->intToDTime(i[1]+1) << " " << i[2] << endl;
        }
        outFile.close();
        cout << "Data saved to : " << filename << endl;
    }
    void load(string filename) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }
        string line;
        while (getline(inFile,line)) {
            istringstream iss(line);
            string cmd;
            

            iss>> cmd  ;
            if (cmd=="book") {
                string a,b;
                // int id ;
                iss>> a>> b ;
                bool flag = this->book(a,b);
                cout << (flag==0? "not done" : "booking done successflly") << endl;
            }else if (cmd=="delete") {
                string a,b;
                // int id ;
                iss>> a>> b ;
                bool flag = this->del(a,b);
                cout << (flag==0? "unable to delelte" : "deleting done successflly") << endl;
            } else if (cmd == "suggest") {
                string a,b;
                iss>> a >> b;
                this->suggest(a,b);
            } else if (cmd=="show") {
                string a,b;
                iss>> a >> b;
                this->listBooking(a,b);
            }else  if (cmd=="save") {
                string a;
                iss>> a;
                this->save(a);
            } else if(cmd=="load") {
                string b;
                iss>> b;
                this->load(b);
            } else {
                cout << "wrong command or wrong format! press ctrl+C to exit" << endl;
                exit(0);
            }
        }
    }
};

void run() {
    cout << "Welcome to My Project" << endl;
    cout << "Commands: (book a b) (delete a b) (suggest a b) (show a b) (save filename) (load filename)" << endl;
    cout << "used notation for a and b :: HHMMDDMM (all three are 0 based indexed)"  << endl;
    cout << "used notation for filename: any valid name plus .txt" << endl;
    string line;
    Room* M1 = new Room();
    while (true) {
        cout << "> " ;
        getline(cin,line);
        istringstream iss(line);
        string cmd;
        iss>> cmd;

        if (cmd=="book") {
            string a,b;
            // int id ;
            iss>> a>> b ;
            bool flag = M1->book(a,b);
            cout << (flag==0? "not done" : "booking done successflly") << endl;
        }else if (cmd=="delete") {
            string a,b;
            // int id ;
            iss>> a>> b ;
            bool flag = M1->del(a,b);
            cout << (flag==0? "unable to delelte" : "deleting done successflly") << endl;
        } else if (cmd == "suggest") {
            string a,b;
            iss>> a >> b;
            M1->suggest(a,b);
        } else if (cmd=="show") {
            string a,b;
            iss>> a >> b;
            M1->listBooking(a,b);
        }else  if (cmd=="save") {
            string a;
            iss>> a;
            M1->save(a);
        } else if(cmd=="load") {
            string b;
            iss>> b;
            M1->load(b);
        } else {
            cout << "wrong command or wrong format! press ctrl+C to exit" << endl;
            exit(0);
        }

    }

}


