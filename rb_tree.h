//
// Created by kai.chen on 2022/1/1.
//
//
//      1. AVL二叉平衡搜索树
//
//      2. 红黑树
//
#ifndef ALGORITHM_ADVANCED_RB_TREE_H
#define ALGORITHM_ADVANCED_RB_TREE_H
//
// 1. AVL二叉平衡搜索树
//      （请先回顾常规二叉搜索树的查找插入删除 请见 datastruct_algorithm/binary_tree_op.h）
//
//  这里重点讲：AVL的插入
//      要维持平衡的性质，AVL的插入需要特殊的旋转操作
//  先总结一下破坏平衡的情况：
//      (1) 插入节点位于X的左子节点的左子树----左左
//      (2) 插入节点位于X的左子节点的右子树----左右
//      (3) 插入节点位于X的右子节点的左子树----右左
//      (4) 插入节点位于X的右子节点的右子树----右右
//
//  把(1)(4)理解为"外插"(外侧插入), 可以通过"单旋转"调整解决
//  把(2)(3)理解为"内插"(内侧插入), 可以通过"双旋转"调整解决
//
//     --------------------------------------------
//                  单旋转
//     --------------------------------------------
//      解决情况(1)----左左：
//             18(root)
//        14      20
//     12   16
//  (11)待插入节点
//
// ===> 单旋转操作：将节点14提起，18自然下滑，14的右子树挂到18的左侧。


//     --------------------------------------------
//                  双旋转
//     --------------------------------------------
//      解决情况(2)----左右：
//             18(root)
//        14      20
//     12   16
//        (15)待插入节点
//
// ===> 双旋转操作：目标把16提起放到根节点，14退化为16的左子树，18退化为16的右子树
//   ===> 具体可以通过两次单旋转完成： 第一步：16提到14完成一次单旋转，第二步：16提到18完成一次单旋转

template<class K, class V>
struct AVLTreeNode
{
    K _key;
    V _value;
    AVLTreeNode<K, V>* _left;
    AVLTreeNode<K, V>* _right;
    AVLTreeNode<K, V>* _parent;

    int _bf;	// 平衡因子，表示当前右子树比左子树高多少。正常情况为0，1/-1。 当发生插入的时候会出现2/-2此时需要调整

    AVLTreeNode(const K& key, const V& value)
            :_left(NULL)
            ,_right(NULL)
            ,_parent(NULL)
            ,_key(key)
            ,_value(value)
            ,_bf(0){}
};


// 2. RB红黑树
//  （红黑树放弃了AVL的完全平衡的性质，通过增加颜色限制来获得更好的插入删除效率, 经验说明查找效率也几乎和AVL相等）
//  性质：
//      [1]. 每个节点不是红色就是黑色
//      [2]. 根节点为黑色
//      [3]. 如果节点为红色，其子节点必须为黑色。（但黑色结点的子结点可以是黑色的。）
//      [4]. 任一节点至NULL的任何路径，所含之黑节点数必须相同
//
// 这里重点理解：红黑树的插入
//      根据二叉搜索树的规则 和 性质[4]，新插入的节点X一定为红色，所以它的父节点需要是黑色(因为性质3).
//      除了上面这种情况其他情况都会破坏红黑树
//
//  总结一下破坏的情况：(定义 P:父节点，S:伯父节点，G:祖父节点，GG:曾祖父节点）
//      (1). S为黑色，且X为外侧插入。 -> 单旋转P，G 并改变PG颜色
//      (2). S为黑色，且X为内侧插入。 -> 单旋转X，P 并改变GX颜色，再单旋转X，G （其实就是一次双旋转）
//      (3). S为红色，且X为外侧插入。 -> 单旋转P，G 并改变X颜色。判断此时如果GG为黑，一切搞定。否则
//      (4). 否则如果GG为红，还需要继续往上做（把当前发生冲突的红节点当成X），直到不再有父子节点连续为红的情况
//
//   为了避免出现状况4 "父子节点都为红色"而发生连续向RB树的上层结构调整的情况，造成插入效率低下
//      "由上而下的设计" top-down procedure
//     => 假设新增节点A，那么从root往下沿着A的路径，只要看到某节点X的两个子节点都是红色，就把X改成红色，两个子节点改成黑色
//          如果出现X的父节点也是红色，就做像状况(1)/(2)一样做一次 旋转
//        最后插入新节点，就只需要要么直接插，要么插完做一次旋转就可以。
//                  （因为 上面的步骤保证了不会再出现状况4，旋转后连续为红的原因就是两个子节点都红，它被破坏了）
typedef bool color_type;
const color_type RED = false; // 红色节点
const color_type BLACK = true;

template <class K, class V>
struct RBTreeNode{
    color_type _color;

    RBTreeNode<K,V>* _parent;
    RBTreeNode<K,V>* _left;
    RBTreeNode<K,V>* _right;

    K _key;
    V _value;

