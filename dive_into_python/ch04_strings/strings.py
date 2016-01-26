def show_strings():

    # compound replacement fields
    indexes = [1,2,3]
    s = '{0[0]}'.format(indexes)
    print(s)

    # example:
    # '1MB = 1000{0.modules[humansize].SUFFIXES[1000][0]}'.format(sys)

    # complicated format
    s = '{0:.1f}'.format(indexes[1])
    print(s)

    # some other methods
    multiline = '''Line 1
    Line 2
    Line 3
    '''

    print(multiline.splitlines())
    print(multiline.lower())
    print(multiline.count('L'))

    # turn HTTP request to dict
    query = 'user=pilgrim&database=master&password=PapayaWhip'
    a_list = query.split('&')
    print(a_list)

    a_list_of_lists = [v.split('=', 1) for v in a_list if '=' in v]
    print(a_list_of_lists)

    # dict() applied to list of lists works fine
    # noinspection PyTypeChecker
    a_dict = dict(a_list_of_lists)
    print(a_dict)


def show_bytes():
    # immutable bytes sequence
    b = b'abcd\x65'
    print(type(b))
    print(b)

    # you can not assign individual bytes. If you need to change individual bytes,
    # you can convert the bytes object into a bytearray object
    ba = bytearray(b)
    ba[0] = 65
    print(type(ba))
    print(ba)

    # You can’t concatenate bytes and strings. They are two different data types


if __name__ == '__main__':
    show_strings()
    show_bytes()
