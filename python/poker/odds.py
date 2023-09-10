import ctypes
import numpy as np

poker_lib=ctypes.cdll.LoadLibrary("/usr/local/lib/poker/libpoker.so")

def calculate_table_odds( hero_cards : list, villains, n_proc : int = 8):
    villains_str = ""
    for v in villains:
        villains_str += v[0]
        villains_str += v[1]
    
    func_wrapper = poker_lib.get_table_odds
    func_wrapper.restype = None
    func_wrapper.argtypes = (
        ctypes.POINTER(ctypes.c_char),
        ctypes.POINTER(ctypes.c_char),
        ctypes.POINTER(ctypes.c_char),
        ctypes.c_int,
        np.ctypeslib.ndpointer(dtype=np.float32, shape=(4,)),
        ctypes.c_int
    )
    results = np.zeros(4, dtype=np.float32)
    func_wrapper(
        hero_cards[0].encode(), 
        hero_cards[1].encode(),
        villains_str.encode(),
        len(villains),
        results,
        n_proc
    )

    return results