#include<iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

enum RBT_color { red, black };

class RBT_node
{
public:
	RBT_color color;    // ��ɫ
	int key;            // �ؼ���(��ֵ)
	RBT_node* left;    // ����
	RBT_node* right;    // �Һ���
	RBT_node* parent; // �����

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
	RBT_node* root;//���ڵ�

public:
	RB_tree() :root(NULL) {}

	void pre_order();// ǰ�����"�����"
	void insert(int key);//����ڵ�
	void remove(int key);//ɾ���ڵ�
	void min_max(int& min, int& max);//Ѱ����С���ֵ

private:
	void pre_order(RBT_node* node) const;// ǰ�����"�����"
	void left_rotate(RBT_node*& root, RBT_node* x);//����
	void right_rotate(RBT_node*& root, RBT_node* y);//����
	void insert(RBT_node*& root, RBT_node* node);//���뺯��
	void insert_fix(RBT_node*& root, RBT_node* node);//�����޸�����
	void remove(RBT_node*& root, RBT_node* node);//ɾ������
	void remove_fix(RBT_node*& root, RBT_node* node, RBT_node*parent);//ɾ���޸�����
	RBT_node* search(RBT_node* x, int key);//��������
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
	* �Ժ�����Ľڵ�(x)��������ת
	*
	* ����ʾ��ͼ(�Խڵ�x��������)��
	*      px                              px
	*     /                               /
	*    x                               y
	*   /  \      --(����)-->     / \
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
	* �Ժ�����Ľڵ�(y)��������ת
	*
	* ����ʾ��ͼ(�Խڵ�y��������)��
	*            py                               py
	*           /                                /
	*          y                                x
	*         /  \      --(����)-->      /  \
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

	// 1. �����������һ�Ŷ�������������ڵ���ӵ�����������С�
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

	/*2.������Ľڵ���ɫΪ"��ɫ"��
	Ϊʲô��ɫ�ɺ�ɫ�������Ǻ�ɫ�أ�Ϊʲô�أ��ڻش�֮ǰ��������Ҫ������ϰһ�º���������ԣ�
	(1) ÿ���ڵ�����Ǻ�ɫ�������Ǻ�ɫ��
	(2) ���ڵ��Ǻ�ɫ��
	(3) ÿ��Ҷ�ӽڵ��Ǻ�ɫ��[ע�⣺����Ҷ�ӽڵ㣬��ָΪ�յ�Ҷ�ӽڵ㣡]
	(4) ���һ���ڵ��Ǻ�ɫ�ģ��������ӽڵ�����Ǻ�ɫ�ġ�
	(5) ��һ���ڵ㵽�ýڵ������ڵ������·���ϰ�����ͬ��Ŀ�ĺڽڵ㡣
	������Ľڵ���ɫΪ��ɫ������Υ��"����(5)"����Υ��һ�����ԣ�����ζ��������Ҫ��������Խ�١�
	����������ҪŬ����������������������ʼ��ɣ������˵Ļ�����������һ�ź�����ˡ�*/
	node->color = red;

	//3.ͨ��һϵ�е���ת����ɫ�Ȳ�����ʹ֮���³�Ϊһ�ź������
	insert_fix(root, node);
}

