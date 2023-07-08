#include <iostream>
#include <cassert>

using namespace std;

template <class T>
struct TreeNode {
    T value;
    TreeNode* parent = nullptr;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

template <class T>
void DeleteTree(TreeNode<T>* node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template <class T>
void PrintTree(const TreeNode<T>* root, ostream& out = cout) {
    out << " ( "s;
    out << root->value;
    if (root->left || root->right) {
        if (root->left) {
            PrintTree(root->left, out);
        } else {
            out << "*"s;
        }
        if (root->right) {
            PrintTree(root->right, out);
        } else {
            out << "*"s;
        }
    }
    out << " ) "s;
}

template <class T>
ostream& operator << (ostream& out, const TreeNode<T>* node) {
    PrintTree(node, out);
    return out;
}

template <class T>
bool CheckTreeProperty(TreeNode<T>* node) {
    TreeNode<T>* pnode = node->parent;
    TreeNode<T>* tnode = node;

    while(pnode != NULL) {
        if (pnode->left == tnode){
            if (pnode->value < node->value)
                return false;
        } else {
            if (pnode->value > node->value)
                return false;
         }

        tnode = pnode;
        pnode = pnode->parent;
    }

    bool isLeft, isRight;
    if (node->left != NULL)
        isLeft = _isSearchTree(node->left);
    else 
        isLeft = true;
    if (node->right != NULL)
        isRight = _isSearchTree(node->right);
    else
        isRight = true;

    return isLeft && isRight;
}

template <class T>
TreeNode<T>* begin(TreeNode<T>* node) {
    auto parent_maybe = node;
    while (parent_maybe->parent != nullptr)
    {
        parent_maybe = parent_maybe->parent;
    }
    return minimum(parent_maybe);
}

template <class T>
TreeNode<T>* next(TreeNode<T>* node) {
    if (node->right != nullptr) 
    {
        return minimum(node->right);
    }
    auto var = node->parent;
    auto var_node = node;
    while (var != nullptr && var_node == var->right)
    {
        var_node = var;
        var = var->parent;
    }
    return var;
}

template <class T>
TreeNode<T>* minimum(TreeNode<T>* node)
{
  if (node->left == nullptr)
  {
     return node;
  }
  return minimum(node->left);
}

template <class T>
TreeNode<T>* maximum(TreeNode<T>* node)
{
  if (node->right == nullptr)
     return node;
  return maximum(node->right);      
}

// функция создаёт новый узел с заданным значением и потомками
TreeNode<int>* N(int val, TreeNode<int>* left = nullptr, TreeNode<int>* right = nullptr) {
    auto res = new TreeNode<int>{val, nullptr, left, right};
    if (left) {
        left->parent = res;
    }
    if (right) {
        right->parent = res;
    }

    return res;
}

int main() {
    using T = TreeNode<int>;

    T* root = N(6, N(4, N(3), N(5)), N(8, N(7)));
    cout << root << endl;

    T* iter = begin(root);
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    DeleteTree(root);
}