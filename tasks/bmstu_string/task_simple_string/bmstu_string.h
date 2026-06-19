#pragma once

#include <exception>
#include <iostream>


namespace bmstu
{

template <typename T>
class simple_basic_string;

typedef simple_basic_string<char> string;
typedef simple_basic_string<wchar_t> wstring;
typedef simple_basic_string<char16_t> u16string;
typedef simple_basic_string<char32_t> u32string;

template <typename T>
class simple_basic_string
{ 
   public:
	/// Конструктор по умолчанию
	simple_basic_string() : ptr_(new T[1]{0}), size_(0) {}

	simple_basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {
		ptr_[size_]=0;
	}

	simple_basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		size_t ind = 0;
		for(auto i = il.begin(); i != il.end(); i++ ){
			ptr_[ind] = *i;
			ind++;
		}
		ptr_[size_]=0;
	}

	/// Конструктор с параметром си-с
	simple_basic_string(const T* c_str) {
		size_=strlen_(c_str);
		ptr_=new T[size_ + 1];
		for (size_t i = 0; i<size_; i++){
			ptr_[i]= c_str[i];
		}
		ptr_[size_]=0;
	}

	/// Конструктор копирования
	simple_basic_string(const simple_basic_string& other) {
		size_=other.size_;
		ptr_=new T[size_ + 1];
		for ( size_t i =0; i<size_; i++){
			ptr_[i]= other.ptr_[i];
		}
		ptr_[size_]=0;
	}

	/// Перемещающий конструктор
	simple_basic_string(simple_basic_string&& dying) {
		ptr_=dying.ptr_;
		size_=dying.size_;
		dying.ptr_=nullptr;
		dying.size_=0;
	}

	/// Деструктор
	~simple_basic_string() {
		delete[] ptr_;
	}

	/// Геттер на си-строку
	const T* c_str() const { return ptr_; }

	size_t size() const { return size_; }

	/// Оператор копирующего присваивания
	simple_basic_string& operator=(simple_basic_string&& other)
	{
		if (this==&other){
			return *this;
		}
		delete[] ptr_;

		ptr_ = other.ptr_;
		size_=other.size_;
		other.size_=0;
		other.ptr_=nullptr;
		return *this;
	}

	/// Оператор копирующего присваивания си строки
	simple_basic_string& operator=(const T* c_str) {

		clean_();
		size_=strlen_(c_str);
		ptr_=new T[size_ +1];
		for (size_t i = 0; i< size_; i++){
			ptr_[i]= c_str[i];
		}
		ptr_[size_]=0;
		
		return *this; }

	/// Оператор копирующего присваивания
	simple_basic_string& operator=(const simple_basic_string& other)
	{
		if (this==&other){
			return *this;
		}
		clean_();
		size_=other.size_;
		ptr_=new T[size_ +1];
		for (size_t i = 0; i< size_; i++){
			ptr_[i]= other.ptr_[i];
		}
		ptr_[size_]=0;
		return *this;
	}

	friend simple_basic_string<T> operator+(const simple_basic_string<T>& left,
											const simple_basic_string<T>& right)
	{
		simple_basic_string res(left.size_ + right.size_);
		for (size_t i = 0; i< left.size_; i++){
			res.ptr_[i]= left.ptr_[i];
		}
		for (size_t i = 0; i< right.size_; i++){
			res.ptr_[left.size_ + i]= right.ptr_[i];
		}
		res.ptr_[res.size_]=0;
		return res;
	}

	template <typename S>
	friend S& operator<<(S& os, const simple_basic_string& obj)
	{
		os<< obj.c_str();
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, simple_basic_string& obj)
	{
		return is;
	}

	simple_basic_string& operator+=(const simple_basic_string& other)
	{
		*this = *this + other;
		return *this;
	}

	simple_basic_string& operator+=(T symbol) {
		simple_basic_string tmp(size_ + 1);
		for (size_t i = 0; i<size_; i++){
			tmp.ptr_[i]=ptr_[i];
		}
		tmp.ptr_[size_] = symbol;
		tmp.ptr_[size_+1]=0;
		*this = std::move(tmp);
		return *this; }

	T& operator[](size_t index) noexcept { return ptr_[index]; }

	T& at(size_t index) {
		if(index >= size_){
		throw std::out_of_range("Wrong index");
		}
		return ptr_[index];
	}

	T* data() { return ptr_; }

   private:
	static size_t strlen_(const T* str) {
		size_t len = 0;
		while(str[len]!= 0){len++;}
		return len; }

	void clean_() {
		delete[] ptr_;
		ptr_=nullptr;
		size_ = 0;
	}

	T* ptr_ = nullptr;
	size_t size_ = 0;
};
}
 // namespace bmstu