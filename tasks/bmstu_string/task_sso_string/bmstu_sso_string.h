#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
template <typename T>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename T>
class basic_string 
{
   private:
	static constexpr size_t SSO_CAPACITY =
		(sizeof(T*) + sizeof(size_t) + sizeof(size_t)) / sizeof(T) - 1;

	struct LongString
	{
		T* ptr;
		size_t size;
		size_t capacity;
	};

	struct ShortString
	{
		T buffer[SSO_CAPACITY + 1];
		unsigned char size;
	};

	union Data
	{
		LongString long_str;
		ShortString short_str;
	};

	Data data_;
	bool is_long_;

	bool is_long() const { return is_long_; }

	T* get_ptr(){
		return is_long_==true ? data_.long_str.ptr : data_.short_str.buffer;
	}

	const T* get_ptr() const {
		return is_long_==true ? data_.long_str.ptr : data_.short_str.buffer;
	}

	size_t get_size() const {
		return is_long_==true ? data_.long_str.size : static_cast<size_t>(data_.short_str.size);
	}

	size_t get_capacity() const {
		return is_long_==true ? data_.long_str.capacity : SSO_CAPACITY;
	}

   public:
	basic_string(){
		is_long_=false;
		data_.short_str.size=0;
		data_.short_str.buffer[0]=static_cast<T>(0);

	}

	basic_string(size_t size) {
		is_long_ = false;
		if (size<=SSO_CAPACITY){
			data_.short_str.size=static_cast<unsigned char>(size);
			for(size_t i = 0; i!=size; i++){
				data_.short_str.buffer[i]= static_cast<T>(0);
			}
		}
		else{
			is_long_=true;
			data_.long_str.ptr=new T[size+1];
			data_.long_str.size = size;
			data_.long_str.capacity = size;
			for(size_t i = 0; i!=size; i++){
				data_.long_str.ptr[i]= static_cast<T>(0);
			}
		}
	}

	basic_string(std::initializer_list<T> il){
		size_t len = il.size();
		is_long_= len > SSO_CAPACITY;
		if (is_long_ == true){
			data_.long_str.ptr = new T[len+1];
			data_.long_str.size = len;
			data_.long_str.capacity = len;
			size_t i = 0;
			for(auto z: il){
				data_.long_str.ptr[i]=z; 
				i++;
			}
			data_.long_str.ptr[len] = static_cast<T>(0);
		}
		else{
			data_.short_str.size=static_cast<unsigned char>(len);
			size_t  i = 0;
			for(auto z: il){
				data_.short_str.buffer[i]=z;
				i++;
			}
			data_.short_str.buffer[len]= static_cast<T>(0);
		}
	}

	basic_string(const T* c_str){
		size_t len = 0;
		while(c_str[len]!='\0') len++;

		is_long_=len < SSO_CAPACITY;

		if(is_long_==false){
			data_.short_str.size = static_cast<unsigned char>(len);
			for(size_t i=0; i<=len; i++){
				data_.short_str.buffer[i]=c_str[i];
			}
		}
		else{
			data_.long_str.ptr = new T[len+1];
			data_.long_str.size = len;
			data_.long_str.capacity = len;
			for(size_t i = 0; i<=len; i++){
				data_.long_str.ptr[i] = c_str[i];
			}
		}
	}

	basic_string(const basic_string& other){
		const T* source = other.get_ptr();
		size_t len = other.get_size();
		is_long_=len < SSO_CAPACITY;
		if(is_long_==false){
			data_.short_str.size = other.get_size();
			for(size_t i=0; i<=len; i++){
				data_.short_str.buffer[i]=source[i];
			}
		}
		else{
			data_.long_str.ptr = new T[len+1];
			data_.long_str.size = len;
			data_.long_str.capacity = len;
			for(size_t i = 0; i<=len; i++){
				data_.long_str.ptr[i] = source[i];
			}
		}
	}

	basic_string(basic_string&& dying) noexcept {
		is_long_=dying.is_long_;
		if (is_long_==true){
			data_.long_str = dying.data_.long_str;
	
			dying.is_long_=false;
			dying.data_.long_str.ptr = nullptr;
			dying.data_.long_str.size = 0  ;
			dying.data_.long_str.capacity = 0;
		}
		else{
			data_.short_str = dying.data_.short_str;
			dying.data_.short_str.size = 0;
			dying.data_.short_str.buffer[0]= static_cast<T>(0);
		}
	}

	~basic_string() {
		if (is_long_) delete[] data_.long_str.ptr;
		else delete[] data_.short_str.buffer;
	}

	const T* c_str() const {
		return is_long_==true ? data_.long_str.ptr : data_.short_str.buffer;
	}

	size_t size() const {
		return is_long_==true ? data_.long_str.size : static_cast<size_t>(data_.short_str.size);
	}

	bool is_using_sso() const { return !is_long_; }

	size_t capacity() const {
		return is_long_==true ? data_.long_str.capacity : SSO_CAPACITY;
	}

	basic_string& operator=(basic_string&& other) noexcept { return *this; }

	basic_string& operator=(const T* c_str) { return *this; }

	basic_string& operator=(const basic_string& other) { return *this; }

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		return {};
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		return is;
	}

	basic_string& operator+=(const basic_string& other) { return *this; }

	basic_string& operator+=(T symbol) { return *this; }

	T& operator[](size_t index) noexcept
	{
		return is_long_==true ? data_.long_str.ptr[index] : data_.short_str.buffer[index];
	}

	T& at(size_t index){
		if(is_long_ == true){
			if(data_.long_str.size<index){throw std::out_of_range("Wrong index");}
			else{return data_.long_str.ptr[index];}
		}
		else{
			if(static_cast<size_t>(data_.short_str.size)< index){throw std::out_of_range("Wrong index");}
			else{return data_.short_str.buffer[index];}
		}
	}

	T* data() {
		return is_long_==true ? data_.long_str.ptr : data_.short_str.buffer;
	}

   private:
	static size_t strlen_(const T* str) { return 0; }

	void clean_() {}
};
}  // namespace bmstu
