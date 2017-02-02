# Necessary Part
### What is an overload?
Allows you to specify more than one definition for a function name or an operator in the same scope.

### Why does Mark Isaacson hate fabs?
You effecively have to have different names for they same type of operation depending on the type.
abs for ints fabs for floats/doubles

### Does plain c allow overloads?
No, C does not supports function overloading. Printf works using a feature in C called varargs.
There is only one printf function, but the compiler uses a special calling convention to call it,
where whatever arguments you provide are put in sequence on the stack.

### In C++ pattern matching, what is the typical way to make patterns more board?
What? Are we talking about templates with types and adding more?

### How are patterns made more specific?
Is this traits for the types?

### What is your understanding of "more specific overload"?
Are we talking about adding traits or generic types? 

This has to do with adding more and more specific traits?  The concept of going from say
an iterator, to forwads and backwards, to random access etc?

This can also be done with explicit types

### Can the compiler deduce template arguments to a template function call?
Yes see example

```cpp
#include <iostream>
template <class T, class R>
  T sum(T a, R b){
    return a + b;
  }

int main() {
  auto i = sum(1,2);
  if ( i == (int) i && i == 3)
    std::cout << "ints!!!!!!" << std::endl;
  
  auto j = sum(1.6,2);
  if (j == (double)j && j == 3.6)
    std::cout << "double to int" << std::endl;
}
```

```
gcc version 4.6.3
   
ints!!!!!!
double to int
```

### Is it possible to specify some template arguments and have the compiler deduce other template arguments within the same funtion call?
Yes see example

```cpp
#include <iostream>
template <class T, class R>
  T sum(T a, R b){
    return a + b;
  }

int main() {
  auto i = sum(1,2);
    std::cout << i << std::endl;
  
  auto j = sum<int>(1.6,2);
  if (j == (int)j && j == 3)
    std::cout << "double to int" << std::endl;
  
  auto k = sum<double>(1.2,2);
  if (k == (double)k && k == 3.2)
    std::cout << "ints converted" << std::endl;
}
```

### Is the order of templates arguments a matter of style?
Functionally order of template arguments determine how the template gets interpreted and 
the way types get resolved / inferred. 

I understand why you put this here but what I was trying to get at is constructing templates
requires clear and coherent thought.  See the following:

```cpp
template <class T, class R>
  void advance(T& i, R n) {
    typename iterator_traits<T>::iterator_category category;
    detail::advance_dispatch(i, n, category);
  }
}

template <class InputIterator, class Distance>
  void advance(InputIterator& i, Distance n) {
    typename iterator_traits<InputIterator>::iterator_category category;
    detail::advance_dispatch(i, n, category);
  }
}
```

### Do automatic conversions apply when deducing template parameters?
Yes see example

```cpp
#include <iostream>
template <class T>
  T sum(T a, T b){
    return a + b;
  }

int main() {
  auto i = sum(1,2);
    std::cout << i << std::endl;
  
  auto j = sum<int>(1.6,2);
  if (j == (int)j && j == 3)
    std::cout << "double to int" << std::endl;
  
  auto k = sum<double>(1.2,2);
  if (k == (double)k && k == 3.2)
    std::cout << "ints converted" << std::endl;
}
```

```
gcc version 4.6.3
   
3
double to int
ints converted
```

### What is type dispatch?
Could not find much information on type dispatching but did on tag:
http://www.generic-programming.org/languages/cpp/techniques.php#tag_dispatching

From what I understand it you to give specify which functions should be called based
on the tag that you give it. 

For example:

```cpp
namespace std {
  struct input_iterator_tag { };
  struct bidirectional_iterator_tag : input_iterator_tag { };
  struct random_access_iterator_tag : bidirectional_iterator_tag { };

  namespace detail {
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
      while (n--) ++i;
    }

    template <class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, 
       bidirectional_iterator_tag) {
      if (n >= 0)
        while (n--) ++i;
      else
        while (n++) --i;
    }

    template <class RandomAccessIterator, class Distance>
    void advance_dispatch(RandomAccessIterator& i, Distance n, 
       random_access_iterator_tag) {
      i += n;
    }
  }

  template <class InputIterator, class Distance>
  void advance(InputIterator& i, Distance n) {
    typename iterator_traits<InputIterator>::iterator_category category;
    detail::advance_dispatch(i, n, category);
  }
}
```

The relation between tag dispatching and traits classes is that the property used for dispatching
(in this case the iterator_category) is often accessed through a traits class. The main advance()
function uses the iterator_traits class to get the iterator_category. It then makes a call the the
overloaded advance_dispatch() function. The appropriate advance_dispatch() is selected by the compiler
based on whatever type the iterator_category resolves to, either input_iterator_tag, 
bidirectional_iterator_tag, or random_access_iterator_tag. A tag is simply a class whose only purpose
is to convey some property for use in tag dispatching and similar techniques. Inheritance of tags is
used to encode the refinement hierarchy of concepts, so that overloading can pick the most specific
algorithm based on the tag hierarchy. Refer to this page for a more detailed description of iterator tags.


### What are type traits?
They are they else-if-then of Types.  Allow you to make compile-time decisions based on types.

```cpp
// Example 3: A traits example
//
template <class T>
class SomeTemplate
{
    // generic implementation (1)
    ...
};
template <>
class SomeTemplate<char>
{
    // implementation tuned for char (2)
    ...
};
...
SomeTemplate<int> a;      // will use (1)
SomeTemplate<char*> b;    // will use (1)
SomeTemplate<char> c;     // will use (2)
```
https://erdani.com/publications/traits.html

### What is the heiarchy of iterator categories?
There are five (until C++17)six (since C++17) kinds of iterators: InputIterator, OutputIterator,
ForwardIterator, BidirectionalIterator, RandomAccessIterator, and ContiguousIterator (since C++17).
Instead of being defined by specific types, each category of iterator is defined by the operations
that can be performed on it. This definition means that any type that supports the necessary
operations can be used as an iterator -- for example, a pointer supports all of the operations
required by RandomAccessIterator, so a pointer can be used anywhere a RandomAccessIterator is expected.

