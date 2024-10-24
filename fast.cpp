#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
clock_t start, end;
double cpu_time_used;
void set_clock(){
    start = clock();
}
double end_clock(){
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;

}
void bubbleSort(int nums[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (nums[j] > nums[j + 1]) {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}
void siftDown(int *arr, int start, int end) {   
    int root = start;        
    while (root * 2 + 1 <= end) {
        int child = root * 2 + 1;     
        int swap = root;      // 存储最大值的位置    
        if (arr[swap] < arr[child]) {     
            swap = child;   // 记录左孩子的位置
        }
        if (child + 1 <= end && arr[swap] < arr[child + 1]) {      // 如果右孩子存在且大于根节点
            swap = child + 1;     // 记录右孩子的位置
        }
        if (swap != root) {   // 如果最大值不是根节点
            int temp = arr[root];   // 交换根节点和最大值
            arr[root] = arr[swap];  
            arr[swap] = temp;        
            root = swap;        
        } else {
            return;  
        }
    }
}
void heapsort(int *arr, int len) {
    int start = (len - 2) / 2;    // 最后一个非叶子节点
    while (start >= 0) {
        siftDown(arr, start, len - 1);   // 建立最大堆
        start--;    // 向前继续建立最大堆
    }
    int end = len - 1;    // 数组最后一个元素
    while (end > 0) {
        int temp = arr[0];   // 交换根节点和最后一个元素
        arr[0] = arr[end];   
        arr[end] = temp;      
        siftDown(arr, 0, end - 1);   // 重新建立最大堆
        end--;    // 继续建立最大堆
    }
}

//从这开始，到后面指定位置是抄袭howthon的
#define MAX_NODES 33333333 // 假设最多100个节点

// 节点结构体
typedef struct FibonacciNode {
    int key;  // 节点的键值
    int degree;  // 节点的度数
    struct FibonacciNode *parent;  // 指向父节点
    struct FibonacciNode *child;   // 指向子节点
    struct FibonacciNode *left;    // 指向左兄弟
    struct FibonacciNode *right;   // 指向右兄弟
    int mark;  // 标记位
} FibonacciNode;

// 斐波那契堆
typedef struct FibonacciHeap {
    FibonacciNode *min;  // 指向最小的节点
    int n;  // 堆中节点的数量
} FibonacciHeap;

// 静态内存池
FibonacciNode node_pool[MAX_NODES];
int node_count = 0;

//debug
void printHeap(FibonacciHeap *heap)
{
    if (heap->min == NULL)
    {
        printf("Heap is empty\n");
        return;
    }
    printf("Heap nodes: ");
    FibonacciNode *x = heap->min;
    do
    {
        printf("%d ", x->key);
        x = x->right;
    } while (x != heap->min);
    printf("\n");
}

// 模拟手动内存分配：从池中获取新节点
FibonacciNode* createNode(int key) {
    if (node_count >= MAX_NODES) {
        printf("内存池已满，无法分配新节点\n");
        return NULL;
    }

    FibonacciNode *node = &node_pool[node_count++];
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;

    // 调试信息：验证节点创建
    printf("创建节点：%d，当前已使用节点数量：%d\n", key, node_count);

    return node;
}

// 创建斐波那契堆
FibonacciHeap* createHeap() {
    static FibonacciHeap heap;
    heap.min = NULL;
    heap.n = 0;
    return &heap;
}

// 将节点加入双向链表
void insertNode(FibonacciNode *node, FibonacciNode *minNode) {
    if (minNode == NULL) {
        node->left = node;
        node->right = node;
    } else {
        node->right = minNode;
        node->left = minNode->left;
        minNode->left->right = node;
        minNode->left = node;
    }
}

// 插入一个节点到斐波那契堆中
void insertHeap(FibonacciHeap *heap, int key) {
    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化，无法插入节点\n");
        return;
    }

    FibonacciNode *node = createNode(key);
    if (node == NULL) {
        printf("错误：无法创建新节点，内存分配失败\n");
        return;
    }

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        insertNode(node, heap->min);
        if (key < heap->min->key) {
            heap->min = node;
        }
    }
    heap->n++;

    // 调试信息：验证插入节点后堆的状态
    printf("插入节点：%d，当前最小节点：%d，堆中节点数量：%d\n", key, heap->min->key, heap->n);
}


