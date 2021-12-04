// This is simple tasks handling class. Part of Krivenko Slava's codebase. Copyright 2021.
// define FX_SYS_CALL_IMPLEMENTATION somewhere to compile 
#ifndef SYS_CALL_H
#define SYS_CALL_H

#include <stdlib.h> // for core system() function

namespace Internal {

	// Struct for handling 'out_of_range' situations
	struct out_of_range {};

	// Dynamic Array Template class that mostly acts like std::vector, but this one independent
	template<typename _Ty>
	class DArray {
		using value_t = _Ty;
		using pointer = _Ty*;
		using const_pointer = const _Ty*;
		using reference = _Ty&;
		using const_reference = const _Ty&;
	public:
		// Iterators class
		class iterator {
		public:
			iterator(pointer p) : _Current(p)  {}
			iterator& operator++() { _Current++; return *this; }
			iterator& operator--() { _Current--; return *this; }
			reference operator*() { return *_Current; }
			bool operator==(const iterator& _Rhs) const { return (*_Current == *_Rhs._Current); }
			bool operator!=(const iterator& _Rhs) const { return !((*_Current == _Rhs._Current)); }
		private:
			pointer _Current;
		};

		// Default constructor
		DArray() : _Size(0), _Vals(0), _Space(0) {}
		// Default explicit constructor
		explicit DArray(int s) : _Size(s), _Vals(new value_t[s]), _Space(s) {
			for (unsigned int i = 0; i < _Size; ++i)
				_Vals[i] = value_t();
		}
		// Copy constructor
		DArray(const DArray& _Rhs) : _Size(_Rhs._Size), _Vals(new value_t[_Rhs._Size]) {
			for (unsigned int i = 0; i < _Size; ++i)
				_Vals[i] = _Rhs._Vals[i];
		}
		// Copy assingment
		DArray<value_t>& operator=(const DArray<value_t>& _Rhs) {
			if (this == &_Rhs) return *this;
			if (_Rhs._Size <= _Space)
				for (unsigned int i = 0; i < _Rhs._Size; ++i) {
					_Vals[i] = _Rhs[i];
					_Size = _Rhs._Size;
					return *this;
				}
			pointer p = new value_t[_Rhs._Size];
			for (unsigned int i = 0; i < _Rhs._Size; ++i)
				p[i] = _Rhs._Vals[i];
			delete[] _Vals;
			_Size = _Rhs._Size;
			_Space = _Rhs._Size;
			_Vals = p;
			return *this;
		}
		// Destructor
		~DArray() { }

		// Begin iterator
		iterator begin() { return iterator(&_Vals[0]); }
		// Begin iterator
		const iterator begin() const { return iterator(&_Vals[0]); }
		// End iterator
		iterator end() { return iterator(&_Vals[_Size]); }
		// End iterator
		const iterator end() const { return iterator(&_Vals[_Size]); }
		// Begin iterator
		const iterator cbegin() const { return iterator(&_Vals[0]); }
		// End iterator
		const iterator cend() const { return iterator(&_Vals[_Size]); }
		// @returns True if DArray has nothing
		bool empty() const { return (_Size == 0); }
		// @returns Current DArray capacity
		unsigned int capacity() const { return _Space; }
		// @returns Current size, how much elements contain
		unsigned int size() const { return _Size; }
		// Takes care about our data (idk what to say)
		void reserve(unsigned int nm) {
			if (nm <= _Space) return;
			pointer p = new value_t[nm];
			for (unsigned int i = 0; i < _Size; ++i)
				p[i] = _Vals[i];
			delete[] _Vals;
			_Vals = p;
			_Space = nm;
		}
		// Resizing our DArray to fit to new size, with default parameter for 
		void resize(unsigned int ns, value_t val = value_t()) {
			this->reserve(ns);
			for (unsigned int i = _Size; i < ns; ++i)
				_Vals[i] = value_t();
			_Size = ns;
		}
		// Clear DArray. What else?..
		void clear() {
			delete[] _Vals;
			_Size = 0;
			_Space = 0;
		}
		// Passing your value of type into DArray
		void push_back(const_reference v) {
			if (_Space == 0)
				reserve(8);
			else if (_Size == _Space)
				reserve(2 * _Space);
			_Vals[_Size] = v;
			++_Size;
		}
		// Popping you value if exists, otherwise may cause error
		void pop_back() {
			_Vals[_Size] = 0;
			--_Size;
		}

		// @returns Reference to your object at current position, if it exists
		reference at(unsigned int pos) {
			if (pos < 0 || _Size <= pos) throw out_of_range();
			return _Vals[pos];
		}
		// @returns Reference to your object at current position, if it exists
		const_reference at(unsigned int pos) const {
			if (pos < 0 || _Size <= pos) throw out_of_range();
			return _Vals[pos];
		}
		// @returns Reference to your object at current position, if it exists
		reference operator[](unsigned int i) { return _Vals[i]; }
		// @returns Reference to your object at current position, if it exists
		const_reference operator[](unsigned int i) const { return _Vals[i]; }
		// @returns Reference to first object
		reference front() { return _Vals[0]; }
		// @returns Reference to first object
		const_reference front() const { return _Vals[0]; }
		// @returns Reference to last object
		reference back() { return _Vals[_Size - 1]; }
		// @returns Reference to last object
		const_reference back() const { return _Vals[_Size - 1]; }
		// @returns Pointer to your data
		pointer data() { return _Vals; }
		// @returns Pointer to your data
		const_pointer data() const { return _Vals; }

