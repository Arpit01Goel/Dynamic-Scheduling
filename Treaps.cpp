#include <bits/stdc++.h>
using namespace std;
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

        Node*insert(Node* node, int a,int b) {
            if (!node) return new Node(a,b);
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
            cout << "see me" << endl;
            return nullptr;
        }
        Node* searchRight(Node* node,int b) {
            if (!node) return nullptr;
            if (node->b<b) return nullptr;
            if (node->a <= b && b<=node->b) return node;

            Node* temp = searchRight(node->left, b);
            return ( temp==nullptr? node:temp);
        }
        Node* searchLeft(Node* node, int a) {
            if (!node) return nullptr;
            if (node->a >a) return nullptr;
            if (node->a <=a && node->b >=a) return node;

            Node* temp = searchLeft(node->right,a);
            return (temp==nullptr? node:temp);
        }
        Node* searchBetween(Node* node, int a,int b) {
            if (!node) return nullptr;
            if (node->a <= a && node->b >=b) return node;
            if (node->a > b) return searchBetween(node->left,a,b);
            if (node->b<a) return searchBetween(node->right,a,b);
            cout << "problem" << endl;
            return nullptr;
        }
    public:
        Treap(): root(nullptr) {}
        
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
            root = insert(root,a,b);
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
            }else {
                cout << "wrong qeury" << endl;
            }
        }
};