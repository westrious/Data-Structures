#include<iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
#include "BTree.h"
using namespace std;

void way1(BTree& tree, int n, ofstream& time_file)//按递增顺序插入N个整数，按相同顺序删除
{
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.Insert(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "顺序插入" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.Delete(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "顺序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(BTree& tree, int n, ofstream& time_file)//按递增顺序插入N个整数，按相反顺序删除
{
	for (int i = 1; i <= n; i++)tree.Insert(i);

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = n; i > 0; i--)tree.Delete(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "逆序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(BTree& tree, int n, ofstream& time_file)//按随机顺序插入N个整数，按随机顺序删除
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
	for (int i = 0; i < n; i++)tree.Insert(a[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机插入" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 0; i < n; i++)tree.Delete(b[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

int main()
{
	BTree tree;
	ofstream time_file("C:\\Users\\zxy\\Desktop\\b_tree.txt", ios::app);
	for (int n = 100; n <= 1000000; n *= 10)
	{
		way1(tree, n, time_file);
		way2(tree, n, time_file);
		way3(tree, n, time_file);
	}
	time_file.close();
	return 0;
}