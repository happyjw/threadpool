# threadpool
一个简单的线程池
使用方法：
##### 一.首先创建一个线程池对象，可以直接定义对象或new一个对象例如:
1. ThreadPool pool(2,true);
2. ThreadPool *pPool = new ThreadPool(2,true);
###### 第一个参数表示线程池的线程数量，第二个参数表示线程池是否延迟关闭
##### 二.向线程池里加入要执行的任务,每个要执行的任务都需要写一个继承自的Task类并改写Run函数.
##### 三.调用PoolStart开启线程池,然后调用PushTask向线程池加入要执行的任务
##### 四.调用PoolStop停止线程执行
##### 具体参见Demo