void RB_tree::insert_fix(RBT_node*& root, RBT_node* node)
{
	/*�ڶ����У�������ڵ���ɫΪ"��ɫ"֮�󣬲���Υ��"����(5)"���������׻�Υ����Щ�����أ�
	����"����(1)"����Ȼ����Υ���ˡ���Ϊ�����Ѿ�����Ϳ�ɺ�ɫ�ˡ�
	����"����(2)"����ȻҲ����Υ�����ڵ�һ���У������ǽ���������������������Ȼ��ִ�еĲ�������������ݶ�����������ص㣬�����������ı���ڵ㡣���ԣ����ڵ���Ȼ�Ǻ�ɫ��
	����"����(3)"����Ȼ����Υ���ˡ������Ҷ�ӽڵ���ָ�Ŀ�Ҷ�ӽڵ㣬����ǿսڵ㲢������������Ӱ�졣
	����"����(4)"�����п���Υ���ģ�
	�ǽ���������취ʹ֮"��������(4)"���Ϳ��Խ������¹���ɺ�����ˡ�*/

	//һ������Ľڵ�ĸ��ڵ��Ǻ�ɫ�������ڵ��Ȼ�����游�ڵ���Ϊ��ɫ��������ڵ�Ҳ���ڣ���Ϊ�սڵ㣬Ҳ��Ϊ���ڣ���ɫΪ��ɫ��
	//��һ����Ϊ3������������˼·Ϊ������ɫ�Ľڵ��Ƶ����ڵ㣻Ȼ�󣬽����ڵ���Ϊ��ɫ����
	RBT_node* parent, * gparent;
	while ((parent = node->parent) && parent->color == red)
	{
		gparent = parent->parent;
		//���ڵ�Ϊ����
		if (parent == gparent->left)
		{
			RBT_node* uncle = gparent->right;
			//1.����ڵ��Ǻ�ɫ
			if (uncle && uncle->color == red)
			{
				parent->color = black;
				uncle->color = black;
				gparent->color = red;
				node = gparent;
			}
			//2.����ڵ��Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
			else if (parent->right == node)
			{
				left_rotate(root, parent);
				node = parent;
			}
			//3.����ڵ��Ǻ�ɫ���ҵ�ǰ�ڵ�������
			else
			{
				parent->color = black;
				gparent->color = red;
				right_rotate(root, gparent);
			}
		}
		//���ڵ�Ϊ�Һ���
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

	//��������Ľڵ�ĸ��ڵ��Ǻ�ɫ��ʲôҲ����Ҫ����

	//��������Ľڵ��Ǹ��ڵ�
	//��һ������½����ڵ���Ϊ��ɫ
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

	//node��������
	if (node->left && node->right)
	{
		//��ȡ��̽ڵ�
		RBT_node* replace = node->right;
		while (replace->left)replace = replace->left;

		swap_key(node, replace);
		//��ɾ���ڵ��Ϊreplace

		child = replace->right;
		parent = replace->parent;
		color = replace->color;

		//ɾ��replace
		if(replace==node->right)parent->right = child;
		else parent->left = child;
		if (child)child->parent = parent;

		if (color == black)remove_fix(root, child, parent);

		delete replace;
		return;
	}

	//nodeֻ������
	if (node->left)child = node->left;
	//nodeֻ���Һ��ӻ�û����
	else child = node->right;

	parent = node->parent;
	color = node->color;

	if (child)child->parent = parent;

	//node���Ǹ��ڵ�
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
		//1��node������
		if (parent->left == node)
		{
			brother = parent->right;

			//1.1���ֵܽڵ�Ϊ��ɫ
			if(brother->color==red)
			{
				brother->color = black;
				parent->color = red;
				left_rotate(root, parent);
				brother = parent->right;
			}
			//1.2���ֵܽڵ�Ϊ��ɫ��������������Ҳ��Ϊ��ɫ
			if ((!brother->left || brother->left->color == black) && (!brother->right || brother->right->color == black))
			{
				brother->color = red;
				node = parent;
				parent = node->parent;
			}
			else
			{
				//1.3���ֵܽڵ�Ϊ��ɫ����������Ϊ�죬�Һ���Ϊ��
				if (!brother->right || brother->right->color == black)
				{
					brother->left->color = black;
					brother->color = red;
					right_rotate(root, brother);
					brother = parent->right;
				}
				//1.4���ֵܽڵ�Ϊ��ɫ�������Һ���Ϊ��ɫ
				brother->color = parent->color;
				parent->color = black;
				brother->right->color = black;
				left_rotate(root, parent);
				node = root;
				break;
			}
		}
		//2��node���Һ���
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

void way1(RB_tree &tree, int n, ofstream &time_file)//������˳�����N������������ͬ˳��ɾ��
{
	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.insert(i);
	endTime = clock();//��ʱ����
	
	if (time_file.is_open())
	{
		time_file << "˳�����"<<n<<"����������ʱ�䣺"<< (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.remove(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "˳��ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(RB_tree& tree, int n, ofstream& time_file)//������˳�����N�����������෴˳��ɾ��
{
	for (int i = 1; i <= n; i++)tree.insert(i);

	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = n; i > 0; i--)tree.remove(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "����ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(RB_tree& tree, int n, ofstream& time_file)//�����˳�����N�������������˳��ɾ��
{
	//�õ��������˳������
	int* a = new int[n];
	for (int i = 0; i < n; i++)a[i] = i + 1;
	random_shuffle(a, a + n);
	//�õ����ɾ��˳������
	int* b = new int[n];
	for (int i = 0; i < n; i++)b[i] = i + 1;
	random_shuffle(b, b + n);

	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = 0; i < n; i++)tree.insert(a[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "�������" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 0; i < n; i++)tree.remove(b[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "���ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
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