// 将一个节点y连接到另一个节点x上
void link(FibonacciNode *y, FibonacciNode *x) {
    // 从根链表中移除y
    y->left->right = y->right;
    y->right->left = y->left;

    // 把y作为x的子节点
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
    }
    x->degree++;
    y->mark = 0;
}
//辅助函数
void fibHeapLink(FibonacciHeap *heap, FibonacciNode *y, FibonacciNode *x)
{
    // 将y链接为x的子节点
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL)
    {
        x->child = y;
        y->left = y->right = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
    y->mark = 0;
}

// v2.0
// 合并堆的根链表，确保每个度数唯一
void _consolidate(FibonacciHeap *heap)
{
    int D = (int)(log(heap->n) / log(2)) + 1;
    // FibHeapNode *A[D];
    //  动态分配 A 数组 Windows的Mingw不支持c99
    FibonacciNode **A = (FibonacciNode **)malloc(D * sizeof(FibonacciNode *));
    // FibHeapNode *A = (FibHeapNode *) malloc(sizeof(FibHeapNode *[D]));

    for (int i = 0; i < D; i++)
    {
        A[i] = NULL;
    }

    FibonacciNode *x = heap->min;
    int numRoots = 0;
    if (x != NULL)
    {
        numRoots++;
        x = x->right;
        while (x != heap->min)
        {
            numRoots++;
            x = x->right;
        }
    }
    while (numRoots > 0)
    {
        int d = x->degree;
        FibonacciNode *next = x->right;

        while (A[d] != NULL)
        {
            FibonacciNode *y = A[d]; // y与x度数相同
            if (x->key > y->key)
            { // 交换x与y
                FibonacciNode *temp = x;
                x = y;
                y = temp;
            }
            fibHeapLink(heap, y, x);
            A[d] = NULL;
            d++;
        }

        A[d] = x;
        x = next;
        numRoots--;
    }

    heap->min = NULL;

    for (int i = 0; i < D; i++)
    {
        if (A[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            }
            else
            {
                A[i]->left = heap->min;
                A[i]->right = heap->min->right;
                heap->min->right->left = A[i];
                heap->min->right = A[i];
                if (A[i]->key < heap->min->key)
                {
                    heap->min = A[i];
                }
            }
        }
    }
    // a small bug i forgot
    free(A);
}

// 度数合并操作
void consolidate(FibonacciHeap *heap) {
    int maxDegree = 20;  // 假设最大度数为20
    FibonacciNode *A[maxDegree];

    for (int i = 0; i < maxDegree; i++) {
        A[i] = NULL;
    }

    FibonacciNode *x = heap->min;
    FibonacciNode *start = heap->min;
    do {
        FibonacciNode *next = x->right;
        int d = x->degree;
        while (A[d] != NULL) {
            FibonacciNode *y = A[d];
            if (x->key > y->key) {
                FibonacciNode *temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        x = next;
    } while (x != start);

    heap->min = NULL;

    for (int i = 0; i < maxDegree; i++) {
        if (A[i] != NULL) {
            if (heap->min == NULL) {
                heap->min = A[i];
            } else {
                insertNode(A[i], heap->min);
                if (A[i]->key < heap->min->key) {
                    heap->min = A[i];
                }
            }
        }
    }

    // 调试信息：合并后的最小节点
    if (heap->min != NULL) {
        printf("合并后最小节点：%d\n", heap->min->key);
    } else {
        printf("合并后堆中没有最小节点\n");
    }
}

FibonacciNode *_extractMin(FibonacciHeap *heap)
{
    FibonacciNode *z = heap->min;

    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化\n");
        return NULL;
    }

    if (heap->min == NULL) {
        printf("错误：堆中没有节点可以提取\n");
        return NULL;
    }

    if (z != NULL)
    {
        // 将最小节点的每个子节点添加到根链表中
        if (z->child != NULL)
        {
            FibonacciNode *child = z->child;
            do
            {
                FibonacciNode *next = child->right;
                child->left = heap->min;
                child->right = heap->min->right;
                heap->min->right->left = child;
                heap->min->right = child;
                child = next;
            } while (child != z->child);
        }

        // 移除最小节点
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            heap->min = NULL;
        }
        else
        {
            heap->min = z->right;
            // 调整堆
            _consolidate(heap); // 需要实现合并堆
        }
        heap->n--;
        // 调试信息：验证提取最小节点后堆的状态
        printf("提取最小节点：%d，当前最小节点：%d（如果存在），堆中剩余节点数量：%d\n",z->key, heap->min != NULL ? heap->min->key : -1, heap->n);
    }
    return z;
}


