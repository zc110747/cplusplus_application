# smart_point

智能指针是C++中的一种特殊类型的指针，它提供了自动内存管理的功能。
智能指针通过在对象不再被使用时自动释放其所指向的内存，从而避免了内存泄漏的问题。
C++标准库提供了三种主要的智能指针：unique_ptr、shared_ptr和weak_ptr。

## unique_ptr

unique_ptr是一种独占所有权的智能指针，它确保同一时间只有一个unique_ptr指向某个对象。
当unique_ptr被销毁时，它所指向的对象也会被自动删除。unique_ptr不支持复制操作，但可以通过std::move进行所有权的转移
std::make_unique是创建std::unique_ptr实例的推荐方式

## shared_ptr

shared_ptr是一种共享所有权的智能指针，它允许多个shared_ptr指向同一个对象。
当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除。shared_ptr通过引用计数来管理对象的生命周期。

## weak_ptr

weak_ptr是一种弱引用的智能指针，它指向由shared_ptr管理的对象，但不会增加对象的引用计数。
weak_ptr主要用于解决循环引用的问题，避免内存泄漏。
