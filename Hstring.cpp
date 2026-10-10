#include "Hstring.h"
#include <cstring>
#include <iostream>

// 默认构造函数
Hstring::Hstring() 
    : buffer_(new char[INIT_BUFF_SIZE]{})
    , buffer_size_(INIT_BUFF_SIZE)
    , data_length_(0)   {}    

// 有参构造函数
Hstring::Hstring(const char* str)
    : buffer_(new char[INIT_BUFF_SIZE]{})
    , buffer_size_(INIT_BUFF_SIZE)
    , data_length_(0)
{
    append(str);
}

// 拷贝构造函数 -- 委托构造
Hstring::Hstring(const Hstring& other) : Hstring(other.c_str()) {}
    
// 析构函数
Hstring::~Hstring()
{
    delete[] buffer_;
    buffer_ = nullptr;
}

//追加字符串
void Hstring::append(const char* str)
{
    size_t str_length = strlen(str);
    if(str_length + data_length_ >= buffer_size_)
    {
        size_t new_buffer_size = buffer_size_ * 2;
        while(new_buffer_size <= str_length + data_length_)
        {
            new_buffer_size *= 2;
        }
        char* new_buffer = new char[new_buffer_size]{};
        memcpy(new_buffer, buffer_, data_length_);
        
        //防止悬挂指针
        delete[] buffer_;
        buffer_ = nullptr;

        buffer_ = new_buffer;
        buffer_size_ = new_buffer_size;
    }
    memcpy(buffer_ + data_length_, str, str_length);
    data_length_ += str_length;
}

//获取字符串内容
const char* Hstring::c_str() const
{
    return buffer_;
}

//获取字符串长度
size_t Hstring::length() const
{
    return data_length_;
}


//重载 +
Hstring& Hstring::operator+(const Hstring& other)
{
    append(other.c_str());
    return *this;
}
Hstring& Hstring::operator+(const char* str)
{
    append(str);
    return *this;
}

//重载 -
Hstring& Hstring::operator-(const Hstring& other)
{
    KMP kmp(buffer_, data_length_, other.c_str(), other.length());
    int index = kmp.search();
    if(index != -1)
    {
        //参数1：目标地址，参数2：源地址，参数3：拷贝的字节数
        std::memmove(buffer_ + index, buffer_ + index + other.length(), data_length_ - index - other.length() + 1);

        data_length_ -= other.length(); 
    }
    else
    {
        std::cout << "字串未找到!" << std::endl;
    }
    return *this;
}

//修改操作
Hstring& Hstring::replace(int index, int len, const Hstring& other)
{
    if(index < 0 || index >= data_length_ || len < 0)
    {
        std::cout << "索引或长度无效!" << std::endl;
        return *this;
    }
    if(index + len >= data_length_)
    {
        size_t new_buffer_size = buffer_size_ * 2;
        while(new_buffer_size <= index + len)
        {
            new_buffer_size *= 2;
        }
        char* new_buffer = new char[new_buffer_size]{};
        memcpy(new_buffer, buffer_, index);
        memcpy(new_buffer + index, other.c_str(), other.length());
        memcpy(new_buffer + index + other.length(), buffer_ + index + len, data_length_ - index - len + 1);

        delete[] buffer_;
        buffer_ = nullptr;

        buffer_ = new_buffer;
        buffer_size_ = new_buffer_size;
        data_length_ = data_length_ - len + other.length();
    }
    else
    {
        std::memmove(buffer_ + index + other.length(), buffer_ + index + len, data_length_ - index - len + 1);
        memcpy(buffer_ + index, other.c_str(), other.length());
        data_length_ = data_length_ - len + other.length();
    }
    return *this;
}

// 查找
int Hstring::find(const Hstring& substr) const
{
    KMP kmp(buffer_, data_length_, substr.c_str(), substr.length());
    return kmp.search();
}

//重载 = 运算符
Hstring& Hstring::operator=(const Hstring& other)
{
    if(this != &other)
    {
        delete[] buffer_;
        buffer_ = nullptr;

        buffer_size_ = INIT_BUFF_SIZE;
        data_length_ = 0;
        
        buffer_ = new char[buffer_size_]{};
        append(other.c_str());
    }
    return *this;
}
#if 0
Hstring& Hstring::operator=(int value)
{
    char str[20];
    // snprintf 是 C/C++ 中用于按照指定格式，将数据写入字符缓冲区的函数。
    //参数1：目标地址，参数2：最大拷贝长度，参数3：格式化字符串，参数4：要格式化的值
    snprintf(str, sizeof(str), "%d", value);
    delete[] buffer_;
    buffer_ = nullptr;

    buffer_size_ = INIT_BUFF_SIZE;
    data_length_ = 0;

    buffer_ = new char[buffer_size_]{};
    append(str);
    return *this;
}
#endif

Hstring& Hstring::operator=(int value)
{
    delete[] buffer_;
    buffer_ = nullptr;


    buffer_size_ = INIT_BUFF_SIZE;
    buffer_ = new char[buffer_size_]{};
    
    
    bool sign = value < 0;
    //安全处理 INT_MIN，避免取负时发生溢出。 INT_MIN：-2147483648
    unsigned int result = sign ? 0u - static_cast<unsigned int>(value) : static_cast<unsigned int>(value);

    //转换成字符
    int i = 0;
    if(sign)
    {
        buffer_[i++] = '-';
    }
    int start = i;
    while(result != 0)
    {
        buffer_[i++] = static_cast<char>(result % 10 + '0');
        result /= 10;
    }

    //反转
    int left = start;
    int right = i - 1;
    Reversal(buffer_, left, right);

    data_length_ = i;

    return *this;
}

//反转
void Hstring::Reversal(char* arr, int left, int right)
{
    while(left < right)
    {
        char temp = arr[left];
        arr[left++] = arr[right];
        arr[right--] = temp;
    }
}

//KMP算法实现
int* Hstring::KMP::build_next()
{
    int* next = new int[substr_len_] {};
    next[0] = 0;
    int prefix_len = 0; //当前共同后缀长度
    int i = 0;
    while(i < substr_len_)
    {
        if(substr_[i] == substr_[prefix_len])
        {
            prefix_len++;
            next[i] = prefix_len;
            i++;
        }
        else if(prefix_len > 0)
        {
            prefix_len = next[prefix_len - 1];
        }
        else
        {
            next[i] = 0;
            i++;
        }
    }
    return next;
}                

int Hstring::KMP::search()
{
    int* next = build_next();
    int i = 0;
    int j = 0;
    while (i < str_len_)
    {
        if(str_[i] == substr_[j])
        {
            i++;
            j++;
        }
        else if(j > 0)
        {
            j = next[j - 1];
        }
        else
        {
            i++;
        }
        if(j == substr_len_)
        {
            return i - j;
        }
    }
    return -1;
}        