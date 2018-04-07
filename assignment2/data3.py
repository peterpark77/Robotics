import numpy as np
import matplotlib.pyplot as plt

# example data
x = np.arange(30,130,10)
y_raw_iter = [[15,17,13,12,18,11,16,13,12,15],[11,14,13,11,12,13,21,12],[13,19,16,13,14,15,12,14,13,15],[19,16,15,20,13,14,13,14,23,20],[23,17,21,19,18,14,15,17,16,16],[17,23,18,18,20,18,20,17,21,33],[17,19,28,38,21,35,21,30,24,20],[23,19,19,20,26,25,32,26,19,28],[28,22,28,21,30,21,40,23,25,21],[26,21,28,20,24,31,20,27,28,26]]

y_length = [np.mean(y) for y in y_raw_iter]
yerr = [np.std(y) for y in y_raw_iter]


plt.figure()
plt.errorbar(x, y_length,  yerr, ecolor='b', capthick= 4 , fmt='--o')
plt.title("rrt_iterations vs length of robot")
plt.ylabel("Num_rrt_iterations")
plt.xlabel("length of robot")

plt.show()
