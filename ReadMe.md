这个工程实现了在编译期间创建的特定事件类型的存储转发

使用和注意

EventHandler&lt;Series&gt; | EventListener&lt;DerivedClass, Event1, ...&gt;
-------------|---------------
事件转发的中心，接收事件数据，注册Listener，转发数据  |  实现相应事件的回调函数，我们需要继承这个类，然后编写回调函数，接着注册到EventHandler中。

### 内存生命周期问题
* 事件数据可以是任何类型或者结构体，EventHandler将会使用完美转发的形式，在堆上复制构造这个事件数据，外界无需关心Handler内部事件的生命周期。
* 注册到EventHandler中的Listener支持自动注销，无论Listener是在**栈**上还是在**堆**中，当Listener析构的时候，就会自动注销。


### 提示
* 由于使用了模板，一些局部代码注册的Event类型也会出现在EventHandler中，可能会给事件分发时的性能带来一定的损耗。
* 通过为EventHandler类型的模板参数指定不同的类型，可以实现多种EventHandler，每个EventHandler是完全独立的，相互不影响。
* Listener支持注册多个EventHandler<Series>。


------
# 使用举例：
## 定义事件结构体：
随意定义结构都可以
```c++
struct EventA
{
	int data;
};
struct EventB
{
	int data;
};

struct EventC
{
	int data;
};

struct EventD
{
	int data;
};

struct EventE
{
	int data;
};
```

## 定义Listener  
注意继承模板类中的参数，第一个模板参数是**当前这个Listener自己的类型**，
接着是所有需要监听的事件结构类型，**每个**事件类型都必须有相应的回调函数，否则会报错。  
回调函数的名字**必须**是**ListenEvent**，参数是不同的Event类型（一个参数指针）。

```c++
class AdvanceListener
	:public Event::EventListener<AdvanceListener, EventA, EventB>
{
public:
    // call back function
	void ListenEvent(EventA * e)
	{
		printf("TestListener get EventA\n");
		eventAData = e->data;
	}

    // call back function
	void ListenEvent(EventB * e)
	{
		printf("TestListener get EventB\n");
		eventBData = e->data;
	}

public:
    // internal data
	int eventAData = 0;
	int eventBData = 0;
};

// another listener sample
class SecondAdvanceListener
	:public Event::EventListener<SecondAdvanceListener, EventC, EventD, EventE>
{
public:
    // call back function
	void ListenEvent(EventC * e)
	{
		printf("SAL recieve a EventC\n");
		eventCData += e->data;
	}
    
    // call back function
	void ListenEvent(EventD * e)
	{
		printf("SAL recieve a EventD\n");
		eventDData += e->data;
	}
    
    // call back function
	void ListenEvent(EventE * e)
	{
		printf("SAL recieve a EventE\n");
		eventEData += e->data;
	}

public:
    // internal data
	int eventCData = 0;
	int eventDData = 0;
	int eventEData = 0;
};

```

## 创建EventHandler
首先定义一个类型用来标记不同的EventHandler，这里使用一个空结构体类型作为EventHandler的模板参数，标记一个独立的EventHandler。
```c++
struct SampleSeries{};
```
声明EventHandler，同时用类型**SampleSeries**作为一个标记，和其他EventHandler区别。  
由于一些遗留的代码问题，这里的演示创建了一个EventHandler的对象，但是其实所有的方法都是静态的，完全可以不创建这个对象。
```c++
Event::EventHandler<SampleSeries> eHandler;
```

## 创建并注册Listener
注意使用指针来传递参数。
```c++
AdvanceListener             advListener;
SecondAdvanceListener       secondAListener;
eHandler.registerListener(  &advListener);
eHandler.registerListener(  &secondAListener);
```

## 定义事件消息
```c++
EventA testEventA;
EventB testEventB;

EventC testEventC_1;
EventD testEventD_1;
EventE testEventE_1;
//||||||||||||||||||
EventD testEventD_2;
EventE testEventE_2;

testEventA.data = 5;
testEventB.data = 8;
testEventC_1.data = 1;
testEventD_1.data = 2;
testEventE_1.data = 3;
//|||||||||||||||||||
testEventD_2.data = 4;
testEventE_2.data = 5;
```
## 发送消息
这里直接传递普通的参数，内部将会调用复制构造函数创建相应的事件对象。
```c++
eHandler.sendEvent(testEventA);
eHandler.sendEvent(testEventB);
eHandler.sendEvent(testEventC_1);
eHandler.sendEvent(testEventD_1);
eHandler.sendEvent(testEventE_1);
eHandler.sendEvent(testEventD_2);
eHandler.sendEvent(testEventE_2);
```

## EventHandler 分派消息
```c++
eHandler.dispatchAll();
```
如果执行了以上的所有步骤，相应Listener的回调函数就会被调用，在EventTest项目中有相关的代码，可以打断点的观察一下信息的分发是否正常。

## Listener的注销
只要脱离了Listener的作用域，它就会自动注销，当然你也可以调用unregisterListener手动注销，这个函数不会和自动注销发生冲突。

