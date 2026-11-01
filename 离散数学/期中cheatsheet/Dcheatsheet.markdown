<font size=2>


* __LEC1__ FTA:$n=p_1^{e_1}p_2^{e_2}……p_n^{e_n}$for every $n>1$


* __LEC2__ $a,b,c \in Z,gcd(c,a)=1,ab|c \Rightarrow b|c $
* $p\  is\ a\ prime,p|ab \Rightarrow p|a \ or\ p|b$
* a,b用FTA表示后$gcd(a,b)$的FTA表示方法


* __LEC3__ Euler function
  $n=p_1^{e_1}p_2^{e_2}……p_n^{e_n}$,$φ(n)=φ(p_1^{e_1})φ(p_2^{e_2})……φ(p_n^{e_n})
  =n(1-p^{-1}_1)(1-p^{-1}_2)……(1-p_n^{-1})$
* 欧拉定理
  $gcd(a,n)=1,a^{φ(n)}=1$
  涉及到乘法阶的概念：对于$Z^*_n$中的元素，每个元素的乘法阶都是$φ(n)$的因数
* 费马小定理
  当欧拉定理中的n为素数，记作p
  $a \in Z_p^* \Rightarrow a^p=a \Leftrightarrow a^{p-1}=1$
  其中$p-1$即为$φ(p)$


* __LEC4__ 公钥私钥生成：
(1)生成$p q$(2)$n=pq$(3)$m=φ(n)$(4)找到$e$使得$gcd(e,m)=1$(5)找到$d$使得$d$为$e$在模$m$意义下的逆元(6)公钥为$(n,e)$，私钥为$(n,d)$
* 加密解密：
明文x加密：$y=x^e\ mod\ m$；密文y解密：$x=y^d\ mod\ m$


* __LEC6__ 拓展欧几里得：$mx+ny=c$,$nx_0+(m\%n)y_0=c$,则$x=y_0$,$y=x_0-⌊m/n⌋y_0$


* __LEC7__ 中国剩余定理：对于线性同余方程组，若每个方程组的模数$n_i$互质，则必有解
* 中国剩余映射：$n=n_1,n_2,……,n_k$(两两互质)，$Z_n \Leftrightarrow Z_{n_1}* Z_{n_2}*……*Z_{n_k}$


* __LEC7__ （封闭性）（结合律）（单位元存在性）（逆元存在性）$ab=ba$


* __LEC8__ field:一个abelian group装配上+ ·两种运算。满足分配率
* $f(X)=(X-a)Q(X)+f(a)$其中Q比f低一次、多项式的根及根的数量
* 集合的order和元素的order
  * 对于order为m的multiplicative Abelian group：所有元素a均满足$a^m=1$,即每个元素的order为m的因数
* p为素数时，$Z_P^*$为循环群


* __LEC9__ (1)约定大素数$P$，原根$G$(2)Alice生成$a$，Bob生成$b$。(3)Alice端：$A=G^a\ mod\ P$(4)Bob端：$B=G^b\ mod\ P$(5)AB互传:Alice:$S_1=B^a\ mod\ P$；；Bob:$S_2=A^b\ mod\ P$；；$S=S_1=S_2$，为密钥。


* __LEC10__ 康托尔对角法则：$b_i$与任何一个$a_{ii}$都不相同→证明了整数集和实数集的非双射
* 康托尔定理：集合的势小于其幂集的势
* 可数集为有限集或整数集的双射集、两可数集的并集可数、两可数集的笛卡尔积仍可数


* __LEC11__ 唱票问题→卡特兰数(?????)


* __LEC12__ 可重复集的计数：n种元素，选r个组合：${n+r-1 \choose r}$
* 二项式反演 
$g_n$:0-n种方案；$f_n$：n种方案。
$g_n=\sum^n_{i=0}{n\choose i}f_i$  ；； $f_n=\sum^n_{i=0}{n \choose i}(-1)^{n-1}g_i$


* __LEC13__ n无→k有（type2）→${n+k-1} \choose {n}$
* 第二类斯特林数$S(n,k)=S(n-1,k-1)+k*S(n-1,k)$
应用场景：四种分配情况
  * 有 无 非空 $s(n,k)$(type3)
  * 有 有 非空 $s(n,k)*k!={n!\over {n_1!n_2!……n_k!}}$(type1)
  可空则为非空的求和
