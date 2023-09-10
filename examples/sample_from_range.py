from poker import Range

r = Range(b"AKs,AQs")

for i in range(10):
    r.random_hand().print()