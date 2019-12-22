//BTree.h�ļ�������ʹ����ģ������û����������ʵ�ַ���  
#pragma once  
#include <queue>  
using namespace std;

//B���Ľ�㶨��  
struct BTreeNode
{
    int num;               //�ؼ��ָ���    
    int* K;                  //ָ��ؼ�������  
    BTreeNode* parent;  //ָ���׽��  
    BTreeNode** A;      //ָ���ӽ�������ָ��  
    BTreeNode(int n, int m, BTreeNode* p)
    {
        num = n;
        parent = p;
        K = new int[m + 1];           //�����m-1���ؼ��֣�K0���ã�Km�������ڱ�  
        A = new BTreeNode * [m + 1]; //�����m����֧��Am�������ڱ�  
        for (int i = 0; i <= m; i++)
            A[i] = NULL;
    }
    ~BTreeNode()
    {
        delete[] K; K = NULL;
        delete[] A; A = NULL;
    }
};

//�����������Ԫ�鶨��  
struct Triple
{
    BTreeNode* node;  //�ؼ������ڽ��  
    int i;                //�ؼ����±�λ��  
    bool tag;             //�����Ƿ�ɹ�  
    Triple(BTreeNode* nd, int pos, bool t)
    {
        node = nd; i = pos; tag = t;
    }
};

//B������  
class BTree
{
public:
    BTree();
    BTree(int m, BTreeNode* root);
    ~BTree();
    Triple Search(const int& x); //�������ĺ���  
    bool Insert(const int& x);      //������ĺ���  
    bool Delete(const int& x);      //ɾ�����ĺ���  
    void InsertKey(BTreeNode* p, int k, BTreeNode* a, int i);    //����һ����Ԫ��(K,A)  
    void SpliteNode(BTreeNode* p, int* k, BTreeNode** a, int i); //���ѽ��  
    void RightAdjust(BTreeNode* p, BTreeNode* q, int i);  //������Ůȡ�ؼ���  
    void LeftAdjust(BTreeNode* p, BTreeNode* q, int i);   //������Ůȡ�ؼ���  
    void LeftCompress(BTreeNode* p, int i);  //�����ƶ�1��λ��  
    void RightCompress(BTreeNode* p, int i); //�����ƶ�1��λ��  
    void MergeNode(BTreeNode* p, BTreeNode* q, BTreeNode* pR, int i); //�ϲ��������  
private:
    int m_m;                //·�����������������  
    BTreeNode* m_pRoot;  //B���ĸ����  
};
BTree::BTree() //Ĭ�Ϲ��캯��  
{
    m_m = 4;         //Ĭ����4��  
    m_pRoot = NULL;  //������ʼΪ��  
}

BTree::BTree(int m, BTreeNode* root)
{
    m_m = m;
    m_pRoot = root;
}

BTree::~BTree() //�ͷ����еĿռ�  
{
    if (m_pRoot != NULL)
    {
        queue<BTreeNode*> nodeQueue; //���ö��У�����α���B��  
        nodeQueue.push(m_pRoot);         //��������  
        while (nodeQueue.size())
        {
            BTreeNode* p = nodeQueue.front();
            if (p->A[0] != NULL) //����Ҷ��㣬�迼����Ů����ɾ��  
            {
                for (int i = 0; i <= p->num; i++)
                    nodeQueue.push(p->A[i]);
            }
            nodeQueue.pop();
            delete p;
            p = NULL;
        }
    }
}
//�������ܣ� ���ҹؼ���x�Ƿ���B����  
//���������� xΪ���ҵĹؼ���  
//����ֵ��   һ��Triple����(node, i, tag)��tag=true��ʾx���ڽ��r�е�Ki��tag=false��ʾx�������У�r�����һ���������Ľ��  