* 通项公式：
应用4中：$g_n=k^n$
应用2中：$f_n=s(n,k)*k!$
由二项式反演：$g_n=\sum^n_{i=0}{f_i}{n \choose i}$
∴$s(n,k)={1\over{k!}}{\sum_{i=0}^{k-1}(-1)^i{k \choose i}(k-i)^n}=\sum^{k-1}_{i=0}\frac{(-1)^{k-1}i^n}{i!(k-i)!}$
* $p_4(6)=p_4(2+4)=p_1(2)+p_2(2)+p_3(2)+p_4(2)=1+1+0+0=2$(type4)


* __LEC14__ 线性齐次递推数列
  * 解方程:$r_n=c_1r^{n-1}+……+c_kr^{n-k}$
$a_n=(A_0+A_1n+……+A_{k1}n^{k_1}){r_1}^n+(B_0+B_1n+……+B_{k_2}n^{k_2})r_2^n+……$(tips:$k_1$ $k_2$等均为$r_1$ $r_2$对应的重根数量)
即：每个不重复的实根对应一个多项式乘上自身的n次方。多项式的次数等于重复次数-1
* 线性非齐次递推数列
  * 研究的类型：$a_n=c_1a_{n-1}+c_2a_{n-2}+……+c_ka_{n-k}+F(n)$,其中$F(n)=(f_ln^l+f_{l-1}n^{l-1}+……+f_1n+f_0)s^n$
  * 解方程：令$y_n$为$a_n=c_1a_{n-1}+c_2a_{n-2}+……+c_ka_{n-k}$的通解,$x_n$为原方程的一个特解。
    * $x_n$的求解：对于y_n对应的特征方程$r_n=c_1r_{n-1}+c_2r_{n-2}+……+c_kr_{n-k}$,s即为其中的一解。记s的重复数为m。则：$x_n=(p_ln^l+p_{l-1}n^{l-1}+……+p_1n^1+p_0)s^nn^m$
    即：F(n)的多项式部分，直接将原系数换成待定系数，后面加上$s^n$与$n^m$，其中m需要在之前的通解$y_n$中得出。


* __LEC15__ $A(x)$可逆 iff $a_0≠0$,A(X)=$\sum_0^{\infin}x^n$,$A(x)^{-1}=1-x$
* $(1+ax)^u=\sum_{n=0}^{\infin}{u\choose n}(ax)^n$
$(1-ax)^{-u}=\sum_{n=0}^{\infin}{{n+u-1}\choose n}(ax)^n$
$(1-ax)^{-1}=\sum_{n=0}^{\infin}(ax)^n$
* $a_n=8a_{n-1}+10^{n-1},a_0=1$ $\rightarrow$ $A(x)=a_0+\sum_{n=1}^{\infin}{8a_{n-1}+10^{n-1}}x^n$ $\rightarrow$ $1+8xA(x)+{x\over{1-10x}}$ $\rightarrow$ $A(x)={{1-9x}\over{(1-8x)(1-10x)}}={{1\over2}\over{1-8x}}+{{1\over2}\over{1-10x}}={1\over2}\sum_{n=0}^{\infin}{8^nx^n}+{1\over2}\sum_{n=0}^{\infin}{10^nx^n}$ $\rightarrow$ $a_n={1\over 2}(8^n+10^n)$
* $\sum_{n=0}^{\infin}{a_nx^n}=\prod\sum_{n_i\in N_i}x^{n_i}$
type2:$20\rightarrow5,\geq3,\geq2,\geq4,\geq6$:$\sum_{n=0}^{\infin}a_nx^n=(x^3+)(x^2+)(x^4+)(x^6+)(1+)={x^{15}\over(1-x)^5}=x^{15}\sum_{m=0}^{\infin}{{-5\choose n}(-1)^mx^m}$ $\rightarrow$ $a_{20}={-5\choose 5}(-1)^5$
* $\sum_{n=0}^{\infin}{{a_n\over n!}x^n}=\prod\sum_{n_i\in N_i}{x^{n_i}\over {n_i!}}$
* $1+{x^2 \over 2!}+{x^4 \over 4!}+……={{e^x+e^{-x}}\over 2}$
${x \over 1!}+{x^3 \over 3!}+{x^5 \over 5!}+……={{e^x-e^{-x}}\over 2}$
$1+x+{x^2\over 2!}+{x^3\over 3!}+……=e^x$
* 容斥原理的奇技淫巧：+1-2+3-4……(交并转换式)