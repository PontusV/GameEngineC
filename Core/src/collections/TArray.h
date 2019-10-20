#ifndef TEMPLATE_ARRAY_H
#define TEMPLATE_ARRAY_H
#include <vector>
#include <initializer_list>
#include <iterator>
#include <cstddef>
#include <memory>
/* Templated dynamic array. Requires the templated type to be copyable and default constructed. */
template<class T, class Allocator = std::allocator<T>>
class TArray {
public:
	typedef typename std::vector<T, Allocator>::iterator iterator;
	typedef typename std::vector<T, Allocator>::const_iterator const_iterator;

	TArray(std::initializer_list<T> list) : data(list) {
	}
	TArray(std::vector<T, Allocator> vector) : data(vector) {
	}
	~TArray() {
	}

	void push_back(T&& element) {
		data.push_back(element);
	}
	T& emplace_back(T&& element) {
		return data.emplace_back(element);
	}
	iterator erase(iterator it) {
		return data.erase(it);
	}
	void pop_back() {
		data.pop_back();
	}
	void swap(TArray<T, Allocator>& other) {
		data.swap(other);
	}
	bool empty() {
		return data.empty();
	}
	void insert(iterator it, std::initializer_list<T> list) {
		data.insert(it, list);
	}

	void resize(std::size_t size) {
		data.resize(size);
	}
	std::size_t getSize() {
		return data.size();
	}

	iterator begin() {
		return data.begin();
	}
	const_iterator begin() const {
		return data.begin();
	}
	iterator end() {
		return data.end();
	}
	const_iterator end() const {
		return data.end();
	}
	std::reverse_iterator<iterator> rbegin() {
		return data.rbegin();
	}
	std::reverse_iterator<iterator> rend() {
		return data.rend();
	}

	T& front() {
		return data.front();
	}
	T& back() {
		return data.back();
	}
	T& at(std::size_t index) {
		return data.at(index);
	}
	T& operator[](std::size_t index) {
		return at(index);
	}
private:
	std::vector<T, Allocator> data;
};
#endif