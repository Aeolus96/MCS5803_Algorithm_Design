# // I have neither given nor received any unauthorizedaid in completing this work,
# // nor have I presented someone else's work as my own.
# // Your Name: Devson Butani
# // LTU ID: 000732711
# // Date: 10/16/2022

import math

neg_inf = -math.inf


# Find the sum of elements between left and right
def middleSum(array, low, middle, high):
    # Left side
    total = 0  # temp sum
    left_max = neg_inf
    # Sum backwards
    for i in range(middle, low - 1, -1):
        total += array[i]
        if total > left_max:
            left_max = total
            # print("Left Sum: ", left_max)

    # Right side
    total = 0  # reset temp sum
    right_max = neg_inf
    # Sum forwards including high
    for i in range(middle, high + 1):
        total += array[i]
        if total > right_max:
            right_max = total
            # print("Right Sum: ", right_max)

    maximum_midArray_sum = max(
        left_max + right_max - array[middle],
        left_max,
        right_max,
    )
    # Left(includes middle) + Right(includes middle) - middle(2mid - 1mid = 1overall)
    # print("Middle Sum: ", maximum_mid)
    return maximum_midArray_sum


# Find the max sub array recursively using divide and conquer
def maxSum(array, low, high):
    # Bad array index input at the begining
    if low > high:
        return 0
    # If only one element in array or last element in recursion
    if low == high:
        return array[low]

    # Find middle index. Floor division
    middle = (low + high) // 2
    # print("Middle index: ", middle)
    # Return recursively the highest of either maxSum(left or right)
    # or middleSum for every divide and pass it on to the top
    maximum_subArray_sum = max(
        maxSum(array, low, middle - 1),
        maxSum(array, middle + 1, high),
        middleSum(array, low, middle, high),
    )
    # print("Max Sum: ", max_s)
    return maximum_subArray_sum


# INPUT:
a = [-1, -2, -3, 4, 5, -7, 10, -15]
# [2, -4, 1, 9, -6, 7, -3] = 11
# [0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1] = 6
# [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] = 55
# [-1, -2, -3, 4, 5, -7, 10, -15] = 12
n = len(a)

# execute
if __name__ == "__main__":
    max_sum = maxSum(a, 0, n - 1)
    print("Maximum Sub Array: ", max_sum)
