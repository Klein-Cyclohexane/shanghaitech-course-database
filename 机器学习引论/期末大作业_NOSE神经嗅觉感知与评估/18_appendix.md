### A: Environment and Pretrained Models

The environment for this project can be quite tricky. We encourage you follow the exact steps of IBM's MoLFormer repository to set up the environment. You can find the instructions [here](https://github.com/IBM/molformer)

Warning: The environment includes [apex](https://github.com/NVIDIA/apex), which may fail in certain CUDA versions. If you encounter issues, try using a different CUDA version or change the optimization method to Adam in the training scripts.

Note: A group of people said that switching the apex to 22.04-dev branch can solve the problem. But that didn't work for me. You can try it if you encounter issues.

Before running the training scripts, ensure you have the datasets prepared and placed in the correct directories. You will also need to download the MoLFormer_Pretrained model from [here](https://github.com/Farzaneh-Taleb/MoLFormer_N2024) Notice that the checkpoint files are vital for your fine-tuning process. Make sure to have them before you run the training scripts.

### B: Contribution

Wenye Xiong (XXXXXXXX): He contributed to 100% of the code in this project and 50% of the report. He also wrote all the documentation and README files.

Jun Wang (XXXXXXXX): He contributed to 25% of the report. He also helped with processing the datasets and preparing the presentation slides.

Ge Gao (XXXXXXXX): She contributed to 25% of the report. She also helped with processing the datasets and preparing the presentation slides.