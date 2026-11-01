from builtins import range
import numpy as np
from random import shuffle
from past.builtins import xrange
# we should download the 'past' module by 'pip install future' ????!!!!

def svm_loss_naive(W, X, y, reg):
    """
    Structured SVM loss function, naive implementation (with loops).

    Inputs have dimension D, there are C classes, and we operate on minibatches
    of N examples.

    Inputs:
    - W: A numpy array of shape (D, C) containing weights.
    - X: A numpy array of shape (N, D) containing a minibatch of data.
    - y: A numpy array of shape (N,) containing training labels; y[i] = c means
        that X[i] has label c, where 0 <= c < C.
    - reg: (float) regularization strength

    Returns a tuple of:
    - loss as single float
    - gradient with respect to weights W; an array of same shape as W
    """

    loss = 0.0
    dW = np.zeros(W.shape) # initialize the gradient as zero

    #############################################################################
    # TODO:                                                                     #
    # Compute the gradient of the loss function and store it dW.                #
    # Rather than first computing the loss and then computing the derivative,   #
    # it may be simpler to compute the derivative at the same time that the     #
    # loss is being computed. As a result you may need to modify some of the    #
    # code above to compute the gradient.                                       #
    #############################################################################
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    
    num_classes = W.shape[1]
    num_train = X.shape[0]
    
    for i in range(num_train):
        scores = X[i].dot(W)  # (D) dot (D,C) -> (C)
        correct_class_score = scores[y[i]]
        
        for j in range(num_classes):
            if j == y[i]:
                continue
            
            margin = scores[j] - correct_class_score + 1  # Delta = 1
            
            if margin > 0:
                # Add to the loss when margin > 0
                loss += margin
                
                # Gradient computation:
                # For the correct class
                dW[:, y[i]] -= X[i]
                
                # For the incorrect class with margin > 0
                dW[:, j] += X[i]
    
    # Average the loss and gradient
    loss /= num_train
    dW /= num_train
    
    # Add regularization to the loss
    loss += reg * np.sum(W * W)
    
    # Add regularization gradient
    dW += 2 * reg * W
    
    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    return loss, dW

def svm_loss_vectorized(W, X, y, reg):
    """
    Structured SVM loss function, vectorized implementation.

    Inputs and outputs are the same as svm_loss_naive.
    """
    loss = 0.0
    dW = np.zeros(W.shape) # initialize the gradient as zero

    #############################################################################
    # TODO:                                                                     #
    # Implement a vectorized version of the structured SVM loss, storing the    #
    # result in loss.                                                           #
    #############################################################################
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    
    num_train = X.shape[0]
    
    # Compute scores for all examples
    scores = X.dot(W)  # (N,D) dot (D,C) -> (N,C)
    
    # Get scores of correct classes
    correct_class_scores = scores[np.arange(num_train), y].reshape(-1, 1)
    
    # Compute margins for all classes
    margins = np.maximum(0, scores - correct_class_scores + 1)
    
    # Set margins for correct classes to 0
    margins[np.arange(num_train), y] = 0
    
    # Compute loss
    loss = np.sum(margins) / num_train
    
    # Add regularization
    loss += reg * np.sum(W * W)
    
    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    #############################################################################
    # TODO:                                                                     #
    # Implement a vectorized version of the gradient for the structured SVM     #
    # loss, storing the result in dW.                                           #
    #                                                                           #
    # Hint: Instead of computing the gradient from scratch, it may be easier    #
    # to reuse some of the intermediate values that you used to compute the     #
    # loss.                                                                     #
    #############################################################################
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    binary = margins.copy()
    binary[binary > 0] = 1

    # Count how many margins > 0 for each example
    row_counts = np.sum(binary, axis=1)

    # For each instance, the correct class gets a negative gradient contribution
    # equal to the sum of all positive margin counts
    binary[np.arange(num_train), y] = -row_counts

    # Compute gradient using matrix operations
    # X.T has shape (D,N), binary has shape (N,C), so dW will be (D,C)
    dW = X.T.dot(binary)

    # Average over training examples
    dW /= num_train

    # Add regularization gradient
    dW += 2 * reg * W
    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    return loss, dW