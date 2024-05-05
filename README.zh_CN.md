# debug.hpp

受够了手动遍历 vector 并逐个 cout 的低效调试？🥵

来试试这个单头文件库吧 🚀 无痛打印包括 STL 容器在内的任何对象！🤩

[English](README.md) | [简体中文](README.zh_CN.md)

- 从 C++11 到 C++20 都能支持 ✅
- 简洁的语法，易于使用 ✅
- 打印几乎所有STL类 ✅
- 支持自定义类 ✅
- 在Release构建中自动禁用 ✅
- 只需引入单个头文件 ✅
- 线程安全 ✅

## 🎨 用法

```cpp
debug(), "我的变量是", your_variable;
```

输出:

```
your_file.cpp:233:  我的变量是 {1, 2, 3}
```

> 假设your_variable是一个`std::vector`

> [!WARNING]
> debug()仅在`Debug`版本中有效! 在`Release`版本中它会自动禁用 (我们通过检查NDEBUG宏是否被定义来实现这一点)。是的，在`Release`版本中完全没有输出，这是有意设计的。

这是一个很方便的功能：无需在调试后手忙脚乱地删除所有的debug()语句，您只需切换到`Release`版本，所有的debug都会自动消失，没有任何运行时开销！需要调试时，只需再次切换回`Debug`版本，之前写的debug()语句都会重新开始工作。

如果你确实需要在`Release`版本中使用debug()，可以在包含此头文件之前加上`#define DEBUG_LEVEL 1`来强制保持启用。

## ✨ 打印自定义类型

```cpp
struct Student {
    std::string name;
    int age;

    // 通常直接返回一个 tuple 即可，debug() 将会像打印 tuple 一样打印这个类:
    auto repr() const {
        return std::make_tuple(name, age);
    }

    // 或者返回字符串：
    std::string repr() const {
        return "Student{name: " + name + " age: " + std::to_string(age) + "}";
    }

    // 返回 map 也可以，这里的 variant 将会被自动展开并打印:
    auto repr() const {
        return std::map<std::string>, std::variant<std::string, int>>({"name", name}, {"age", age});
    }
};

// 如果你不方便添加成员函数，也可以在 Student 所在的同一命名空间中定义一个全局函数 repr（多亏了 C++ 的 ADL 机制）:
inline auto repr(Student const &stu) {
    return std::make_tuple(name, age);
}
```

> [!WARNING]
> repr 必须加上 const 限定符！否则 debug() 将无法正常调用 repr 函数。

## 🎁 输出保存为字符串

```cpp
auto s = std::string(debug(), "我的变量是", your_variable);
// s 现在的内容: "your_file.cpp:233:  my variable is {1, 2, 3}"

auto s = std::string(debug().noloc(), "我的变量是", your_variable);
// s 现在的内容: "my variable is {1, 2, 3}"
```

## 📝 输出重定向到 spdlog

```cpp
#define DEBUG_OUTPUT(str) spdlog::info(x)  // 导入头文件前定义这个宏就可以控制 debug() 的结果输出到哪里
#include "debug.hpp"
```

## 🚩 断言检查

```cpp
debug().check(some_variable) > 0;
```

当`some_variable > 0`为false时，将触发一个'陷阱'中断（MSVC下为__debugbreak，GCC下为__builtin_trap，可配置，见下文），以便调试器捕获，同时打印包含具体数值的错误消息:

```
your_file.cpp:233:  assertion failed: 3 < 0
```

## 🌠 Release 模式

调试完成后，无需手忙脚乱地删除所有debug()！只需:
```cpp
#define NDEBUG
```
就可以抑制所有debug()输出和断言检查，完全没有运行时开销。对于CMake或Visual Studio用户，只需切换到`Release`版本即可抑制debug()输出。因为它们会在`Release`、`RelWithDebInfo`和`MinSizeRel`构建类型中自动为您定义`NDEBUG`。

## 😏 经过测试的编译器