Triple BTree::Search(const int& x)
{
    int i = 0;  //�±�  
    BTreeNode* p = m_pRoot, * q = NULL;  //�������浱ǰ�������ĸ����  

    while (p != NULL) //һֱ��鵽Ҷ���  
    {
        //n, A0,(K1, A1), (K2, A2), ... (Kn, An)  
        //ȷ��i��ʹ��Ki <= x < Ki+1��K[0]��������  
        //����������䵱ȻҲ����д�� for(i = 1; i <= n && x >= p->K[i]; i++)  
        //����Ϊ����Ki <= x < Ki+1�����ϵʽͳһ������������д�����۲����ĳ��򣬷�������д���������±�������ж�   
        int n = p->num;   //��ǰ���Ĺؼ��ָ���     
        for (i = 0; i < n && x >= p->K[i + 1]; i++)  //���ԸĽ�һ�£��ö��ֲ���  
            ;
        if (x == p->K[i]) //�Ƿ����ҵ��������ж��±꣬i���Ϊn   
            return Triple(p, i, true);
        q = p;
        p = p->A[i];     //������һ�㣬Ki��Ki+1�м��ָ��  
    }
    return Triple(q, i, false); //x�������У��ҵ��˿��Բ���Ľ��λ��  
}
//�������ܣ� ����ؼ���x��B����  
//���������� xΪ����Ĺؼ���  
//����ֵ��   �����Ƿ�ɹ�  

bool BTree::Insert(const int& x)
{
    if (m_pRoot == NULL) //����  
    {
        m_pRoot = new BTreeNode(1, m_m, NULL);  //�µĸ�����1���ؼ���  
        m_pRoot->K[1] = x;    //���Ĺؼ���  
        return true;
    }

    Triple triple = Search(x);     //����Ƿ��Ѵ���  
    if (triple.tag == true) //x����B����  
        return false;

    BTreeNode* p = triple.node, * q; //����ַ  
    //����������Ԫ��(k,a) ����kΪ�ؼ��֣�aΪ����ָ��  
    BTreeNode* a = NULL;
    int k = x;
    int i = triple.i;

    while (1) //�������  
    {
        if (p->num < m_m - 1) //�ؼ��ָ���δ�������ޣ�����ֱ�Ӳ���  
        {
            InsertKey(p, k, a, i); //(k, a)���뵽λ��(Ki, Ai)����  
            return true;
        }
        SpliteNode(p, &k, &a, i); //��p�����ѳ�������㣬һ�������Ϊp������һ����Ϊ��Ԫ��(k,a)���Ա���뵽�����  
        if (p->parent != NULL)     //����㲻Ϊ��  
        {
            q = p->parent; //��ø����  
            for (i = 0; i < q->num && x >= q->K[i + 1]; i++) //ȷ���µĲ���λ��i  
                ;
            p = q;   //������һ��  
        }
        else
        {
            //�Ѿ������˸�����Ҫ�½�һ�����  
            m_pRoot = new BTreeNode(1, m_m, NULL);  //�µĸ�����1���ؼ���  
            m_pRoot->K[1] = k; //�¸��Ĺؼ���  
            m_pRoot->A[0] = p; //��ָ��  
            m_pRoot->A[1] = a; //��ָ��  
            p->parent = a->parent = m_pRoot; //��������ָ��ĸ����  
            return true;
        }
    }
}
//�������ܣ� ����ؼ���x��B���У�����ʵ�ʵĲ��뺯��  
//���������� pָ�����ؼ������ڽ�㣬kΪ����Ĺؼ��֣�aΪ�ؼ��ֵ����ڣ�iΪ����λ��  
//����ֵ��   ��  

void BTree::InsertKey(BTreeNode* p, int k, BTreeNode* a, int i)
{
    for (int j = p->num; j > i; j--) //��K[i],A[i]�Ժ��Ԫ�ض�������һ��λ��  
    {
        p->K[j + 1] = p->K[j];
        p->A[j + 1] = p->A[j];
    }
    p->num++;        //���Ĺؼ��ָ�����1  
    p->K[i + 1] = k; //������Ԫ����K[i],A[i]�Ժ�  
    p->A[i + 1] = a;
    if (a != NULL)    //��ΪΪ�գ�����¸����ָ��  
        a->parent = p;
}
//�������ܣ� ���ѽ��  
//���������� pָ��Ҫ���ѵĽ�㣬kָ�����Ĺؼ��֣�aָ��ؼ��ֵ����ڣ�iΪ����λ��  
//����ֵ��   ��  

