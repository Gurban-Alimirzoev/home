#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    TreeNode<T>* pnode = node->parent;
    const TreeNode<T>* tnode = node;

    while (pnode != NULL) {
        if (pnode->left.get() == tnode) {
            if (pnode->value < node->value)
                return false;
        }
        else {
            if (pnode->value > node->value)
                return false;
        }

        tnode = pnode;
        pnode = pnode->parent;
    }

    bool isLeft, isRight;
    if (node->left.get() != NULL)
        isLeft = CheckTreeProperty(node->left.get());
    else
        isLeft = true;
    if (node->right.get() != NULL)
        isRight = CheckTreeProperty(node->right.get());
    else
        isRight = true;

    return isLeft && isRight;
}


template <class T>
TreeNode<T>* begin(TreeNode<T>* node) {
    TreeNode<T>* parent_maybe = node;
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
        return minimum(node->right.get());
    }
    auto var = node->parent;
    auto var_node = node;
    while (var != nullptr && var_node == var->right.get())
    {
        var_node = var;
        var = var->parent;
    }
    return var;
}

template <class T>
TreeNode<T>* minimum(TreeNode<T>* node)
{
    if (node->left.get() == nullptr)
    {
        return node;
    }
    return minimum(node->left.get());
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) 
{
    if (left != NULL && right != NULL && left->parent == nullptr && right->parent == nullptr)
    {
        TreeNodePtr<int> res = std::make_unique<TreeNode<int>>(val, move(left), move(right));
        res->left->parent = res.get();
        res->right->parent = res.get();
        return res;
    }
    else if (left != NULL && left->parent == nullptr && right == NULL)
    {
        TreeNodePtr<int> res = std::make_unique<TreeNode<int>>(val, move(left), move(right));
        res->left->parent = res.get();
        return res;
    }
    else if (right != NULL && right->parent == nullptr && left == NULL)
    {
        TreeNodePtr<int> res = std::make_unique<TreeNode<int>>(val, move(left), move(right));
        res->right->parent = res.get();
        return res;
    }
    else if (left == NULL && right == NULL)
    {
        TreeNodePtr<int> res = std::make_unique<TreeNode<int>>(val, move(left), move(right));
        return res;
    }
    else
    {
        throw std::invalid_argument("");
    }
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}