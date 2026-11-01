# HW6 Guidance

- Create conda environment:

    ```shell
    conda create -n CS182 python=3.10 -y
    conda activate CS182
    pip install numpy pandas matplotlib -i https://pypi.tuna.tsinghua.edu.cn/simple
    pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
    ```

    Note: You may need to change the fourth line of the commands to ensure downloading the suitable PyTorch version. Please check [PyTorch official website](https://pytorch.org/) for details. The above provided command requires you have an NVIDIA GPU with CUDA>=11.8. If the official website does not provide the suitable PyTorch version, you can try the following command:
    ```shell
    pip3 install torch torchvision torchaudio  -i https://pypi.tuna.tsinghua.edu.cn/simple
    ```

- Run `HW6_coding.ipynb` and finish `TODO` in the code base.

