from encodings import utf_8
import _mapper
import cffi

class Mapper():
    def __init__(self):
        # self.p = _point.lib.get_point(x, y) if x else _point.lib.get_default_point()
        self.ffi = cffi.FFI()
        pass
        
    def test(self, t: int):
        _mapper.lib.test_bindings(t)

    def test_string(self, t: str):
        _t = self.ffi.new("char[]", str.encode(t))
        _mapper.lib.test_string(_t)

    def test_vector(self, t: list):
        vcount = len(t)
        _t = list(map(lambda x: self.ffi.new("char[]", str.encode(x)), t))
        _t.append(self.ffi.NULL)

        _vec = self.ffi.new("char*[]", _t)
        _mapper.lib.test_vector(_vec, vcount)

    def test_bool(self, t: bool):
        _mapper.lib.test_bool(t)

    def test_return_int(self):
        x = _mapper.lib.test_return_int()
        print(x)

    def test_return_string(self):
        x = _mapper.lib.test_return_string()
        print(x)
        # y = self.ffi.string(x)
        # print(self.ffi.string(x))
        # print(y)
        
    def test_return_vector(self):
        x = _mapper.lib.test_return_vector()
        print(x)
        i = 0
        arr = []
        while x[i] != self.ffi.NULL:
            arr.append(self.ffi.string(x[i])) 
            i = i + 1
        
        # print("{}: {}".format(i, arr))

    def test_return_bool(self):
        x = _mapper.lib.test_return_bool()
        print(x)
        



if __name__ == '__main__':
    mapper = Mapper()
    print("------ Call Tests ------")
    mapper.test(t=5)
    mapper.test_string("Hello World.")
    v = []
    v.append("List 1")
    v.append("List 2")
    mapper.test_vector(v)
    mapper.test_bool(False)
    print("------ Return Tests ------")
    mapper.test_return_int()
    mapper.test_return_string()
    mapper.test_return_vector()
    mapper.test_return_bool()
