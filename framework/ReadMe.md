this->template
在基类中，定义了一个模板类，实现了xxx函数，在子类中调用它的时候需要用使用template关键字。

std::is_array
typename std::enable_if<!std::is_array<U>::value>::type

enable_if

decltype()
返回操作数的数据类型

typedef创建了存在类型的别名，而typename告诉编译器std::vector<T>::size_type是一个类型而不是一个成员。

enable_shared_from_this<T>
shared_from_this
当类A被share_ptr管理，且在类A的成员函数里需要把当前类对象作为参数传给其他函数时，就需要传递一个指向自身的share_ptr
