with open("./data/data_500",'r') as f:
    test_data = [int(line.strip()) for line in f]
    for i in range(1, len(test_data)):
        if test_data[i-1] <= test_data[i]:
            raise ValueError(f"List is not strictly decreasing at index {i-1} and {i}")
#本代码本来使用类似冒泡排序，但是冒泡排序的时间复杂度为O(n^2)，
# 但在github copilot的建议下，使用了更快的方法
        

