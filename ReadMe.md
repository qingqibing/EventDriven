这个工程实现了在编译期间创建的特定事件类型的存储转发

使用和注意

EventHandler&lt;Series&gt; | EventListener&lt;DerivedClass, Event1, ...&gt;
-------------|---------------
这是整个事件转发的中心，所有的事件结构体的指针都发送到这个类中的静态方法中  |  这是一个实现事件响应代码的类型，我们需要继承这个类，然后编写回调函数，接着注册到EventHandler中。

### 内存生命周期问题
* 所有的事件对象必须在堆上分配，由EventHandler负责释放内存
* 由于EventHandler大量使用静态成员和方法，所以EventListener最好是一个全局的变量，避免野指针。
* EventListener可以在栈上分配，但是要注意手动释放。


### 提示
* 这个事件可以是任何类型，但是发送消息的时候是直接传递事件的指针，所以需要使用者自己维护这个指针的生命周期。
* 由于使用了模板进行编译期的改动，使用了static变量的特性，所有在使用了EventHandler的项目中所有的Event类型都会出现，这是和类型相关的。哪怕只是在局部的某个函数中向EventHandler发送了一个特别的Event，EventHandler中也会出现和这个Event相关的存储结构体。所以请一定要注意在EventHandler中的Event指针和delegate（listener）指针的声明周期，需要手动维护。
* 第二条表明这个EventHandler适合贯穿整个项目的观察者模式，尽量不要在局部使用，如果在局部使用，请确保能够手动维护event和delegate的声明周期（注销listener的方法还没有目前还没有实现，等待实现中...）

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
注意继承模板类中的参数，第一个是**当前这个Listener自己**，
接着是所有需要监听的事件结构类型。
回调函数的名字**必须**是**ListenEvent**，参数是不同的Event类型。
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
```c++
Event::EventHandler eHandler;
```

## 创建并注册Listener
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
```c++
eHandler.sendEvent(&testEventA);
eHandler.sendEvent(&testEventB);
eHandler.sendEvent(&testEventC_1);
eHandler.sendEvent(&testEventD_1);
eHandler.sendEvent(&testEventE_1);
eHandler.sendEvent(&testEventD_2);
eHandler.sendEvent(&testEventE_2);
```

## EventHandler 分派消息
```c++
eHandler.dispatchAll();
```

如果执行了以上的所有步骤，就可以直接在对应的Listener中发现对应的存储信息发生了改变，在EventTest项目中有相关的代码，可以实时的观察一下信息的分发是否正常。
