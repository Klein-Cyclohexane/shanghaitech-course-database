#(c) Is the given data set linear separable? If yes, construct a linear hypothesis function to separate the given data set. If no, explain the reason. \defpoints{3}
'''\begin{table}[h]
    \centering
    \begin{tabular}{c|cccccc}
        Data & (1,3) & (4,4) & (3,-6) & (-2,1) & (-3,5) & (-6,-4) \\ \hline
        Label & +1 & -1 & -1 & +1 & -1 & -1
    \end{tabular}
    \label{tab:my_label}
\end{table}'''
import matplotlib.pyplot as plt
import numpy as np

# Data points
X = np.array([[1, 3], [4, 4], [3, -6], [-2, 1], [-3, 5], [-6, -4]])
# Labels
y = np.array([1, -1, -1, 1, -1, -1])
# Plotting the data points
plt.scatter(X[:, 0], X[:, 1], c=y, cmap='bwr', edgecolor='k')
plt.title('Data Points')
plt.xlabel('X1')
plt.ylabel('X2')
plt.xlim(-7, 5)
plt.ylim(-7, 7)
plt.axhline(0, color='black', lw=0.5)
plt.axvline(0, color='black', lw=0.5)
plt.grid()

plt.show()