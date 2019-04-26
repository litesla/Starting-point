****

**正在维护当中。。。。。。**

****



## 爬虫项目

### 前期准备

通常不使用浏览器进行爬虫

1. pyenv
   * python 各版本不兼容，需要 pyenv 管理 python 环境，在各版本中切换
   * pyenv install 3.7.1
   * https://www.cnblogs.com/olajennings/p/8832481.html
2. pycharm
   * python 的 IDE
   * 推荐上官网下载 [博客](https://blog.csdn.net/qq_38683692/article/details/79868391)
   * pycharm 新建的项目放在`PycharmProjects`文件夹中
   * 打开 pycharm 中的 Terminal: `View -> ToolWindows -> Terminal`
3. anaconda
   * [官网下载](https://www.anaconda.com/distribution/)
   * [博客](https://www.jianshu.com/p/3e53407f8bd4)
   * [换源](https://blog.csdn.net/dream_allday/article/details/80344511)
   * 切换环境等常用命令
     * `conda activate 环境名`  进入某环境
     * `conda deactivate` 退出环境
     * `conda env list` 列举当前所有环境
     *  `conda create -n your_env_name python=3.5` 创建指定python版本的环境
     *  `activate your_env_name` 进入（激活）指定名字的环境
     *  `conda install lib_name` 安装指定库名
     *  `conda uninstall lib_name` 卸载指定库名
4. scrapy
   * [博客](https://blog.csdn.net/a6864657/article/details/82634187)
5. xpath



### 项目需求

统计某个在 OJ 上写了多少行代码



## 软件说明 

### Scrapy 的使用

说明：爬虫框架

1. 下载器（Download）
2. 分析器（Parse）
3. 存储器（Store, Save, Date, Item(一项、条目)）



#### 1.新建爬虫项目

```
scrapy startproject [爬虫项目的名字]
```



#### 2.新建爬虫

```
cd 
scrapy genspider [爬虫的名字] [爬虫的域名]
```

使用 fake-useragent 模块随机生成 User-Agent

[博客](https://www.cnblogs.com/yuqiangli0616/p/9277263.html)



```python
url = 开始的网址
callback = 分析的方法
```



#### 3.middlewares

```
from fake_useragent import UserAgent

class RandomUserAgentMiddleware(object):

    def __init__(self):
        self.agent = UserAgent()

    @classmethod
    def from_crawler(cls, crawler):
        return cls()

    def process_request(self, request, spider):
        request.headers.setdefault('User-Agent', self.agent.random)
```



#### 4.settings

**24 行左右**

```python
ROBOTSTXT_OBEY = False
```

**53 行左右**

```python
SPIDER_MIDDLEWARES = {
    'baidubaike.middlewares.BaidubaikeSpiderMiddleware': 543,
    'baidubaike.middlewares.RandomUserAgentMiddleware': 555,
}
```



#### 5.执行爬虫

```
scrapy crawl [爬虫名字](不加.py后缀)
```



#### 6.错误信息

>from fake_useragent import UserAgent
>ImportError: No module named fake_useragent

**解决方法**

```
sudo -H pip install fake-useragent
```