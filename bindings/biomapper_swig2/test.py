# import the module
import ctypes
import mapper
# import cffi

# load the library


# # create a Geek class
# class Geek(object):
#
#     # constructor
#     def __init__(self):
#
#         # attribute
#         self.obj = lib.Geek_new()
#
#
#     # define method
#     def myFunction(self):
#         lib.Geek_myFunction(self.obj)
#
#
#
# # create a Geek class object
# f = Geek()
#
# # object method calling
# f.myFunction()


class Mapper:
    def __init__(self):
        # self.p = _point.lib.get_point(x, y) if x else _point.lib.get_default_point()
        # self.ffi = cffi.FFI()
        # self.lib = ctypes.cdll.LoadLibrary('./libmapper.so')
        # self.lib.test_bindings.argtypes = [ctypes.c_int]
        # self.lib.test_string.argtypes = [ctypes.c_char_p]
        # self.lib.test_bool.argtypes = [ctypes.c_bool]
        # self.lib.test_vector = [ctypes.c_char_p, ctypes.c_int]
        #
        # self.mapper = lib.

        pass

    def test(self, t: int):
        print("==== Test Int ====")
        print(mapper.__file__)
        mapper.test_bindings(t)

    def test_string(self, t: str):
        print("==== Test Str ====")
        # Need to encode the string
        # _t = t.encode()
        # self.lib.test_string(_t)
        mper.test_string(_t)

    def test_vector(self, t: list):
        print("==== Test List ====")
        # vcount = len(t)
        # _t = list(map(lambda x: x.encode(), t))
        # # _t.append(self.ffi.NULL)
        # # self.lib.test_vector(t)
        #
        # # _vec = self.ffi.new("char*[]", _t)
        # self.lib.test_vector(_t, vcount)

    def test_bool(self, t: bool):
        print("==== Test Bool ====")
        # _mapper.lib.test_bool(t)
        mapper.test_bool(t)

    def test_return_int(self):
        print("==== Test Int Return ====")
        x = mapper.test_return_int()
        print(x)

    def test_return_string(self):
        print("==== Test String Return ====")
        x = mapper.test_return_string()
        print(x)
        # y = self.ffi.string(x)
        # print(self.ffi.string(x))
        # print(y)

    def test_return_vector(self):
        print("==== Test Vector Return ====")
        # x = self.lib.test_return_vector()
        # print(x)
        # i = 0
        # arr = []
        # while x[i] != self.ffi.NULL:
        #     arr.append(self.ffi.string(x[i]))
        #     i = i + 1

        # print("{}: {}".format(i, arr))

    def test_return_bool(self):
        print("==== Test Bool Return ====")
        x = mapper.test_return_bool()
        print(x)




if __name__ == '__main__':
    # mapper = Mapper()
    print("------ Call Tests ------")
    mapper.test_bindings(5)
    mapper.test_string("Hello World.")

    # v = []
    # v.append("List 1")
    # v.append("List 2")
    # mapper.test_vector(v)
    mapper.test_bool(True)
    print("------ Return Tests ------")
    print(mapper.test_return_int())
    print(mapper.test_return_string())
    # mapper.test_return_vector()
    print(mapper.test_return_bool())

    join_index = 1
    start_range = 300000
    end_range = 500000
    zero_based = False
    has_header = True
    delimiter = ','
    file_path = "/my/file.txt"
    mf = mapper.MapperFile(join_index=join_index, start_range=start_range, end_range=end_range, zero_based=zero_based,
                           has_header=has_header, delimiter=delimiter, file_path=file_path)
    mf.print_info()
    
