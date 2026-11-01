import numpy as np
import torch.nn as nn
import matplotlib.pyplot as plt

def generate_data():
    mu      = np.array([1.0, 3.0])                    # mean: (1, 3)
    v1      = np.array([0.866, 0.5])
    v2      = np.array([-0.5, 0.866])
    V       = np.column_stack((v1, v2))
    stds    = np.array([3.0, 1.0])                    # sigma_1 = 3, sigma_2 = 1
    D       = np.diag(stds ** 2)
    cov     = V @ D @ V.T
    rng     = np.random.default_rng(0)
    N       = 2000
    L       = V * stds                                #  V @ diag(σ)
    samples = mu + rng.standard_normal((N, 2)) @ L.T

    # show data
    plt.figure(figsize=(6, 6))
    plt.scatter(samples[:, 0], samples[:, 1], s=8, color="0.55")
    plt.gca().set_aspect("equal")

    # show principal components
    colors = ['red', 'blue']
    labels = ['PC1', 'PC2']
    for i in range(2):
        direction = V[:, i] * stds[i]
        plt.arrow(mu[0], mu[1], direction[0], direction[1], width=0.06, head_width=0.4, color=colors[i], length_includes_head=True, label=labels[i])
    plt.legend()
    plt.show()

    return samples


class CNN(nn.Module):
    def __init__(self, classes=10, n1=16, n2=32, n3=64):
        super(CNN, self).__init__()
        self.classes = classes
        self.n1 = n1
        self.n2 = n2
        self.n3 = n3

        self.conv1 = nn.Conv2d(in_channels=1, out_channels=self.n1, kernel_size=5, stride=1, padding=0)
        self.conv2 = nn.Conv2d(in_channels=self.n1, out_channels=self.n2, kernel_size=5, stride=1, padding=0)

        self.maxpool = nn.MaxPool2d(2)
        self.fc1 = nn.Linear(self.n2 * 4 * 4, self.n3)
        self.fc2 = nn.Linear(self.n3, self.classes)

        self.net = nn.Sequential(
            self.conv1,
            nn.ReLU(),
            self.maxpool,
            self.conv2,
            nn.ReLU(),
            self.maxpool,
            nn.Flatten(),
            self.fc1,
            nn.ReLU(),
            self.fc2,
            nn.Softmax(dim=1)
        )

    def forward(self, x):
        x = self.net(x)
        return x


def show_cluster(X, centers, labels, iter_num):
    plt.figure(figsize=(6,6))
    plt.scatter(X[:, 0], X[:, 1], c=labels, s=3, cmap='tab10', alpha=0.6)
    plt.scatter(centers[:, 0], centers[:, 1], c='black', marker='x', s=100, linewidths=2)
    plt.title(f"K-Means - Iteration {iter_num}")
    plt.grid(True, linestyle=':')
    plt.axis('equal')
    plt.show()