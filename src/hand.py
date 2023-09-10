import ctypes

poker_hand_lib=ctypes.cdll.LoadLibrary("build/libhand.so")

class Hand:
    cpp_pointer = None
    def __init__(self, c1 : int, c2 : int):
        Class_ctor_wrapper = poker_hand_lib.hand_init
        Class_ctor_wrapper.restype = ctypes.c_void_p
        self.cpp_pointer = ctypes.c_void_p(Class_ctor_wrapper(c1, c2))

    def __del__(self):
        print('destructor called')
        poker_hand_lib.hand_delete(self.cpp_pointer)

    def print(self):
        poker_hand_lib.hand_print(self.cpp_pointer)

if __name__ == '__main__':
    h = Hand(7, 19)
    h.print()