void BTree::SpliteNode(BTreeNode* p, int* k, BTreeNode** a, int i)
{
    InsertKey(p, *k, *a, i); //�Ȳ�����˵  
    int mid = (m_m + 1) / 2;   //[ceil(m/2)]  
    int size = (m_m & 1) ? mid : mid + 1; //��ż�Ծ����˷���ʱ�����Ĺؼ��ָ���  

    BTreeNode* q = new BTreeNode(0, m_m, p->parent); //�½��  
    //��p��K[mid+1...m]��A[mid..m]�Ƶ�q��K[1...mid-1]��A[0...mid-1]  
    q->A[0] = p->A[mid];
    for (int j = 1; j < size; j++)
    {
        q->K[j] = p->K[mid + j];
        q->A[j] = p->A[mid + j];
    }
    //�޸�q�е���Ů�ĸ����Ϊq���������Ҫ����Ϊ��Щ��Ůԭ���ĸ����Ϊp  
    if (q->A[0] != NULL)
    {
        for (int j = 0; j < size; j++)
            q->A[j]->parent = q;
    }
    //���½��Ĺؼ��ָ���  
    q->num = m_m - mid;  //���q��m �C[ceil(m/2)], A[ceil(m/2)],(K [ceil(m/2)]+1, A [ceil(m/2)]+1), ��, (Km, Am)  
    p->num = mid - 1;    //���p��[ceil(m/2)]�C1, A0, (K1, A1), (K2,A2), ��, (K[ceil(m/2)]�C1, A[ceil(m/2)]�C1)   
    //�����µ���Ԫ��(k,a)  
    *k = p->K[mid];
    *a = q;
}

//�������ܣ� ɾ���ؼ���x  
//���������� xΪҪɾ���Ĺؼ���  
//����ֵ��   ɾ���Ƿ�ɹ�  

bool BTree::Delete(const int& x)
{
    Triple triple = Search(x); //����Ƿ��Ѵ���  
    if (triple.tag == false)       //x����B����  
        return false;
    BTreeNode* p = triple.node, * q; //Ҫɾ���Ĺؼ������ڽ��  
    int i = triple.i;

    if (p->A[i] != NULL) //��Ҷ���  
    {
        q = p->A[i];    //������������С�ؼ���  
        while (q->A[0] != NULL)
            q = q->A[0];
        p->K[i] = q->K[1];   //��Ҷ����滻  
        LeftCompress(q, 1);  //ɾ��K[1]����ʵֻ���ú���Ľ�㸲��һ�¼���  
        p = q;               //ת��ΪҶ����ɾ��  
    }
    else
        LeftCompress(p, i);  //Ҷ���ֱ��ɾ������ʵֻ���ú���Ľ�㸲��һ�¼���  

    int mid = (m_m + 1) / 2; //��[ceil(m/2)]  
    //���濪ʼ����  
    while (1)
    {
        if (p == m_pRoot || p->num >= mid - 1) //����1������2  
            break;
        else
        {
            q = p->parent; //���׽��  
            for (i = 0; i <= q->num && q->A[i] != p; i++) //�ҵ�p�ڸ�����е�λ��Ai  
                ;
            if (i == 0)     //pΪ����ָ��  
                RightAdjust(p, q, i);  //���p�������q��p�����ֵܽ�������ת����  
            else
                LeftAdjust(p, q, i);   //���p�������q��p�����ֵܽ�������ת����  
            p = q;         //���ϵ���  
        }
    }
    if (m_pRoot->num == 0) //һ�ſ���  
    {
        p = m_pRoot->A[0];
        delete m_pRoot;
        m_pRoot = p;
        if (m_pRoot != NULL)
            m_pRoot->parent = NULL;
    }
    return true;
}
//�������ܣ� ͨ������Ů�������������Ů�ж����㣬������Ůȡһ���ؼ���  
//���������� pָ��ɾ���Ĺؼ������ڽ�㣬qָ�򸸽�㣬iΪp��q�е�λ��  
//����ֵ��   ��  

void BTree::RightAdjust(BTreeNode* p, BTreeNode* q, int i)
{
    BTreeNode* pR = q->A[i + 1];  //p�����ֵ�  
    if (pR->num >= (m_m + 1) / 2)       //����3���ֵ����㹻��Ĺؼ��֣������ٻ���[ceil(m/2)]  
    {
        //����p  
        p->num++;                  //p�Ĺؼ��ָ�����1  
        p->K[p->num] = q->K[i + 1];  //�������Ӧ�ؼ�������  
        p->A[p->num] = pR->A[0];   //���ֵ�����ָ���Ƶ�p������  
        if (p->A[p->num] != NULL)
            p->A[p->num]->parent = p;  //�޸ĸ���㣬ԭ����pR  
        //���������  
        q->K[i + 1] = pR->K[1];      //���ֵܵ���С�ؼ������Ƶ������  
        //�������ֵ�  
        pR->A[0] = pR->A[1];       //���ֵ�ʣ��ؼ�����ָ��ǰ��  
        LeftCompress(pR, 1);       //����K[1],A[1]���ؼ��ָ�����1��LeftCompress���Զ����1    
    }
    else
        MergeNode(p, q, pR, i + 1);//����4 (...p Ki+1 pR...)  
}
//�������ܣ� ͨ������Ů�������������Ů�ж����㣬������Ůȡһ���ؼ���  
//���������� pָ��ɾ���Ĺؼ������ڽ�㣬qָ�򸸽�㣬iΪp��q�е�λ��  
//����ֵ��   ��  