// 提取堆中的最小节点
FibonacciNode* extractMin(FibonacciHeap *heap) {
    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化\n");
        return NULL;
    }

    if (heap->min == NULL) {
        printf("错误：堆中没有节点可以提取\n");
        return NULL;
    }

    FibonacciNode *z = heap->min;
    if (z != NULL) {
        // 把最小节点的孩子移到根链表
        if (z->child != NULL) {
            FibonacciNode *child = z->child;
            do {
                child->parent = NULL;
                child = child->right;
            } while (child != z->child);
        }

        // 从根链表中移除z
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            heap->min = NULL;
        } else {
            heap->min = z->right;
            //debug
            printHeap(heap);
            _consolidate(heap);  // 执行度数合并
        }

        heap->n--;
        //debug
        //printHeap(heap);
        // 调试信息：验证提取最小节点后堆的状态
        //printf("提取最小节点：%d，当前最小节点：%d（如果存在），堆中剩余节点数量：%d\n",z->key, heap->min != NULL ? heap->min->key : -1, heap->n);

        return z;
    }

    printf("错误：无法提取最小节点，未知原因\n");
    return NULL;
}


// 斐波那契堆排序算法
void fabsort(int arr[], int n) {
    FibonacciHeap *heap = createHeap();
    if (heap == NULL) {
        printf("错误：无法创建斐波那契堆\n");
        return;
    }

    // 插入所有元素到斐波那契堆
    for (int i = 0; i < n; i++) {
        insertHeap(heap, arr[i]);
    }

    //debug
    //printHeap(heap);
    //插入节点是正常的

    // 提取最小值进行排序
    for (int i = 0; i < n; i++) {
        FibonacciNode *minNode = _extractMin(heap);
        if (minNode != NULL) {
            arr[i] = minNode->key;
        } else {
            printf("错误：无法提取最小节点，排序失败\n");
            return;
        }
    }
}
//到这结束，都是抄袭的


int main() {
    FILE *file2=fopen("data.csv", "w"),*file; 
    if (file2 == NULL) {
        fprintf(stderr, "无法打开文件 %s\n", "data.csv");
        exit(1);
    }
    double arr[5];
    char new_data[30];
    fprintf(file2, "size,bubblesort,heapsort,fabsort\n");
    
    long num_elements = 131072; 
    char filename[30];
    int *data = (int*)malloc(num_elements * sizeof(int));
    
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    sprintf(filename,"./data/data_%ld",num_elements);
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file\n");
        free(data);
        return 1;
    }

    for (long i = 0; i < num_elements; i++) {
        if (fscanf(file, "%d", &data[i]) != 1) {
            fprintf(stderr, "Error reading data from file at index %ld\n", i);
            fclose(file);
            free(data);
            return 1;
        }
    }
    
    set_clock();bubbleSort(data,num_elements);arr[0]=end_clock();
    set_clock();heapsort(data,num_elements);arr[1]=end_clock();
    set_clock();fabsort(data,num_elements);arr[2]=end_clock();
    fprintf(file2, "%ld,%f,%f,%f\n", num_elements,arr[0], arr[1], arr[2]);
    fclose(file);
    free(data);
    
    fclose(file2);
    return 0;
}