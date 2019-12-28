#include<iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

enum RBT_color { red, black };

class RBT_node
{
public:
	RBT_color color;    // 颜色
	int key;            // 关键字(键值)
	RBT_node* left;    // 左孩子
	RBT_node* right;    // 右孩子
	RBT_node* parent; // 父结点

	RBT_node(int value, RBT_color c, RBT_node* p, RBT_node* l, RBT_node* r) :
		key(value), color(c), parent(), left(l), right(r) {}
};

void swap_key(RBT_node* a, RBT_node* b)
{
	int tmp = a->key;
	a->key = b->key;
	b->key = tmp;
}

class RB_tree
{
private:
	RBT_node* root;//根节点

public:
	RB_tree() :root(NULL) {}

	void pre_order();// 前序遍历"红黑树"
	void insert(int key);//插入节点
	void remove(int key);//删除节点
	void min_max(int& min, int& max);//寻找最小最大值

private:
	void pre_order(RBT_node* node) const;// 前序遍历"红黑树"
	void left_rotate(RBT_node*& root, RBT_node* x);//左旋
	void right_rotate(RBT_node*& root, RBT_node* y);//右旋
	void insert(RBT_node*& root, RBT_node* node);//插入函数
	void insert_fix(RBT_node*& root, RBT_node* node);//插入修复函数
	void remove(RBT_node*& root, RBT_node* node);//删除函数
	void remove_fix(RBT_node*& root, RBT_node* node, RBT_node*parent);//删除修复函数
	RBT_node* search(RBT_node* x, int key);//搜索函数
};

void RB_tree::pre_order(RBT_node* node)const
{
	if (node)
	{
		if (node == root)cout << node->key << '(' << 'B' << ')';
		else
		{
			if (node->color == black)
				cout << ' ' << node->key << '(' << 'B' << ')';
			else cout << ' ' << node->key << '(' << 'R' << ')';
		}
		pre_order(node->left);
		pre_order(node->right);
	}
}

void RB_tree::pre_order()
{
	if (!root)cout << "Null";
	else pre_order(root);
}

void RB_tree::min_max(int& min, int& max)
{
	RBT_node* find_min, * find_max;
	find_min = root;
	find_max = root;
	while (find_min->left)find_min = find_min->left;
	min = find_min->key;
	while (find_max->right)find_max = find_max->right;
	max = find_max->key;
}

