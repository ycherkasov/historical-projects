import os
import stat
import glob


def show_filesystem_func():
    print(os.getcwd())


def show_list_comprehensions():
    # A list comprehension provides a compact way of mapping a list into another list by applying a function
    a_list = [1, 2, 3, 4]
    print([elem*2 for elem in a_list])

    # it does not change the original list

    files_in_dir = [os.path.realpath(f) for f in glob.glob('*.py')]
    print(files_in_dir)

    # List comprehensions can also filter items
    big_in_dir = [os.path.realpath(f) for f in glob.glob('*.py') if os.stat(f).st_size > 600]
    print(big_in_dir)

    # there’s no limit to how complex a list comprehension can be
    sizes = [(os.stat(f).st_size, os.path.realpath(f)) for f in glob.glob('*.py')]
    print(sizes)



def show_dict_comprehensions():
    # A dictionary comprehension is like a list comprehension, but it constructs a dictionary instead of a list
    metadata_dict = {f: os.stat(f) for f in glob.glob('*.py')}
    print(metadata_dict)

    # Here’s a trick with dictionary comprehensions that might be useful someday:
    # swapping the keys and values of a dictionary (value also must be hashable)
    a_dict = {'a': 1, 'b': 2, 'c': 3}
    rev_dict = {value: key for key, value in a_dict.items()}
    print(a_dict)
    print(rev_dict)


if __name__ == '__main__':
    show_filesystem_func()
    show_list_comprehensions()
    show_dict_comprehensions()