	private:
		unsigned int _Size, _Space;
		pointer _Vals;
	};

}

constexpr unsigned char PriorityNull= 0;
constexpr unsigned char PriorityLowLow = 1;
constexpr unsigned char PriorityLow = 2;
constexpr unsigned char PriorityLowMed = 3;
constexpr unsigned char PriorityDefault = 4;
constexpr unsigned char PriorityMedMax = 5;
constexpr unsigned char PriorityMax = 6;
constexpr unsigned char PriorityMaxMax = 7;

// Struct that represents tasking by priority with command string
struct SysTask {
	const char* command;
	unsigned int priority;

	SysTask() = default;
	SysTask(const char* cmd, unsigned int prior) : command(cmd), priority(prior) { }
	SysTask(const char* cmd) : command(cmd), priority(PriorityDefault) { }

	bool operator==(const SysTask _Rhs) const { return (command == _Rhs.command && priority == _Rhs.priority); }
	bool operator!=(const SysTask _Rhs) const { return (!(*this == _Rhs)); }
	void operator=(SysTask* _Rhs) const {}
};

#define INVALID_TASK SysTask("null", PriorityNull)

using SysQueue = Internal::DArray<SysTask>;

// Big boy class, that handling and processing all tasks
class TaskManager {
public:
	// Add task to the queue
	// NOTE: if you are adding task filled with string command only, it will gets highest priority
	SysTask AddTask(SysTask task);
	// Insert task to an existing position if 'find' has in queue
	void InsertTask(SysTask insert, SysTask find);
	// Erase task from queue
	// @returns True if erase successfull, otherwise false
	void EraseTask(SysTask task);
	// Executes queue filled with tasks
	// - If several tasks has the same priority and commands, they are will be executed by order
	// - You can pass in one task instance and it will be executed immediately, but if queue contain it.
	bool Execute(SysTask task = INVALID_TASK, bool erase_explicit = false);
	// Execute unhandled task with the command immediately
	// - You can pass in just a regular string
	void RunExplicit(SysTask task);
private:
	SysQueue main_queue;
};

	// Main Class Instance that works with all System Tasks Stuff.
	static TaskManager SysCall;

#ifdef SYS_CALL_IMPLEMENTATION

namespace Utils {

	// Task Compare
	// @returns True if equals, otherwise false
	bool TaskCmdCmp(SysTask a, SysTask b)
	{
		if (a.command == b.command) {
			b = a; 
			return true;
		}
		return false;
	}

	// @param task Returns the finded task
	// @returns Position of finded task, otherwise zero
	int FindTaskByCommand(SysQueue queue, SysTask* task)
	{
		if (queue.size() == 0) return -1; // Check if queue filled with something
		for (unsigned int i = 0; i < queue.size(); i++) // Go through queue
			if (TaskCmdCmp(queue[i], *task)) { // Compare tasks from queue with the given
				return i; // if find one, 'TaskCmp' will pass it in second parameter, and then we are return position of finded task
			} 
		return -1; // in we don't found comparing tasks, return negative value, that indicates this.
	}

	// @returns Position of finded task, otherwise zero
	int FindTask(SysQueue queue, SysTask* task)
	{
		if (queue.size() == 0) return -1;
		for (unsigned int i = 0; i < queue.size(); i++)
			if (!TaskCmdCmp(*task, {"null"})) // if task has an command passed in, go find this
				return FindTaskByCommand(queue, task);
		return -1;
	}
}

SysTask TaskManager::AddTask(SysTask task)
{
	main_queue.push_back(task); // just passing in our task
	return task; // and return it
}

void TaskManager::InsertTask(SysTask insert, SysTask find)
{
	// gets position of our needed task, or negative value if it's not exists at all
	int pos = Utils::FindTask(main_queue, &find);
	if (pos != -1)
		main_queue.at(pos) = insert;
}

void TaskManager::EraseTask(SysTask task)
{
	int pos = Utils::FindTask(main_queue, &task); // Find task position
	if (pos != -1) // Check if task exists
		main_queue.at(pos) = INVALID_TASK; // set it to invalid one
}

bool TaskManager::Execute(SysTask task, bool erase_explicit)
{
	if (main_queue.size() == 0)  // Check if queue contain anything
		return false;
	if (task != INVALID_TASK) { // if we passed something in parameter
		int pos = Utils::FindTaskByCommand(main_queue, &task); // Find task position
		if (pos != -1) { // check if task exists
			RunExplicit(main_queue.at(task.priority)); // execute it immediately
			if (erase_explicit) // if we need to call it once...
				main_queue.at(pos) = INVALID_TASK; // erase it from queue, we don't need this anymore
			return true; // true, execution successfull!
		}
		return false; // oh man, your task isn't in queue
	}
	for (unsigned char prior = PriorityMaxMax; prior >= PriorityLow; prior--) // Iterate priority from highest to lowest
		for (unsigned int i = 0; i < main_queue.size(); i++) { // Go through queue
			SysTask task = main_queue[i];  // Get task example
			if (task == INVALID_TASK || task.priority != prior) continue; // if it invalid(erased), or priority is not compatible, skip for now
			RunExplicit(task); // otherwise, execute it right now!
		}
	main_queue.clear(); // clear our queue
	return true; // true, execution successful!
}

void TaskManager::RunExplicit(SysTask task)
{
	::system(task.command); // TODO: replace it with something faster!
}

#endif // !SYS_CALL_IMPLEMENTATION

#endif // !SYS_CALL_H