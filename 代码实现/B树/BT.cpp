#include<iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
#include "BTree.h"
using namespace std;

void way1(BTree& tree, int n, ofstream& time_file)//������˳�����N������������ͬ˳��ɾ��
{
	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.Insert(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "˳�����" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.Delete(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "˳��ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(BTree& tree, int n, ofstream& time_file)//������˳�����N�����������෴˳��ɾ��
{
	for (int i = 1; i <= n; i++)tree.Insert(i);

	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = n; i > 0; i--)tree.Delete(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "����ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(BTree& tree, int n, ofstream& time_file)//�����˳�����N�������������˳��ɾ��
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
	for (int i = 0; i < n; i++)tree.Insert(a[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "�������" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 0; i < n; i++)tree.Delete(b[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "���ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
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