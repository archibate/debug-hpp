# debug.hpp

便捷地打印任何对象！

[English](README.md) | [简体中文](README.zh_CN.md)

用法:
```cpp
debug(), "我的变量是", your_variable;
```

输出:
```
  your_file.cpp:233:  我的变量是 {1, 2, 3}
```

> 假设your_variable是一个`std::vector`

**警告**: debug()仅在`Debug`版本中有效! 在`Release`版本中它会自动禁用 (我们通过检查NDEBUG宏是否被定义来实现这一点)。是的，在`Release`版本中完全没有输出，这是有意设计的。

这是一个很方便的功能：无需在调试后手忙脚乱地删除所有的debug()语句，您只需切换到`Release`版本，所有的debug都会自动消失，没有任何运行时开销！需要调试时，只需再次切换回`Debug`版本，之前写的debug()语句都会重新开始工作。

如果你确实需要在`Release`版本中使用debug()，可以在包含此头文件之前加上`#define DEBUG_LEVEL 1`。

我们还支持断言检查:
```cpp
debug().check(some_variable) > 0;
```

当`some_variable > 0`为false时，将触发一个'陷阱'中断（MSVC下为__debugbreak，GCC下为__builtin_trap，可配置，见下文），以便调试器捕获，同时打印人类可读的错误消息:
  your_file.cpp:233:  assertion failed: 3 < 0


调试完成后，无需手忙脚乱地删除所有debug()！只需:
```cpp
#define NDEBUG
```
就可以抑制所有debug()输出和断言检查，完全没有运行时开销。对于CMake或Visual Studio用户，只需切换到`Release`版本即可抑制debug()输出。因为它们会在`Release`、`RelWithDebInfo`和`MinSizeRel`构建类型中自动为您定义`NDEBUG`。


总之，这是C++程序员都梦寐以求的一个有用的调试工具:

1. 使用简洁的逗号语法打印，易于使用
2. 支持打印STL对象，包括string、vector、tuple、optional、variant、unique_ptr、type_info等（需要C++20）
3. 只需添加一个名为`repr`的成员方法，例如`std::string repr() const { ... }`即可支持打印您的自定义类!
4. 暂不支持打印的类将显示为类似`[TypeName@0xdeadbeaf]`的形式，其中0xdeadbeaf是对象的地址。
5. 高度可定制，通过定义DEBUG_xxx宏来自定义行为（见下文）
6. 调试完成后，只需简简单单`#define NDEBUG`，整个库就都禁用了，没有任何运行时开销
7. 线程安全，每行消息始终是独立的，没有烦人的交错输出冲进控制台（这是你使用cout时的典型体验）。


# 可配置选项

以下是一些可配置的宏，**在包含此头文件之前**定义它们即可生效:

* `#define DEBUG_LEVEL 0` (定义了NDEBUG时默认) - 禁用debug输出，完全没有运行时开销
* `#define DEBUG_LEVEL 1` (!定义了NDEBUG时默认) - 启用debug输出，打印你要求打印的所有内容
* `#define DEBUG_LEVEL 2` - 启用带有详细源代码级别信息的debug输出（需要可读的源文件）

* `#define DEBUG_DEFAULT_OUTPUT std::cerr` (默认) - 控制debug的输出写到哪里

* `#define DEBUG_PANIC_METHOD 0` - 当断言失败时抛出一个运行时错误，错误消息为debug字符串
* `#define DEBUG_PANIC_METHOD 1` (默认) - 当断言失败时打印错误消息，然后触发一个'陷阱'中断，方便调试器捕获，如果没有调试器附加，程序将终止
* `#define DEBUG_PANIC_METHOD 2` - 当断言失败时打印错误消息，然后调用std::terminate
* `#define DEBUG_PANIC_METHOD 3` - 当断言失败时打印错误消息，不终止，也不抛出任何异常

* `#define DEBUG_REPR_NAME repr` (默认) - 如果对象x有一个成员函数`x.repr()`或一个支持`repr(x)`的全局函数`repr`，则将打印`x.repr()`或`repr(x)`的值。

* `#define DEBUG_RANGE_BRACE "{}"` (默认) - 控制范围类对象（支持begin(x)和end(x)）的格式，如"{1, 2, 3, ...}"
* `#define DEBUG_RANGE_COMMA ", "` (默认) - 同上

* `#define DEBUG_TUPLE_BRACE "{}"` (默认) - 控制元组类对象（支持std::tuple_size<X>）的格式，如"{1, 2, 3}"
* `#define DEBUG_TUPLE_COMMA ", "` (默认) - 同上

* `#define DEBUG_MAGIC_ENUM magic_enum::enum_name` - 启用打印枚举的名称而不是值，需要你有magic_enum.hpp

* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 0` (默认) - 打印无符号整数为十进制
* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 1` - 打印无符号整数为十六进制
* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 2` - 打印无符号整数为全宽十六进制

* `#define DEBUG_HEXADECIMAL_UPPERCASE 0` (默认) - 打印十六进制值为小写（a-f）
* `#define DEBUG_HEXADECIMAL_UPPERCASE 1` - 打印十六进制值为大写（A-F）

* `#define DEBUG_SUPRESS_NON_ASCII 0` (默认) - 将std::string中的非ascii字符视为可打印的（如UTF-8编码的中文字符）
* `#define DEBUG_SUPRESS_NON_ASCII 1` - 将std::string中的非ascii字符视为不可打印的（作为'\xfe'这样打印）

* `#define DEBUG_SHOW_NULLOPT "nullopt"` (默认) - 控制如何打印optional类对象（支持*x和(bool)x）当它是nullopt时

* `#define DEBUG_NAMESPACE_BEGIN` (默认) - 在全局命名空间中暴露debug
* `#define DEBUG_NAMESPACE_END` (默认) - 同上

* `#define DEBUG_NAMESPACE_BEGIN namespace mydebugger {` (默认) - 在命名空间`mydebugger`中暴露debug，使用时如`mydebugger::debug()`
* `#define DEBUG_NAMESPACE_END }` (默认) - 同上

* `#define DEBUG_CLASS_NAME debug` (默认) - debug类的默认名称是`debug()`，你可以在这里定义你自己的名称