    RBTreeNode(const K& key, const V& value)
        :_key(key)
        ,_value(value)
        ,_left(NULL)
        ,_right(NULL)
        ,_parent(NULL)
        ,_color(RED)
    {}
};


// -----------------------------------------------------------------------------------------
//
//       AVL树的实现
// -----------------------------------------------------------------------------------------
template<class K, class V>
class AVLTree{
    typedef AVLTreeNode<K, V> Node;
private:
    Node* _root;
public:
    AVLTree():_root(NULL){}
    // 插入
    bool Insert(const K& key, const V& value){
        if (_root == NULL){
            _root = new Node(key, value);
            return true;
        }
        Node* parent = NULL;
        Node* cur = _root;
        // 第一步 找待插入位置的父节点
        while (cur){
            if (cur->_key > key){
                parent = cur;
                cur = cur->_left;
            }else if (cur->_key < key){
                parent = cur;
                cur = cur->_right;
            }else{
                return false;
            }
        }
        // 插入父节点的左侧/右侧
        cur = new Node(key, value);
        if (parent->_key < key){
            parent->_right = cur;
            cur->_parent = parent;
        }else{
            parent->_left = cur;
            cur->_parent = parent;
        }

        // 调整平衡

        // 不断更新平衡因子 并判断是否需要调整
        while(parent){
            if (cur == parent->_right){
                parent->_bf++;
            }else{
                parent->_bf--;
            }

            if (parent->_bf == 0){ // 插完之后当前父节点正好是平衡的，不需要继续调整了
                break;
            }
            else if (parent->_bf == -1 || parent->_bf == 1){ // 插完之后当前父节点不是完全平衡，需要往上找继续看父节点是否失去平衡
                cur = parent;
                parent = parent->_parent;
            }
            // -2/2  // 插完之后当前父节点失去平衡，开始调整
            else {
                if (parent->_bf == 2 && cur->_bf == 1) { // 情况4---右右
                    RotateL(parent);
                } else if (parent->_bf == 2 && cur->_bf == -1) { // 情况3---右左
                    RotateRL(parent);
                } else if (parent->_bf == -2 && cur->_bf == -1) { // 情况1---左左
                    RotateR(parent);
                } else if (parent->_bf == -2 && cur->_bf == 1) { // 情况2---左右
                    RotateLR(parent);
                }
                break; // 调完一次就平衡了，不用继续
            }
        }
        return true;
    }

    void RotateR(Node* parent){
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* ppNode = parent->_parent;

        // 将subL自然提起，parent下滑，将subLR给parent的左子树
        parent->_left = subLR;
        if (subLR) subLR->_parent = parent;

        subL->_right = parent;
        parent->_parent = subL;

        // 最后提起subL的过程，ppNode从指向parent改到指向subL
        if (_root == parent){
            _root =  subL;
            subL->_parent = NULL;
        }
        else{
            if (ppNode->_right == parent){
                ppNode->_right = subL;
            }
            else{
                ppNode->_left = subL;
            }

            subL->_parent = ppNode;
        }

        subL->_bf = parent->_bf = 0;
    }


    void RotateL(Node* parent){
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Node* ppNode = parent->_parent;

        parent->_right = subRL;
        if(subRL)
            subRL->_parent = parent;

        subR->_left = parent;
        parent->_parent = subR;

        if (parent == _root){
            _root = subR;
            _root->_parent = NULL;
        }
        else{
            if (ppNode->_left == parent){
                ppNode->_left = subR;
            }
            else{
                ppNode->_right = subR;
            }

            subR->_parent = ppNode;
        }

        parent->_bf = subR->_bf = 0;
    }

    void RotateRL(Node* parent){
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        int bf = subRL->_bf;

        RotateR(parent->_right);
        RotateL(parent);

        if (bf == 0){
            subRL->_bf = subR->_bf = parent->_bf = 0;
        }
        else if (bf == 1){
            subR->_bf = 0;
            parent->_bf = -1;
            subRL->_bf = 0;
        }
        else if (bf == -1){
            parent->_bf = 0;
            subR->_bf = 1;
            subRL->_bf = 0;
        }
        else{
            assert(false);
        }
    }

    void RotateLR(Node* parent){
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        int bf = subLR->_bf;

        RotateL(parent->_left);
        RotateR(parent);

        if (bf == 0){
            subLR->_bf = subL->_bf = parent->_bf = 0;
        }
        else if (bf == 1){
            parent->_bf = 0;
            subL->_bf = -1;
            subLR->_bf = 0;
        }
        else if (bf == -1){
            parent->_bf = 1;
            subL->_bf = 0;
            subLR->_bf = 0;
        }
        else{
            assert(false);
        }
    }

    void _InOrder(Node* root){
        if (root == NULL) return;

        _InOrder(root->_left);
        cout<<root->_key<<" ";
        _InOrder(root->_right);
    }

    void InOrder(){
        cout<<"avl_tree inorder result: ";
        _InOrder(_root);
        cout<<endl;
    }

