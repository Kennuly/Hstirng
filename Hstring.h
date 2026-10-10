#pragma once

#include <cstddef>
class Hstring
{
public:
    Hstring();
    Hstring(const char* str);
    Hstring(const Hstring& other);
    ~Hstring();

    //获取字符串长度
    size_t length() const;

    //获取字符串内容
    const char* c_str() const;

    //重载 +
    Hstring& operator+(const Hstring& other);
    //这个重载可以不写，调用时会自动调用构造函数将const char*转换为Hstring对象
    Hstring& operator+(const char* str);

    //重载 - 
    Hstring& operator-(const Hstring& other);

    //修改操作
    Hstring& replace(int index, int len, const Hstring& other);

    // 查找
    int find(const Hstring& substr) const;

    //重载 = 运算符
    Hstring& operator=(const Hstring& other);
    Hstring& operator=(int value);

private:
    //追加字符串
    void append(const char* str);
    //字符串反转
    void Reversal(char* arr, int left, int right);
    
    class KMP
    {
        const char* substr_;
        size_t substr_len_;
        const char* str_;
        size_t str_len_;
    public:
        KMP(const char* str, size_t str_len, const char* substr, const size_t substr_len)
        : str_(str), str_len_(str_len), substr_(substr), substr_len_(substr_len)
        {}
        ~KMP(){}
        int* build_next();
        int search();
    };

private:
    char* buffer_;      //动态缓冲区（含初始缓冲区）
    size_t buffer_size_;    //缓冲区总容量
    size_t data_length_;  //实际存储字符串长度
    static const size_t INIT_BUFF_SIZE = 128;   //初始化缓冲区大小
    
};


