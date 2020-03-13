//
// Created by 孙卓文 on 2020/1/8.
//

#ifndef RBT_RBT_H
#define RBT_RBT_H

#include <iostream>
#include <queue>

template <typename Comparable>
class RedBlackTree;

template <typename Comparable>
class RedBlackNode;

template <typename Comparable>
class RedBlackTree
{
    typedef RedBlackNode<Comparable> Node;

public:
    explicit RedBlackTree(const Comparable &negInf);    //构造函数传入的参数是很大的负数
    ~RedBlackTree();
    enum {RED, BLACK};                                  //节点颜色

    void rotateWithLeftChild(Node * & k2) const;        //向右转，k2为旋转基点
    void rotateWithRightChild(Node * & k1) const;       //向左转，k1为旋转基点

    void doubleRotateWithLeftChild(Node * & k3) const;  //向右双旋转，k3为旋转基点
    void doubleRotateWithRightChild(Node * & k1) const; //向左双旋转，k1为旋转基点

    void handleReorient(const Comparable & item);
    RedBlackNode<Comparable> *rotate(const Comparable &item, Node *theParent) const;

    void Insert(const Comparable &x);
    void Show() const;

//private:
    Node *header;
    Node *nullNode;                                     //代表空节点

    Node *current;                                      //指向当前节点
    Node *parent;                                       //指向当前节点的父节点
    Node *grand;                                        //指向当前节点的祖父节点
    Node *great;                                        //指向当前节点的曾祖父节点
};


template <typename Comparable>
class RedBlackNode
{
    friend class RedBlackTree<Comparable>;
public:
    explicit RedBlackNode(const Comparable &theElement = Comparable(),
                          RedBlackNode *lt = nullptr,
                          RedBlackNode *rt = nullptr,
                          int c = RedBlackTree<Comparable>::BLACK)
            :element(theElement), left(lt), right(rt), color(c){}
//private:
    Comparable      element;
    RedBlackNode    *left;
    RedBlackNode    *right;
    int             color;                              //节点颜色
};

template<typename Comparable>
RedBlackTree<Comparable>::RedBlackTree(const Comparable &negInf) {
    nullNode = new Node();
    RedBlackNode<Comparable> *tmp = nullNode;
    nullNode->left = tmp;
    nullNode->right = tmp;
    header = new Node(negInf);
    header->left = nullNode;
    header->right = nullNode;
}

template<typename Comparable>
RedBlackTree<Comparable>::~RedBlackTree() {
    delete nullNode;
    delete header;
}

template <typename  Comparable>
void RedBlackTree<Comparable>::Insert(const Comparable &x) {
    current = header;
    parent = header;
    grand = header;
    nullNode->element = x;

    while (current->element != x) {
        great = grand;
        grand = parent;
        parent = current;
        current = x < current->element ? current->left:current->right;

        //如何左右子节点的颜色都是红色，则改变右孩子的颜色
        if (current->left->color == RED && current->right->color == RED) {
            handleReorient(x);
        }
    }
    if (current != nullNode) {              //在原有的红黑数找到了一个节点，该节点的值为将要出入的值
        std::cout << "插入失败" << std::endl;
        return;
    }
    current = new Node(x, nullNode, nullNode);
    if (x < parent->element) parent->left = current;
    else parent->right = current;
    //自动平衡
    handleReorient(x);                      //插入的是外自节点，则单旋转，插入的是内子节点，则双旋转

}

template<typename Comparable>
void RedBlackTree<Comparable>::rotateWithLeftChild(RedBlackTree::Node *&k2) const {
    Node *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2 = k1;
}

template<typename Comparable>
void RedBlackTree<Comparable>::rotateWithRightChild(RedBlackTree::Node *&k1) const {
    Node *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
}

template<typename Comparable>
void RedBlackTree<Comparable>::doubleRotateWithLeftChild(RedBlackTree::Node *&k3) const {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template<typename Comparable>
void RedBlackTree<Comparable>::doubleRotateWithRightChild(RedBlackTree::Node *&k1) const {
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template<typename Comparable>
void RedBlackTree<Comparable>::handleReorient(const Comparable &item) {
    //变色，若当前节点是黑色，当前节点的两个儿子节点都是红色，则将当前节点变为红色，两个儿子节点都变成黑色
    current->color = RED;
    current->left->color = BLACK;
    current->right->color = BLACK;

    if (parent->color == RED) {
        grand->color = RED;
        //若一个节点比爸爸小，比爷爷小，这两种情况不同时存在（!=），则为内部孙子
        //内部孙子，多加一次旋转
        if (item < grand->element != item < parent->element) {
            parent = rotate(item, grand);
        }
        current = rotate(item, great);
        current->color = BLACK;
    }
    header->right->color = BLACK;
}

template<typename Comparable>
RedBlackNode<Comparable> *RedBlackTree<Comparable>::rotate(const Comparable &item, RedBlackTree::Node *theParent) const {
    if (item < theParent->element) {
        if (item < theParent->left->element)
            rotateWithLeftChild(theParent->left);      //左子树向右转
        else

            rotateWithRightChild(theParent->left);     //左子树向左转
        return theParent->left;
    }
    else {
        if (item < theParent->right->element)
            rotateWithLeftChild(theParent->right);     //右子树向右转
        else
            rotateWithRightChild(theParent->right);    //右子树向左转
        return theParent->right;
    }
}

template<typename Comparable>
void RedBlackTree<Comparable>::Show() const {
    std::queue<Node*> q;
    q.push(header->right);
    int level = 0;
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            auto *tmp = q.front();
            q.pop();
            //（数据，颜色，层数）
            std::cout << "(" << tmp->element << "," << tmp->color  << "," << level << ")" << "\t";
            if (tmp->left != nullNode) q.push(tmp->left);
            if (tmp->right != nullNode) q.push(tmp->right);
        }
        level ++;
    }
    std::cout << std::endl;
}


#endif //RBT_RBT_H
