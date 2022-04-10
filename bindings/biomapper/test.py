import _mapper

class Mapper():
    def __init__(self):
        # self.p = _point.lib.get_point(x, y) if x else _point.lib.get_default_point()
        pass
        
    def test(t: int):
        _mapper.lib.cffi_test_bindings(t)



if __name__ == '__main__':
    mapper = Mapper
    mapper.test(t=5)