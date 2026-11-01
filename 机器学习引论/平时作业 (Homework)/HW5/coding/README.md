# HW5 Guidance 👣

- Create conda environment on `login node`:
    ```
    conda create -n cs182_env python=3.9 -y
    conda activate cs182_env
    pip install torch torchvision torchaudio --i https://download.pytorch.org/whl/cu118
    pip install numpy pandas matplotlib scikit-learn tqdm -i https://pypi.tuna.tsinghua.edu.cn/simple
    ```


- Run `download_dataset.ipynb` on login node to download the dataset used in `HW5_coding.ipynb`.

- Run `HW5_coding.ipynb` and finish `TODO` in the code base.