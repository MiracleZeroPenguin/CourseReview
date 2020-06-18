void PreOrderTraversal(BinTree BT)
{
    if( BT )
    {
        printf(“%d\n”, BT->Data);        //对节点做些访问比如打印
        PreOrderTraversal(BT->Left);     //访问左儿子
        PreOrderTraversal(BT->Right);    //访问右儿子
    }
}
void InOrderTraversal(BinTree BT)
{
    if(BT)
    {
        InOrderTraversal(BT->Left);
        printf("%d\n", BT->Data);
        InOrderTraversal(BT->Right);
    }
}
void PostOrderTraversal(BinTree BT)
{
    if (BT)
    {
        PostOrderTraversal(BT->Left);
        PostOrderTraversal(BT->Right);
        printf("%d\n", BT->Data);
    }
}
void LevelOrderTraversal(BinTree BT)
{
    BinTree T;
    Queue Q;    //声明一个队列
    if (BT == NULL)
        return;                 //如果树为空，直接返回
    Q = CreatQueue(MAX_SIZE);   //创建并初始化队列
    AddQ(Q, BT);                //将根节点入队
    while (!IsEmpty(Q))
    {
        T = DeleteQ(Q);    　　　　　　　　　　//节点出队
        printf("%d\n", T->Data);    　　　　 //访问出队的节点
        if (T->Left)    AddQ(Q, T->Left);   //若左儿子不为空，将其入队 
        if (T->Right)    AddQ(Q, T->Right)  //若右儿子不为空，将其入队
    }
}