void RB_tree::left_rotate(RBT_node*& root, RBT_node* x)
{
	/*
	* 对红黑树的节点(x)进行左旋转
	*
	* 左旋示意图(对节点x进行左旋)：
	*      px                              px
	*     /                               /
	*    x                               y
	*   /  \      --(左旋)-->     / \
	*  lx   y                          x  ry
	*     /   \                       /  \
	*    ly   ry                     lx  ly
	*
	*
	*/
	RBT_node* y = x->right;

	x->right = y->left;
	if (y->left)y->left->parent = x;

	y->parent = x->parent;
	if (!x->parent)root = y;
	else
	{
		if (x->parent->left == x)x->parent->left = y;
		else x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

void RB_tree::right_rotate(RBT_node*& root, RBT_node* y)
{
	/*
	* 对红黑树的节点(y)进行右旋转
	*
	* 右旋示意图(对节点y进行左旋)：
	*            py                               py
	*           /                                /
	*          y                                x
	*         /  \      --(右旋)-->      /  \
	*        x   ry                           lx   y
	*       / \                                   / \
	*      lx  rx                                rx  ry
	*
	*/
	RBT_node* x = y->left;

	y->left = x->right;
	if (x->right)x->right->parent = y;

	x->parent = y->parent;
	if (!y->parent)root = x;
	else
	{
		if (y->parent->left == y)y->parent->left = x;
		else y->parent->right = x;
	}

	x->right = y;
	y->parent = x;
}

void RB_tree::insert(RBT_node*& root, RBT_node* node)
{
	RBT_node* y = NULL;
	RBT_node* x = root;

	// 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
	while (x)
	{
		y = x;
		if (node->key > x->key)x = x->right;
		else x = x->left;
	}
	node->parent = y;
	if (y)
	{
		if (node->key > y->key)y->right = node;
		else y->left = node;
	}
	else root = node;

	/*2.将插入的节点着色为"红色"。
	为什么着色成红色，而不是黑色呢？为什么呢？在回答之前，我们需要重新温习一下红黑树的特性：
	(1) 每个节点或者是黑色，或者是红色。
	(2) 根节点是黑色。
	(3) 每个叶子节点是黑色。[注意：这里叶子节点，是指为空的叶子节点！]
	(4) 如果一个节点是红色的，则它的子节点必须是黑色的。
	(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
	将插入的节点着色为红色，不会违背"特性(5)"！少违背一条特性，就意味着我们需要处理的情况越少。
	接下来，就要努力的让这棵树满足其它性质即可；满足了的话，它就又是一颗红黑树了。*/
	node->color = red;

	//3.通过一系列的旋转或着色等操作，使之重新成为一颗红黑树。
	insert_fix(root, node);
}

void RB_tree::insert_fix(RBT_node*& root, RBT_node* node)
{
	/*第二步中，将插入节点着色为"红色"之后，不会违背"特性(5)"。那它到底会违背哪些特性呢？
	对于"特性(1)"，显然不会违背了。因为我们已经将它涂成红色了。
	对于"特性(2)"，显然也不会违背。在第一步中，我们是将红黑树当作二叉查找树，然后执行的插入操作。而根据二叉查找数的特点，插入操作不会改变根节点。所以，根节点仍然是黑色。
	对于"特性(3)"，显然不会违背了。这里的叶子节点是指的空叶子节点，插入非空节点并不会对它们造成影响。
	对于"特性(4)"，是有可能违背的！
	那接下来，想办法使之"满足特性(4)"，就可以将树重新构造成红黑树了。*/

	//一、插入的节点的父节点是红色，则插入节点必然存在祖父节点且为黑色，且叔叔节点也存在（若为空节点，也视为存在，颜色为黑色）
	//进一步分为3种情况（其核心思路为：将红色的节点移到根节点；然后，将根节点设为黑色）：
	RBT_node* parent, * gparent;
	while ((parent = node->parent) && parent->color == red)
	{
		gparent = parent->parent;
		//父节点为左孩子
		if (parent == gparent->left)
		{
			RBT_node* uncle = gparent->right;
			//1.叔叔节点是红色
			if (uncle && uncle->color == red)
			{
				parent->color = black;
				uncle->color = black;
				gparent->color = red;
				node = gparent;
			}
			//2.叔叔节点是黑色，且当前节点是右孩子
			else if (parent->right == node)
			{
				left_rotate(root, parent);
				node = parent;
			}
			//3.叔叔节点是黑色，且当前节点是左孩子
			else
			{
				parent->color = black;
				gparent->color = red;
				right_rotate(root, gparent);
			}
		}
		//父节点为右孩子
		else
		{
			RBT_node* uncle = gparent->left;
			if (uncle && uncle->color == red)
			{
				parent->color = black;
				uncle->color = black;
				gparent->color = red;
				node = gparent;
			}
			else if (parent->left == node)
			{
				right_rotate(root, parent);
				node = parent;
			}
			else
			{
				parent->color = black;
				gparent->color = red;
				left_rotate(root, gparent);
			}
		}
	}

	//二、插入的节点的父节点是黑色（什么也不需要做）

	//三、插入的节点是根节点
	//第一种情况下将根节点设为黑色
	root->color = black;
}

void RB_tree::insert(int key)
{
	RBT_node* a = new RBT_node(key, black, NULL, NULL, NULL);
	insert(root, a);
}

void RB_tree::remove(RBT_node*& root, RBT_node* node)
{
	RBT_node* child, * parent;
	RBT_color color;

	//node有两孩子
	if (node->left && node->right)
	{
		//获取后继节点
		RBT_node* replace = node->right;
		while (replace->left)replace = replace->left;

		swap_key(node, replace);
		//待删除节点变为replace

		child = replace->right;
		parent = replace->parent;
		color = replace->color;

		//删除replace
		if(replace==node->right)parent->right = child;
		else parent->left = child;
		if (child)child->parent = parent;

		if (color == black)remove_fix(root, child, parent);

		delete replace;
		return;
	}

	//node只有左孩子
	if (node->left)child = node->left;
	//node只有右孩子或没孩子
	else child = node->right;

	parent = node->parent;
	color = node->color;

	if (child)child->parent = parent;

	//node不是根节点
	if (parent)
	{
		if (parent->left == node)parent->left = child;
		else parent->right = child;
	}
	else root = child;

	if (color == black)remove_fix(root, child, parent);
	delete node;
}

void RB_tree::remove_fix(RBT_node*& root, RBT_node* node, RBT_node* parent)
{
	RBT_node* brother;
	while ((!node || node->color == black) && node != root)
	{
		//1、node是左孩子
		if (parent->left == node)
		{
			brother = parent->right;

			//1.1、兄弟节点为红色
			if(brother->color==red)
			{
				brother->color = black;
				parent->color = red;
				left_rotate(root, parent);
				brother = parent->right;
			}
			//1.2、兄弟节点为黑色，且其两个孩子也都为黑色
			if ((!brother->left || brother->left->color == black) && (!brother->right || brother->right->color == black))
			{
				brother->color = red;
				node = parent;
				parent = node->parent;
			}
			else
			{
				//1.3、兄弟节点为黑色，且其左孩子为红，右孩子为黑
				if (!brother->right || brother->right->color == black)
				{
					brother->left->color = black;
					brother->color = red;
					right_rotate(root, brother);
					brother = parent->right;
				}
				//1.4、兄弟节点为黑色，且其右孩子为红色
				brother->color = parent->color;
				parent->color = black;
				brother->right->color = black;
				left_rotate(root, parent);
				node = root;
				break;
			}
		}
		//2、node是右孩子
		else
		{
			brother = parent->left;
			if (brother->color == red)
			{
				brother->color = black;
				parent->color = red;
				right_rotate(root, parent);
				brother = parent->left;
			}
			if ((!brother->left || brother->left->color == black) && (!brother->right || brother->right->color == black))
			{
				brother->color = red;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!brother->left || brother->left->color == black)
				{
					brother->right->color = black;
					brother->color = red;
					left_rotate(root, brother);
					brother = parent->left;
				}
				brother->color = parent->color;
				parent->color = black;
				brother->left->color = black;
				right_rotate(root, parent);
				node = root;
				break;
			}
		}
	}
	if (node)node->color = black;
}

RBT_node* RB_tree::search(RBT_node* x, int key)
{
	while (x && x->key != key)
	{
		if (key < x->key)x = x->left;
		else x = x->right;
	}
	return x;
}

void RB_tree::remove(int key)
{
	RBT_node* node;
	if ((node = search(root, key)) != NULL)
		remove(root, node);
}

void way1(RB_tree &tree, int n, ofstream &time_file)//按递增顺序插入N个整数，按相同顺序删除
{
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.insert(i);
	endTime = clock();//计时结束
	
	if (time_file.is_open())
	{
		time_file << "顺序插入"<<n<<"个整数所用时间："<< (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.remove(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "顺序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(RB_tree& tree, int n, ofstream& time_file)//按递增顺序插入N个整数，按相反顺序删除
{
	for (int i = 1; i <= n; i++)tree.insert(i);

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = n; i > 0; i--)tree.remove(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "逆序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(RB_tree& tree, int n, ofstream& time_file)//按随机顺序插入N个整数，按随机顺序删除
{
	//得到随机插入顺序数组
	int* a = new int[n];
	for (int i = 0; i < n; i++)a[i] = i + 1;
	random_shuffle(a, a + n);
	//得到随机删除顺序数组
	int* b = new int[n];
	for (int i = 0; i < n; i++)b[i] = i + 1;
	random_shuffle(b, b + n);

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = 0; i < n; i++)tree.insert(a[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机插入" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 0; i < n; i++)tree.remove(b[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

int main()
{
	RB_tree tree;
	ofstream time_file("C:\\Users\\zxy\\Desktop\\bs_tree.txt", ios::app);
	for (int n = 100; n <= 1000000; n *= 10)
	{
		way1(tree, n, time_file);
		way2(tree, n, time_file);
		way3(tree, n, time_file);
	}
	time_file.close();
	return 0;
}