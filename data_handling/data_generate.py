import random
def data(length,type='int'):
    ''''
    生成指定数量和类型的随机测试数据。
    :param length:指定生成数量
    :param type:指定生成类型
    '''
    if type=='int':
        return [random.randint(0,100000) for _ in range(length)]
    elif type=='float':
        return [random.uniform(0,100) for _ in range(length)]
    else:
        raise ValueError("Unsupported data type. Use 'int' or 'float'.")  # 如果传入的数据类型不是int或float，抛出异常

for i in range(1,18):
    length=2**i
    print(f"length={length}")
    datas=data(length)
    with open(f'./data/data_{length}','w') as f:
        for item in datas:
            f.write("%s\n" % item)