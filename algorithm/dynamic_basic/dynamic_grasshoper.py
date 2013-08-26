__author__ = 'straight'


def grasshopper1():
    """ 1 задача о кузнечике
    Есть прямая, вдоль которой стоят столбики 0, 1, 2,...n
    Со столбика 0 до n прыгает кузнечик
    он может прыгать на 1 столбик вперед, на 2, ... на k столбиков
    Сколько всего вариантов добраться до столбика n
    """
    # number of points
    n = 10
    k = 5

    # a[i] - array to save number of ways to reach point 'i'
    a = [None] * n
    a[0] = 1
    a[1] = 1
    for i in range(2, n):
        sum = 0
        for j in range(max(i - k, 0), i):
            sum += a[j]
        a[i] = sum

    # result
    print(a)


def grasshopper2():
    """ 2 задача о кузнечике
    Есть прямая, вдоль которой стоят столбики 0, 1, 2,...n
    Со столбика 0 до n прыгает кузнечик
    он может прыгать на 1 столбик вперед, на 2, ... на k столбиков
    На некоторых столбмках сидят лягушки, соторые его съедят
    Сколько всего вариантов добраться до столбика n с учетом столбиков, на которые нельзя
    """

    # number of points
    n = 10
    k = 5

    # a[i] - array to save number of ways to reach point 'i'
    a = [None] * n
    a[0] = 1
    a[1] = 1

    # init frog columns (let it be 3, 7)
    a[3] = 0
    a[7] = 0

    for i in range(2, n):

        # column with frog, skip this
        if a[i] is 0: continue

        sum = 0
        for j in range(max(i - k, 0), i):
            sum += a[j]
        a[i] = sum

    # result
    print(a)


def grasshopper3():
    """ 3 задача о кузнечике
    Есть прямая, вдоль которой стоят столбики 0, 1, 2,...n
    Со столбика 0 до n прыгает кузнечик
    он может прыгать на 1 столбик вперед, на 2, ... на k столбиков
    На каждом столбике кузнечик плучает или отдает какое-то количество монеток
    Задача - найти путь, на котором прибыть будет максимальна
    """
    # number of points
    n = 10
    k = 5

    d = [0, 2, -5, -1, 3, 2, -7, 8, -5, 0]
    frm = [0] * n

    # a[i] - maximum number of coins we can collect reaching column 'i'
    a = [0] * n
    a[0] = 0

    for i in range(0, n):
        max_amount = 0
        result_dict = {}
        for j in range(max(i - k, 0), i):
            if a[i] <= (a[i - j] + d[i]):
                a[i] = a[i - j] + d[i]
                if a[i] > max_amount:
                    max_amount = a[i]
                    frm[i] = i-j
                    result_dict[i-j] = max_amount


    # result
    print(a)
    print(frm)
    print(result_dict)


def main():
    grasshopper3()


if __name__ == "__main__":
    main()