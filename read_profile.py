import pstats

p = pstats.Stats('profile')
p.sort_stats('cumtime').print_stats(25)