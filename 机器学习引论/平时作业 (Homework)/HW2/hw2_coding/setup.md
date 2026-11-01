
## Setup

To simplify the management of various computing environments and avoid package version conflicts, we highly recommend using Anaconda, which allows for easy creation, management, and switching between Python environments. If you choose not to use a virtual environment, it is up to you to make sure that all dependencies for the code are installed globally on your machine.

### **Step 0: Make sure that you have installed Anaconda:**

If you decide to work locally, we recommend using the free [Anaconda Python distribution](https://www.anaconda.com/download/), which provides an easy way for you to handle package dependencies. Please be sure to download the Python 3 version. We are no longer supporting Python 2.

### **Step 1: Anaconda Virtual environment:**

Once you have Anaconda installed, it makes sense to create a virtual environment for the course.To set up a virtual environment, run (in a terminal)

`conda create -n CS182 python=3.10 anaconda`

to create an environment called `CS182`.

Then, to activate and enter the environment, run

`conda activate CS182`

To exit, you can simply close the window, or run

`conda deactivate CS182`

Note that every time you want to work on the assignment, you should run `source activate CS182` (change to the name of your virtual env).

You may refer to [this page](https://conda.io/docs/user-guide/tasks/manage-environments.html) for more detailed instructions on managing virtual environments with Anaconda.

### **Step 2: Install packages**

```bash
pip install requirements.txt
```
