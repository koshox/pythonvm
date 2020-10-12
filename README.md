# 自己动手写Python虚拟机

[《自己动手写Python虚拟机》](https://book.douban.com/subject/34442805/)源码

### 开发环境

win10 + cygwin + clion

### 主要功能

1. 基本实现了书上所有的功能，从零构建一个简单的Python虚拟机。

2. 除了GC还存在一些问题，需要参考作者的新项目HiLang改造为链表记录对象的创建。

3. 对随书源码的几个小问题做了修改，CMakeLists也做了一点调整。



最后一章实现了yield和generator以后，就可以写出一个最简单的协程：

```python
# Two simple generator functions
def countdown(n):
    while n > 0:
        print('T-minus', n)
        yield
        n -= 1
    print('Blastoff!')


def countup(n):
    x = 0
    while x < n:
        print('Counting up', x)
        yield
        x += 1


class TaskScheduler:
    def __init__(self):
        self._task_queue = []

    def new_task(self, task):
        '''
        Admit a newly started task to the scheduler

        '''
        self._task_queue.append(task)

    def run(self):
        '''
        Run until there are no more tasks
        '''
        while len(self._task_queue) > 0:
            task = self._task_queue.popleft()
            try:
                # Run until the next yield statement
                task.next()
                self._task_queue.append(task)

            except StopIteration:
                # Generator is no longer executing
                pass


# Example use
sched = TaskScheduler()
sched.new_task(countdown(2))
sched.new_task(countup(5))
sched.run()
```

输出结果：

```py
[T-minus, 2]
[Counting up, 0]
[T-minus, 1]
[Counting up, 1]
Blastoff!
[Counting up, 2]
[Counting up, 3]
[Counting up, 4]
```