void BTree::LeftAdjust(BTreeNode* p, BTreeNode* q, int i)
{
    BTreeNode* pL = q->A[i - 1]; //p�����ֵ�  
    if (pL->num >= (m_m + 1) / 2)      //����3  
    {
        //����p  
        RightCompress(p, 1);     //p�Ĺؼ��ֺ�ָ�������ƶ����ճ�λ�÷�����Ů�Ĺؼ��֣�RightCompress���Զ���1  
        p->A[1] = p->A[0];
        p->K[1] = q->K[i];        //�������Ӧ�ؼ�������  
        p->A[0] = pL->A[pL->num]; //���ֵ�����ָ���Ƶ�p������  
        if (p->A[0] != NULL)
            p->A[0]->parent = p;      //�޸ĸ���㣬ԭ����pL  
        //���������  
        q->K[i] = pL->K[pL->num]; //���ֵܵ����ؼ������Ƶ������  
        //�������ֵ�  
        pL->num--;   //���ֵܵĹؼ��ָ�����1  
    }
    else
    {
        //���һ���һ�£��Է��Ϻϲ������Ĳ���Ҫ��  
        BTreeNode* pR = p;
        p = pL;
        MergeNode(p, q, pR, i);   //����4��ע������i��������i+1 (...p Ki pR...)  
    }
}
//�������ܣ� �����p��i+1��ʼ�Ĺؼ��ֺ�ָ�������ƶ�1��ԭ����K[i],A[i]��ʵ�����ǵ���  
//���������� pָ���㣬iΪ�����ǵ�λ��  
//����ֵ��   ��  

void BTree::LeftCompress(BTreeNode* p, int i)
{
    int n = p->num;   //���ؼ��ָ���  
    for (int j = i; j < n; j++)
    {
        p->K[j] = p->K[j + 1];
        p->A[j] = p->A[j + 1];
    }
    p->num--; //�ؼ��ָ�����1  
}
//�������ܣ� �����p��i��ʼ�Ĺؼ��ֺ�ָ�������ƶ�1��ԭ����K[i],A[i]�ճ�����  
//���������� pָ���㣬iΪ�ճ�����λ�ã����ڷ��µĹؼ���  
//����ֵ��   ��  

void BTree::RightCompress(BTreeNode* p, int i)
{
    for (int j = p->num; j >= i; j--) //K[i],A[i]�ճ������ԷŲ���Ķ�Ԫ��  
    {
        p->K[j + 1] = p->K[j];
        p->A[j + 1] = p->A[j];
    }
    p->num++; //�ؼ��ָ�����1  
}
//�������ܣ� �ϲ��������  
//���������� pָ���㣬qָ���ף�pRָ��p�����ֵܣ�iΪ(...p,K,pR...)�е�Kλ��  
//����ֵ��   ��  

void BTree::MergeNode(BTreeNode* p, BTreeNode* q, BTreeNode* pR, int i)
{
    int n = p->num + 1;   //p�����һ���Źؼ��ֵ�λ��  
    p->K[n] = q->K[i];    //�½������Ĺؼ���  
    p->A[n] = pR->A[0];   //�����ֵ�����һ��ָ��  
    for (int j = 1; j <= pR->num; j++) //�����ֵ�ʣ��ؼ��ֺ�ָ���Ƶ�p��  
    {
        p->K[n + j] = pR->K[j];
        p->A[n + j] = pR->A[j];
    }
    if (p->A[0]) //�޸�p�е���Ů�ĸ����Ϊp���������Ҫ����Ϊ��Щ��Ůԭ���ĸ����ΪpR����������  
    {
        for (int j = 0; j <= pR->num; j++)
            p->A[n + j]->parent = p;
    }
    LeftCompress(q, i);            //�����Ĺؼ��ָ�����1  
    p->num = p->num + pR->num + 1; //�ϲ���ؼ��ֵĸ���  
    delete pR;
    pR = NULL;
}