    bool _IsBalance(Node* root, int& height){
        if (root == NULL){
            height = 0;
            return true;
        }

        int leftHeight = 0;
        int rightHeight = 0;
        if (_IsBalance(root->_left, leftHeight)
            && _IsBalance(root->_right, rightHeight)){
            assert(rightHeight-leftHeight == root->_bf);

            height = leftHeight > rightHeight ? leftHeight+1 : rightHeight+1;
            return abs(leftHeight-rightHeight) < 2;
        }
        else{
            return false;
        }
    }

    bool IsBalance(){
        int height = 0;
        return _IsBalance(_root, height);
    }
};

void TestAVLtree(){
    int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};    //{16, 3, 7, 11, 9, 26, 18, 14, 15};
    AVLTree<int, int> t{};
    for (size_t i = 0; i < sizeof(a)/sizeof(int); ++i){
        t.Insert(a[i], i);
        // cout<<a[i]<<":"<<t.IsBalance()<<endl;
    }

    t.InOrder(); // 中序遍历
    cout<<"isBalanced: "<<t.IsBalance()<<endl; // 判断平衡性
}



// -----------------------------------------------------------------------------------------
//
//       RB红黑树的实现
// -----------------------------------------------------------------------------------------
template<class K, class V>
struct RBTree{
    typedef RBTreeNode<K ,V> Node;
private:
    Node* _root;
public:
    RBTree():_root(NULL){}

    // 插入
    bool Insert(const K& key, const V& value){
        if (_root == NULL){
            _root = new Node(key, value);
            _root->_color = BLACK;

            return true;
        }

        Node* parent = NULL;
        Node* cur = _root;
        while (cur){
            if (cur->_key < key){
                parent = cur;
                cur = cur->_right;
            }
            else if (cur->_key > key){
                parent = cur;
                cur = cur->_left;
            }
            else{
                return false;
            }
        }

        cur = new Node(key, value);
        cur->_color = RED;
        if (parent->_key < key){
            parent->_right = cur;
            cur->_parent = parent;
        }
        else{
            parent->_left = cur;
            cur->_parent = parent;
        }

        // 调整
        while (parent && parent->_color == RED){
            Node* grandfather = parent->_parent;
            if (parent == grandfather->_left){
                Node* uncle = grandfather->_right;
                // 1.uncle存在且为红
                // 2.uncle不存在 或 为黑
                if (uncle && uncle->_color == RED){
                    parent->_color = uncle->_color = BLACK;
                    grandfather->_color = RED;

                    // 改颜色后，继续沿着往上更新
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else{
                    if(parent->_right == cur){
                        RotateL(parent);
                        swap(cur, parent);
                    }

                    RotateR(grandfather);
                    parent->_color = BLACK;
                    grandfather->_color = RED;
                }
            }
            else{
                Node* uncle = grandfather->_left;
                // 1. uncle存在 且为红
                // 2. uncle不存在 或为黑
                if (uncle && uncle->_color == RED){
                    parent->_color = BLACK;
                    uncle->_color = BLACK;
                    grandfather->_color = RED;

                    // 改颜色后，继续沿着往上更新
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else{
                    if(cur == parent->_left){
                        RotateR(parent);
                        swap(cur, parent);
                    }

                    RotateL(grandfather);
                    parent->_color = BLACK;
                    grandfather->_color = RED;
                }
            }
        }

        _root->_color = BLACK;

        return true;
    }

    void RotateR(Node* parent){
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* ppNode = parent->_parent;

        parent->_left = subLR;
        if (subLR) subLR->_parent = parent;

        subL->_right = parent;
        parent->_parent = subL;

        if (_root == parent){
            _root =  subL;
            subL->_parent = NULL;
        }
        else{
            if (ppNode->_right == parent){
                ppNode->_right = subL;
            }
            else{
                ppNode->_left = subL;
            }

            subL->_parent = ppNode;
        }
    }


    void RotateL(Node* parent){
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Node* ppNode = parent->_parent;

        parent->_right = subRL;
        if(subRL) subRL->_parent = parent;

        subR->_left = parent;
        parent->_parent = subR;

        if (parent == _root){
            _root = subR;
            _root->_parent = NULL;
        }
        else{
            if (ppNode->_left == parent){
                ppNode->_left = subR;
            }
            else{
                ppNode->_right = subR;
            }

            subR->_parent = ppNode;
        }

    }

    void InOrder(){
        cout<<"rb_tree inorder result: ";
        _InOrder(_root);
    }

    void _InOrder(Node* root){
        if (root == NULL){
            return;
        }

        _InOrder(root->_left);
        cout<<root->_key<<" ";
        _InOrder(root->_right);
    }
};

void TestRBTree()
{
    int a[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    RBTree<int, int> t;
    for (size_t i = 0; i < sizeof(a)/sizeof(a[0]); ++i){
        t.Insert(a[i], i);
    }

    t.InOrder();
}


#endif //ALGORITHM_ADVANCED_RB_TREE_H
