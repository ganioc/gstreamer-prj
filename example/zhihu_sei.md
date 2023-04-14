请教大佬个问题，您这个代码是怎么加到pipeline里头的。是有element的事件触发回调，还是加自己写了个element放管线里头呢？

03-29
​回复
​赞
苦樱
苦樱

书包
谢谢，另外请教大佬一个可能是比较基础的问题。因为我是打算在每一帧前添加一个相应的SEI，我在pad里生成SEI帧的buffer以后，如何把SEI buffer和视频帧的buffer一同在pad回调函数里送出去呢？
或者我的理解有问题，有其他更方便的方法
04-01
​回复
​赞
书包
书包
作者
​
appsrc appsink 或者用pad 探针

04-01
​回复
​赞
Lfountain
Lfountain
大佬，能请教一些这方面的问题吗
2022-07-05
​回复
​赞
sucaptain
sucaptain
大佬 能分享下完整代码吗, 在这两个有些变量未定义呢