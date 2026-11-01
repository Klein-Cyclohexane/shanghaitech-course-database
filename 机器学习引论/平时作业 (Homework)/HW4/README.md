# CS182 HW4

## Introduction

HW4 consists of two parts: writing exercises and coding exercises.

### Part One: Writing (85 PTS)

1. Finish the problems in `writing/hw4.pdf`.
2. Upload your answers as "yourname_hw4_writing.pdf" to Gradescope.

### Part Two: Coding (30 PTS)

1. Finish `coding/LinearRegression.ipynb`.
2. Please export the jupyter notebook as a PDF named  "yourname_hw4_coding.pdf" and upload it to Gradescope.



# This is the coding part of HW4

### Step 0: Setup Environment

To simplify the management of various computing environments and avoid package version conflicts, we highly recommend using Anaconda, which allows for easy creation, management, and switching between Python environments. If you choose not to use a virtual environment, it is up to you to make sure that all dependencies for the code are installed globally on your machine.

#### **Make sure that you have installed Anaconda:**

If you decide to work locally, we recommend using the free [Anaconda Python distribution](https://www.anaconda.com/download/), which provides an easy way for you to handle package dependencies. Please be sure to download the Python 3 version. We are no longer supporting Python 2.

#### **Anaconda Virtual environment:**

Once you have Anaconda installed, it makes sense to create a virtual environment for the course.To set up a virtual environment, run (in a terminal)

`conda create -n CS182 python=3.10 anaconda`

to create an environment called `CS182`.

Then, to activate and enter the environment, run

`conda activate CS182`

To exit, you can simply close the window, or run

`conda deactivate CS182`

Note that every time you want to work on the assignment, you should run `source activate CS182` (change to the name of your virtual env).

You may refer to [this page](https://conda.io/docs/user-guide/tasks/manage-environments.html) for more detailed instructions on managing virtual environments with Anaconda.

#### Install packages

```bash
pip install ./coding/requirements.txt
```



### Step 1: Following the instruction in `coding/LinearRegression.ipynb` to complete this assignment.

### Step 2: Export the jupyter notebook as PDF and select all pages to the correspondence problem on gradescope `hw4_coding` part. Note that the `coding` part and the `writing` part share the same deadline and grace days.

## **Note:**

If you have any trouble doing your homework, feel free to contact TAs on Piazza.