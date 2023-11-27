2023 Raw Question List
======================
* virtual
    * [x] is dynamic binding turned on once there's a single virtual in or above
        - no. function level
    * [x] once turned on, compiler has no choice but to use dynamic binding?
        - yes
        - it is turned on at compile time to emit code to look up vtable
    * [x] is dynamic binding turned on on per-function level? 
        - yes
    * [x] is virtual key word top-down or bottom-up? 
        - top-down
    * [x] is virtual on the level of class? or function? 
        - function
    * [x] is virtual inheritance even a word? 
        - yes, but it has different meaning than "having 1+ virtual function"
    * [x] why is virtual never removed
        - a chain
    * [x] "turning on" happens at compile time? run time? 
        - at compile 
    * [x] do we need to mark virtual all the way thru? or just at the very top level? 
        - just at the very top
* override
    * [x] can i override without the keyword override? 
        - yes
    * [x] override keyword: just a safety measure? 
        - yeh
    * [x] can i shift section, e.g. private -> public? 
        - probably no
        - actually yes, it's just accessibility, nothing more
* final
    * [x] what if you mark a function final but the class has a subclass? 
        - ok
    * [x] can subclass still hide the final function? 
        - not if signature match completely
        - yes otherwise
    * [x] what if final but not virtual? 
        - you can't 
* abstract
    * [x] is it on the level of class? or function? 
        - class. class with 1+ pure virtual function
    * [x] =0
        - uhuh
    * [x] is it just a special case of virtual class
        - no concept of virtual class
        - but yes, a special case of polymorphic class
    * [x] does leaving a function declared but not defined make it abstract?
        - nope
    * [x] how does function generation work with abstract class
        - same
    * [x] all derived must implement the function? 
        - right
    * [x] can pure virtual function have implementation? 
        - yes
    * [x] can = 0 be added to non-virtual? 
        - no
    * [x] can you provide a constructor to class with a pure virtual function? 
        - yes
    * [x] can abstract class have non virtual function? 
        - yes
* override vs hide
    * [x] when everything match, how is override different from hide? 
        - different concept
    * [x] when you override, don't you effectively hide?  
        - no. it's different
* virtual table
    * [x] how does virtual table work!
        - the table explains it all
        - one table per polymorphic type
        - each contains pointers to virtual functions of itself
        - one pointer per instance to vtable
    * [x] what is the colavita answer? 
        - don't think he's completely right
    * [x] Objects don’t need more than one vptr, so once the price has been paid, it won’t get ‘charged’ again [2].
    * [x] A lie! Multiple inheritance(why?) and Virtual inheritance(why?) mean that this statement is incorrect. But, we haven’t seen those yet.?
* virtual destructor
    * [x] when do you need a virtual destructor, and everything related to destructor
    * [x] for the class declaring a virtual func and all its offsprings? 
        * yes
    * [x] having a virtual destructor alone will create a vtable, right? 
        - yes
    * [x] why don't non-virtual need virtual destructor? 
        * you should not use it as another type
    * [x] when do we ever not need a virtual destructor? 
        - non-polymorphic class
* side cast and dynamic binding
    ```cpp
    A {
        virtual void demo();
    };
    B : A {};
    C : A {
        void demo();
    }

    C used as B? 
    ```
    - not like this


2022 Raw Question List
======================
* public, protected, private inheritance
    * [x] is it only the relationship that's not known by others
        - just the relationship
    * [x] or is all the variables unaccessible
        - members are as normal
    * [x] which is default
        - private
        - multiple inheritance must go like `: public A1, public A2`

* data member
    * [x] single inheritance: layout of members - which order
        - base class variables have lower addr, i.e. |a|a|b|b|...
    * [x] what if base class var shadows derived; 
        - it can only be the other way around

* object lifetime
    * [x] ctor
        * [x] syntax
            - just a normal ctor
        * [x] do we inherit base class's ctor
            - no
        * [x] what if no ctor provided
            - default instantiate everything
        * [x] is ctor hided or not inherited at all?
            - this question doesn't apply
            - not part of the interface even
    * [x] dtor
        * [x] do you need to explicitly destruct members?
            - no. calls base dtor, even for user supplied
        * [x] what if you explicitly destruct members? will their dtor be called twice?
            - yes, it will be called twice
            - what does this question even mean
    * [x] is assignment operator inherited? 
        - yes, but hided by the compiler generated one
    * [x] what about other operators? 
        - yes, and there's no compiler generated ones