- x86-64 gcc 4.8.1 (-std=c++11)
- x86-64 gcc 13.2 (-std=c++20)
- x86-64 gcc 9.1 (-std=c++11)
- x86-64 gcc 13.2 (-std=c++14)
- x86-64 gcc 9.1 (-std=c++17)
- x86-64 clang 6.0.0 (-std=c++11 -stdlib=libc++)
- armv7-a clang 9.0.0 (-std=c++11)
- armv8-a clang 18.1.0 (-std=c++23)
- x64 msvc v19.38 (/std:c++20)
- arm64 msvc v19.38 (/std:c++14)
- x64 msvc v19.31 (/std:c++17)
- x86 msvc v19.14 (/std:c++11)
- x64 msvc v19.latest (/std:c++latest)

详见 https://godbolt.org/z/jYdj4T44n

## 🏅 总结

总之，这是一个C++程序员都梦寐以求的实用调试工具:

1. 使用简洁的逗号语法打印，易于使用
2. 支持打印STL对象，包括string、vector、tuple、optional、variant、unique_ptr、type_info等
3. 只需添加一个名为`repr`的成员方法，例如`std::string repr() const { ... }`即可支持打印您的自定义类!
4. 暂不支持打印的类将显示为类似`[TypeName@0xdeadbeaf]`的形式，其中0xdeadbeaf是对象的地址。
5. 高度可定制，通过定义DEBUG_xxx宏来自定义行为（见下文）。
6. 调试完成后，只需简简单单`#define NDEBUG`，整个库就都禁用了，没有任何运行时开销。
7. 线程安全，每行消息始终是独立的，没有烦人的交错输出冲进控制台（这是你使用cout时的典型体验）。

## 😜 可配置选项

以下是一些可配置的宏，**在包含此头文件之前**定义它们即可生效:

* `#define DEBUG_LEVEL 0` (定义了NDEBUG时默认) - 禁用debug输出，完全没有运行时开销
* `#define DEBUG_LEVEL 1` (!定义了NDEBUG时默认) - 启用debug输出，打印你要求打印的所有内容

* `#define DEBUG_SHOW_LOCATION 1` (默认) - 在每一行调试输出前加上打印该信息的代码文件名和行号 (例如：file.cpp:233)
* `#define DEBUG_SHOW_LOCATION 0` - 不显示代码文件名和行号

* `#define DEBUG_OUTPUT std::cerr <<` (默认) - 控制debug的输出写到哪里（必须可以 DEBUG_OUTPUT(str) 的形式调用）

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

* `#define DEBUG_SHOW_SOURCE_CODE_LINE 1` - 启用带有详细源代码级别信息的debug输出（需要源码文件路径可读）

* `#define DEBUG_NULLOPT_STRING "nullopt"` (默认) - 控制如何打印空的optional类对象（支持*x和(bool)x）

* `#define DEBUG_NULLPTR_STRING "nullptr"` (默认) - 控制如何打印空的指针类对象（支持static_cast<void const volatile *>(x.get())）

* `#define DEBUG_SMART_POINTER_MODE 1` (default) - print smart pointer as raw pointer address (e.g. 0xdeadbeaf)
* `#define DEBUG_SMART_POINTER_MODE 2` - print smart pointer as content value unless nullptr (e.g. 42)
* `#define DEBUG_SMART_POINTER_MODE 3` - print smart pointer as both content value and raw pointer address (e.g. 42@0xdeadbeaf)

* `#define DEBUG_SMART_POINTER_MODE 1` (默认) - 打印智能指针为原始指针地址（例如0xdeadbeaf）
* `#define DEBUG_SMART_POINTER_MODE 2` - 打印智能指针为其指向的内容值，除非为nullptr（例如42）
* `#define DEBUG_SMART_POINTER_MODE 3` - 同时打印智能指针的内容值和原始指针地址（例如42@0xdeadbeaf）

* `#define DEBUG_NAMESPACE_BEGIN` (默认) - 在全局命名空间中暴露debug
* `#define DEBUG_NAMESPACE_END` (默认) - 同上

* `#define DEBUG_NAMESPACE_BEGIN namespace mydebugger {` - 在自定义命名空间`mydebugger`中暴露debug，使用时如`mydebugger::debug()`
* `#define DEBUG_NAMESPACE_END }` - 同上

* `#define DEBUG_CLASS_NAME debug` (默认) - debug类的默认名称是`debug()`，你可以在这里定义你自己的名称

## 💝 遇到问题？

对这个实用的打印工具你有什么想法？欢迎在 [GitHub issues](https://github.com/archibate/debug-hpp/issues) 中提出您的建议或功能需求！
