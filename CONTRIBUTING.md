# 🤝 上海科技大学课程资料库贡献指南 (Contributing Guide)

感谢你愿意为上海科技大学开源学业互助生态添砖加瓦！正是每一位同学的无私分享，才让历届学弟学妹们少踩坑、高效备考。

为了让资料更容易被检索，并能被**「上科大本科老学长智能助手」**等开源工具全自动识别，请参考以下指引：

---

## 🚀 两种提交方式

### 方式一：GitHub 网页端直接上传（零门槛，推荐小白使用）
1. 打开本仓库 [XiongWenye/shanghaitech-course-database](https://github.com/XiongWenye/shanghaitech-course-database)。
2. 点击进入对应课程的文件夹（如 `普通物理I`）。
3. 点击页面右上角的 **`Add file` -> `Upload files`**。
4. 将文件直接拖拽入框中。
5. 在页面底部写一句简短说明（如 `补充 2024 秋普通物理I期中试卷回忆版`），选择 **`Create a new branch for this commit and start a pull request`**。
6. 点击 **`Propose changes`**，发起 Pull Request 即可！

### 方式二：Git 命令行提交（适合熟悉 Git 的同学）
```bash
# 1. Fork 本仓库至你的 GitHub 账号
# 2. Clone 你的 Fork 仓库
git clone https://github.com/<你的用户名>/shanghaitech-course-database.git
cd shanghaitech-course-database

# 3. 创建新分支
git checkout -b add-course-materials

# 4. 拷贝你的资料到对应课程目录，并提交
git add .
git commit -m "feat(普物I): 添加 2024 秋期末试卷与答案"
git push origin add-course-materials

# 5. 在 GitHub 页面点击 'Compare & pull request' 提交 PR
```

---

## 📁 目录与文件命名规范（智能分类关键）

本项目后端集成了 **自动分类索引引擎**，只要文件名或子目录中包含以下关键词，系统就会自动将资料分发至对应板块：

| 资料类型 | 建议文件名包含关键词 | 示例 |
| :--- | :--- | :--- |
| **历年期中期末考卷** | `期末` / `期中` / `final` / `midterm` / `exam` / `试卷` / `真题` / `quiz` / `测验` | `2024春_期末试卷_原卷.pdf` |
| **试卷答案与解析** | `答案` / `解析` / `solution` / `sol` / `ans` / `key` / `评分标准` | `2024春_期末试卷_官方解答.pdf` |
| **考前速查 / 提纲** | `cheatsheet` / `cheat sheet` / `速查` / `提纲` / `复习思维导图` | `CS100_Final_Cheatsheet.pdf` |
| **平时作业与解答** | `作业` / `hw` / `homework` | `HW3_标准解答.pdf` |
| **课件讲义** | `讲义` / `课件` / `ppt` / `slide` / `第X章` / `笔记` | `Lecture_08_动态规划.pdf` |
| **实验与大作业** | `实验` / `lab` / `大作业` / `project` | `实验三_示波器的使用报告.pdf` |

> 📌 **新增一门全新课程**：
> 若仓库中尚未收录该门课程，请直接在仓库根目录以**官方标准中文全称**新建文件夹（例如 `操作系统/`、`编译原理/`），无需修改任何配置脚本。

---

## 🔒 隐私与学术安全防线 (必读)

1. **个人信息脱敏**：在上传实验报告、作业或试卷扫描件前，请**务必抹去或涂黑你的姓名、学号、邮箱**等个人隐私。
2. **遵守学术诚信**：严禁上传当前正在进行、尚未截止评分的平时作业代码与考试试题。
3. **格式推荐**：优先提供 PDF、Markdown、高清原图（PNG/JPG）或 ZIP 打包源码。

---

## 🤖 自动化与机器可读协议

> ⚠️ **重要提醒**：
> 请**绝对不要手动修改**根目录下的 `catalog.json`！
> 
> 当你的 Pull Request 被合并入 `main` 分支后，GitHub Actions 会**全自动运行检测引擎**，几秒钟内重新生成全库索引，并将你贡献的宝贵资料同步至「上科大老学长」等各大智能学业助手，让全校同学即刻收益！
