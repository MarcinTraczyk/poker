import ctypes

poker_lib=ctypes.cdll.LoadLibrary("/usr/local/lib/poker/libpoker.so")

class Hand:
    cpp_pointer = None
    def __init__(self, c1 : int = None, c2 : int = None, cptr = None):
        if cptr is not None:
            self.cpp_pointer = cptr
        else:
            Class_ctor_wrapper = poker_lib.hand_init
            Class_ctor_wrapper.restype = ctypes.c_void_p
            self.cpp_pointer = ctypes.c_void_p(Class_ctor_wrapper(c1, c2))

    def print(self):
        poker_lib.hand_print(self.cpp_pointer)