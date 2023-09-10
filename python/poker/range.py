import ctypes
from poker import Hand

poker_lib=ctypes.cdll.LoadLibrary("/usr/local/lib/poker/libpoker.so")

class Range:
    cpp_pointer = None
    def __init__(self, c1):
        Class_ctor_wrapper = poker_lib.range_init
        Class_ctor_wrapper.restype = ctypes.c_void_p
        self.cpp_pointer = ctypes.c_void_p(Class_ctor_wrapper(c1))

    def random_hand(self):
        Class_ctor_wrapper = poker_lib.hand_init
        Class_ctor_wrapper.restype = ctypes.c_void_p
        cpp_pointer = ctypes.c_void_p(poker_lib.random_hand_ptr(self.cpp_pointer))
        return Hand(cptr=cpp_pointer)