* function resolution
    * [x] what if a function with the name but wrong number of arguments is found in the derived class? 
        - error
        - cannot override, but can hide
    * [x] when hiding/overriding a base class function, do we need to declare it in the derived class'es definition? 
        - yes you do. It is scoped differently
    * [x] is it possible to first search up then search down? 
        - nope you're overthinking it
    * [x] why does dynamic binding have to be dynamic? 
        - what do you think
    * [x] when scope resolution used, always static binding even for virtual function? 
        - right
    * [x] any difference between reference vs pointer to the object
        - no difference in terms of function resolution

* overriding and hiding
    * [x] overloading vs hiding, which takes precedence
        - hiding; first find name, then stay in that scope
        - well they are completely incomparable concepts
    * [x] difference between overriding and hiding
        - In method overriding, when base class reference variable pointing to the object of the derived class, 
        then it will call the overridden method in the derived class.
        - In the method hiding, when base class reference variable pointing to the object of the derived class, 
        then it will call the hidden method in the base class.

* virtual
    * [x] which member function to use
        * especially when 2 parents have the same function
            - ambiguous
            * can I specify which to use?
                - you can `b.A::demo();`
    * [x] override
        * [x] what is the purpose of using override except for avoiding typos?
            - just that
    * [x] static method and dynamic binding? 
        - no dynamic binding for static method
        - no object at all
    * [x] syntax: where does final, override, and virtual go? 
        - after, after, before

* virtual table
    * [x] a vtable for both base and derived? 
        - yes. they are separate
    * [x] multiple functions, same vptr? 
        - same pointer to the virtual table, different pointer to functions

* pure virtual
    * [x] abstract class as an interface?
        * but this will come with the side effect that we always use the actual function, right? 
            - right, and this is desired 
            - can still use scope resolution
            - nonono you can actuallly make all pure virtual
    * [x] is it just a virtual function without implementation?
        - nope
    * [x] are all functions virtual in a pure virtual class? 
        - not necessarily
    * [x] what is the syntax for pure virtual function with definition
        - just def outside
    * [x] how to make a class pure virtual
        - class is not pure virtual
    * [x] how to make a function pure virtual
        * [x] just by not implement it? 
            - nope
        * [x] what's the difference between not implementing it and = 0? and = delete? 
            - completely different
        * [x] = 0 make it virtual vs make it not accessible? 
            - what is even this question
    * [x] is it possible to have a pure virtual dtor? 
        - it is. but you still need impl

* multiple inheritance
    * [x] what is virtual inheritance
        - diamond problem
    * [x] multiple inheritance, what's the address if we cast it to the 2nd base class type
        - indeed different!
    * [x] multiple inheritance: which ctor to call by default
        - in the order of inheritance
    * [x] multiple inheritance: layout of members
        - order of inheritance, i.e. |a1|a1|a2|a2|b|b|...

* cast
    * slicing
        * [x] object slicing happens in what scenario
            - construct an instance of base class with a derived one
        * [x] does slicing already happen during upcasting?
            - nope if reference or pointer
    * dynamic cast
        * [x] no need for try-catch?
            - can throw exception
            - but you might want to fail hard? 
        * [x] "error: 'A' is not polymorphic"
            - right, the cast-away type must be polymorphic
    * side cast 
        - [x] conversion between 2 base classes that are both parents to a derived class

* paradigm
    * [x] what is substitution
        - derived to be treated as a base 
    * [x] what is polymorphism 
        - well at least you now know what polymorphic type is
    * [x] rethink what should be in base class, what should be made virtual, 
    and their relationship with substitute vs code reuse
        - see summary

* others
    * function overloading
        * const T, T the same? 
            - yes
        * T&, T the same? 
        * const reference of copy con, assignment, are they redefinition? 
    * implicit cast, static_cast, c style cast
        * and reference, revisit
    * [x] the link problem at hrt
        - linker order