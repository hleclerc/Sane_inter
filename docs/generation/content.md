# What is Sane ?

Sane is a code generator, designed to **accelerate the development** of codes optimized for **execution speed and memory occupancy**.

In itself, the language does not provide any abstraction that would drive the programmers away from the hardware, nor any stubborn dogmatic rule that would perpetually force to spend time on workarounds. In place of that, Sane allows and promotes the use and development of **active libraries** (libraries that take part in the process of compilation). Teams and developers can therefore **choose the *right* level of abstraction**, depending on the context.

Sane is basically very close to C++, except for the **streamlined syntax**, the **compilation processes**, and last but not least, the dramatically improved **compile-time abilities** which notably drives the tools for **generative programming** and **compiler *aided* decisions**.

<!-- (notably for **generative programming** and **compiler aided decisions**) -->

Sane is the acronym of *Software engineers Are Not Evil*.

<!-- Otherwise, the name could have been BSWDNPA (Blocking The Steering Wheel Does Not Prevent Accidents). -->

## Organization of this document

This documentation starts with the base syntax. The main differences with the base C++ are then emphasized, to drive thereafter to applications on generative programming. It is then followed by examples of commonly used *optional* abstractions (asynchronous execution, security enforcements, ...), illustrating the principle of *active library*.

<!-- Zero-cost enforcements and abstractions are always present, but when it comes to compromises, new ideas or specific needs, we believe that the choice must come from teams  -->

<!-- enabling much better **control** (notably via **active libraries** for *powerful, transparent and controllable* extensions) and  (streamlined syntax, assumed meta and generative programming, deep introspection, etc...).

We believe that we are able to decide what kind of enforcements and abstractions we need, depending on the context. Zero-cost security enforcements are always 
 Things like enforced security, automated handling of asynchronicity, auto-vectorization or  can be absolutely awesome and strongly differentiating, but it depends of the kind of program, the stage (prototype, ...), the stakes, ... Thus, these kind of features are not in the core language but are in libraries 
-->

<!-- libraries. -->

# Base syntax

## Simplifications

For calls, functions definitions, ifs, ... the parentheses become optional. Semicolon, braces and returns have undergone the same treatment.

```python
# 'foo' is a function with two (unconstrained) parameters 'a' and 'b'
def foo a, b
    info a + b

# equivalent to foo( 5, min( 10, 15 ) )
# (Auto-calls are handled right to left)
foo 5, min 10, 15
```

Line continuations are automatic (thanks notably to operators and block structures).

```python
# b is the rhs of the '+'
foo 10, a +
    b

# argument list can be specified in block lines.
# In this case, the comma become optionnal
foo
    10
    a + b
```

But frantic one-liners, are still welcome to play the game :)

```python
# ';' acts as a carriage return
def foo a, b; info a + b

# '()' allows for block definitions
def bar a, b; ( c := a + b; 2 * c )
```

As a rule of thumb, if the compiler can take care of an *obvious* simplification, this simplification is enabled for the greater goods.

```python
# Std def of a lambda functions without parameter
l0 := () => 17 # std def of a lambda func without parameter
# in the following line, as ':=' can't be an argument spec.
# we safely assume that '=> 17' is a no parm lambda
l1 := => 17

# works of course with all the kinds of operators
foo
    => 17 # unnamed arg
    arg: => 17 # named arg
```

## Parameters

Parameters of callable objects (`def`s, `classes`s, `lambda`s, ...) can have default values, constraints (with free parameters) and can be selected using their names.

```python
# a must be a SI32 (32 bits signed integer)
# b and c have default values 'a + 1' and 'b + 1'
def foo a: SI32, b? a + 1, c? b + 1
    a + b + c

# argument may be named. Default values are computed on the go
foo 15, c: 18
```

By default, arguments is passed as *immutable references*.

"Deconstification" is possible, using the `mut` keyword.

```python
# modification of b is possible inside this function
def foo a, mut b
    b += a

# => 25
b := 10
foo 15, b
info b
```

## Qualifiers

Qualifiers like `private`, `protected`, `static`, `global`... may be associated with one or several declarations at the same time if written in sub-blocks.

```python
class Mesh[ ElemTypes ]
    # A public method
    def area
        elems.sum e => e.with_coords( e.nodes.map n => nodes[ n ] ).area

    # A private section (with two attributes)
    private
        nodes: Vec[ Node ]
        elems: HetVec[ ...ElemTypes ]
```


# Templates

The word "template" refers to that of C++. Template in Sane works roughly in the same way as in C++: functions code are actually generated according to the input types and needed compile-time known values. It enables a first-level of compile-time or static polymorphism.

The main differences with C++ are that:

* By default, every callable (`def`, `class`, `lambda`, ...) is a template. There's no special syntax for templates.
* The compiler acts like a server, enabling aggressive **per function caching and hashing**, for the compilation speed and the binary sizes.
* **Template parameters can be of any type**, 
* Selection of surdefinitions work with **programmable criteria** than can work at compile-time (preferred) or run-time (if needed).

## Priorities and conditions

An arbitrary number or surdefinitions may lie on the same scope. Selection may depend on programmable priorities (`pertinence`) and programmable conditions (`when`).

```python
# computed priority
def pow mat: Matrix, n: PositiveInteger 
            pertinence - cost_mult( mat, mat ) * log n
    n & 1 ? mat * pow( mat, n - 1 )
          : pow( mat * mat, n / 2 )

# + computed condition (handled at compile time in this case)
def pow mat: Matrix, n: Number
            when mat.sub_types.some( T => T.is_approximate )
            pertinence - cost_eigen( mat )
    e := eigen mat
    e.P.trans * pow( e.D, n ) * e.P
```

## Multidimensionnal vtables

The `virtual` keyword can work on any kind of argument, indifferently on methods or functions.

```python
# run-time selection using bidimensionnal vtables
def intersection_area virtual a: Square, virtual b: Triangle
    ...

# a (dynamic) polymorphic function
def weight a: GeometricObject, b: GeometricObject, density? 7800
    density * intersection_area a, b
```

> (Remark: there are different kinds of vtables in Sane. Adding a pointer at the beginning (for the simple mono-inheritance case) is the default solution -- very good on a general purpose -- but other choices exist)

## Template classes

**Classes parameters can be of any type**, as long as copy and equality can be Compile-Time handled, in a symbolic way (comparison of graphs) or not (comparison of known values).

This notably does not exclude the types that may imply memory allocation as `String`, `Vec[...]`, ... In this case, the comparison are symbolic and can be handled at Compile-Time.

```python
# `sizes` can be defined using any kind of array
class PoolBySize[ sizes: ArrayLike, mt? true, name? "anon" ]
    class Item
        free_ptr: NullableAT # nullable address
        page_vec: Vec[ AT ]  # contiguous array of addresses
    # StaticMap constructs a Compile-Time tree
    map: StaticMap[ sizes, Item ] 
    ...

# instantiation for a given set of sizes (handled at CT)
pool := PoolBySize[ [ 16, 24, 32 ] ]()
```

## Variadic arguments

Callable parameters (for `def`, `class`, `=>`, ...) can be variadic, with optional constraints. It construct a compile-time known `Varargs` object, containing the optional names and the argument references.

```python
def foo a, ...b: SI32, c: StringLike
    info a, b, c         # -> 4, [5,arg_name:6], "bar"
    info b[ 0 ]          # -> 5
    info b[ "arg_name" ] # -> 6
    info b.values        # -> [5, 6]
    info b.names         # -> [arg_name]

foo 4, 5, arg_name: 6, "bar"
```


## Spread operator

The `...` operator can also be used expand stuff, notably for calls and list definitions.

`...` works with every kinds of lists containing a compile-time runnable `spread` method.

```python
# ex. 1: call with an expanded Varargs 
def foo a, ...b
    bar ...b, 654

# ex. 2: expansion in a list
u := 2 .. 10 # range( 2, 10 )
v := [ 1, ...u, 10 ]
```


# Objects

## Constructors

As with C++, constructors allow a *pre-construction* phase, to initialize attributes only once and directly with the right arguments.

Unlike with C++, pre-construction may occur in a block, **with arbitrary interleaved instructions**.

This block can be delimited explicitly (`wpc`) or implicitly (the last *visible* `init_of` call). `init_of attr_name, [ ctor_args ]` allows to initialize an argument. `init_of self, [ ctor_args ]` enables to call another constructor of the same class.

```python
class MyClass
    def construct
        # explicit pre-construction block (with interleaved instructions)
        wpc
            init_of a, 564 
            z := 12
            init_of b, 654, z
        # -> "normal" construction (*all* arg are now pre-initialized)
        print a + b

    # equivalent ctor with automatic wpc block
    def construct
        # implicit pre-construction block
        init_of a, 564 
        z := 12
        init_of b, 654, z
        # -> "normal" construction (*all* arg are now pre-initialized)
        print a + b

    # wpc allows for the use of arbitrarily complex interleaved instructions
    def construct
        wpc
            u := 7 + 5 
            for n in "ab"
                init_of $n, 654, u
            my_c_init()

    def my_c_init
        init_of c, 564 # (this instruction forces the inlining of my_c_init)

    a: Foo
    b: Bar
    c: Baz
```

## Getsetters

*Symbolic attributes* can be added using `get_...`, `set_...`, `mod_...` and `typeof_...` method names.

<!-- Getters and setters globally enable a wide reduction of parenthesis bloating. More importantly, it helps to write more polymorphic, encapsulated and generic code. -->

```python
# an example with getters and setters
class Complex[ T? FP32 ]
    def get_mag
        sqrt real * real + imag * imag

    def get_arg
        atan2 imag, real

    def set_mag new_mag
        old_arg := arg
        real = new_mag * cos old_arg
        imag = new_mag * sin old_arg
        
    real: T
    imag: T

c := Complex 1, 2
info c.mag # => 2.24
c.mag = 4  # => calls set_mod( 4 )
```

`typeof_...` allows to get type information without the need for a getter (can be useful for pure `set`ters or `mod`ers).

```python
class Smurf
    def typeof_break_dance
        SI32

    def set_break_dance value
        info value

def foo v: SI32
    v = 654

s := Smurf()
foo s.break_dance # will work (typeof is known without a getter)
```

`mod_...` can be seen as a generalized `set`ter. It is called with a modifying function as parameter (instead of a value).

```python
# an example with getters and setters
class NodeList
    def mod_x func
        for p in points
            func p.x
    points: Vec[ Point ]

node_list.x += 10 # 'operator+=' is a register "mod function"
```

# Types for literals

Literals (`12`, `"smurf"`, `[1,2]`, ...) must be of given types, but it's impossible to find types that would directly fit all the needs (one day you need fast coding, on another day you will have to focus on execution speed, security, etc...).

Sane tries then to provide types to fit the most common needs, with maximum flexibility and able to execute the most common operations at good *mean* speeds and memory occupancy, while **fully ensuring that the conversion costs will be zero**.

```python
# generates a mutable 'String'
s := "les { 2 * 4 } scarole"

# supporting common operations
# (with potential memory allocation)
s += "s"

# operations can nevertheless be handled by the compiler,
# enabling to remove intermediate memory allocation
# -> t will be stored in the .text or .data section
# (with removed )
t := static_string s
```

For maximum flexibility, arrays and maps are fully generic

```python
# heterogeneous arrays (no forced conversion)
for a in [ "f", 1 ]; print a + 1 # -> f1 2 
# costlesss conversions (using a std lib function)
for a in vec [ "f", 1 ]; print a + 1 # -> f1 11 
# choice of the target type
for a in Vec[ String ] [ "f", 1 ]; print a + 1 # -> f1 11
```

```python
# works the same way for maps
import HashTable

v := 17
a := { 1: 2, "f": "g", v } # <=> "v": v for the last item
# can be converted with compile time support
info HashTable[ String, String ] a # -> { "1": "2", "f": "g", "v": "17" }```
```

# Memory

## Rvalues

`rvalue` allows to know if a value is *owned* on a given scope.

```python
def foo a, b
    info rvalue a # -> true (s + 2 has been created for the call and will be destroyed just after)
    info rvalue b # -> false (b is a reference on a variable referenced elsewhere)

s := "..."
foo s + 2, s
```

The `move` function enables to force the transfer of the ressources. `forward x` is equivalent to `move x` if `x` is an rvalue. In the other case, it returns the reference on `x`.

<!-- Besides, Sane automates the "last usage tracking" of mutable variables (using the Abstract Syntax Tree). Variables  -->

```python
def foo v
    info rvalue v
    w := Vec[ SI32 ] forward v # content of v will be transfered (not copied !) to w

v := vec [ 1, 2, 3 ]
foo v      # rvalue: false. Content of v will be copied (with a potential mem alloc)
foo move v # rvalue: true.  No copy, no mem alloc
```

## Heap and stack

Variables can be created on the heap, with the (heap or mixed) allocator of your choice. `new` and `delete` are regular standard functions (not operators) that use the standard allocator (which is by default a kind of slub allocator, with pros and cons... but... you have the choice :) ).

If not specified, variables are of course created on *a* stack. By default, variables are created on the current stack. Nevertheless, as an optimization, if they are intended to be returned, they can be created directly on a caller stack. Beside, they can be created on intermediate or partial in presence of a variant of coroutine.

```python
def foo
    vec 0 .. 1e6
a := foo() # the vector is directly is thus stack (not the stack of f)
```

Besides, syntax for pointers and references are not exactly the same than in C++, for faster reading (and conformance with the base syntax).

```python
# new is a regular function (explaining the ',' after the type)
p := new String, "123" # -> type = Ptr[ String ]
# '@' allows for getting the references
info @p
# '->' works as usual
info p->size
# '\' allows for getting a pointer
a := "456"
q := \a # -> a Ptr[ String ] pointing in a
```

# Generative programming

Metaprogramming is at the heart of the facilities offered to ease and speed up the development processes, while ensuring the best performances.

## Compile-time execution

The Sane compiler is able to execute any Sane code during the compilation. It will do it

* if explicitely required (`kv` function),
* if mandatory (notably for *generative programming* or e.g. for template parameters),
* or if trivial (trivial simplifications that do not penalize the compilation time)

Of course, CT execution will not occur on data intended to be handled at run-time (e.g. files, unless explicitly stated -- notably it content is known at compile time).

```python
# a good part of the CT simplifications are automatic 
a := [ 1, 2, 3 ].size
if a != 3
    never_compiled() # because size is trivially known at CT

# they can also be triggered explicitly (`kv` function)
b := 0
for i in 0 .. 10
    b += i
if kv b != 45 # kv( b ) is known to be 45 at CT
    never_compiled() # 

# it can work with files if explicitly stated
c := load( "fs" ).read_file_sync "my_file", kv: true
s := c.split().filter( x => x.size == 3 ).size
info kv s # computed entirely at CT

# triggering can be implicitly 
i := MyClass[ s ] # we store the symbolic repr
j := MyClass[ 17 ] # but here, the equality operator forces the computation
```

<!-- Generative Programming involves generally programs that generate programs. In lot of cases, the point is to ensure that some computations are done before runtime, notably for performance, syntax, factorizations, interoperability...

When programs for code generation are separate from the program to be finally executed, one could use the term external code generation. External code generation is an obvious and very flexible, but generally leads to uncomfortable maintainability issues (problems with tracing, with clarity, with builders generally not designed for that, ...).

With the handling of compile time computations, Sane provides tools for embedded code generation. In enables the largest scope of possibilities while leading code far clearer and easier to maintain. -->

## Computed names

All variable names (whether it's for look up or for creation) may be computed (using `$`s).

```python
# creation of a variable named 'i18n'
i${ 2 * 9 }n := 43
```

Besides, a `\[+]` in a variable declaration enable to declare it in a parent scope (`\` for the first parent, `\\` for the second one, ...), even in a class block (notably to define methods and attributes, static or dynamic).

```python
# (extract of the class used for enums is the std libraries)
class Enum[ id, item_names ]
    # class blocks are executed only once (for a set of template parameters)
    static __nb_items := 0
    for name in item_names 
        num := post_inc __nb_items
        static # (works also with non-static defs and attrs)
        def \get_$name
            Enum[ id, name, item_names ] __value: num

    def write_to_stream os 
        os << item_names[ __value ]

    __value ~= SI32
      
T := Enum[ 0, [ "A", "B" ] ]
info T::A # => A (thanks to the generated `static def get_A` in the class scope)
```

## CT Symbolic computations

Sane give access to SSA graphs either for reading, modification or creation.

Thus we can for instance get a symbolic representation of what a function does, as we can generate code from the construction of a SSA graph.

```python
# extract of a function to differentiate at a graph level
def diff node: CtGraph
    switch node
        Symbolic::mul => node.children[ 0 ] * diff node.children[ 1 ] +
                         node.children[ 1 ] * diff node.children[ 0 ] 
        ... 

# a simple function
def foo n, x
    res := typeof( x ) 0
    for i in 1 .. n
        res += pow x, i
    res

# graph representation + differentiation of foo
x := ct_symbol FP64
n := ct_symbol SI32
sym_diff := diff ct_graph( foo( x, n ) ), x

# compile time optimized differentiation of foo
foo_diff := sym_diff.subs [ x, n ], [ xv, nv ]
```

These symbolic representations can be seen as the level above the AST representation (also accessible via std introspection as shown later). They enable what one can call "code instrospection and filtering".

It is for instance used in the module `vectorize` (providing ways to simplify the writing of loops with SIMD instructions). In all the cases, transformations are triggered by standard calls and are a library concern (meaning that you have the choice to select them, to modify or create new ones for full control and extensibility).

## Ct_eval

For the extreme cases, it is possible to pass an arbitrary string to the compiler (as long of course as it can be CT computed).

```python
a := 5
ct_eval "def foo; ${ 1 + a }"
info foo() # => 6
```

> This construction is probably the closest to the one we can find in "code generation", with a reduction of problems coming from information scattering and tangled build configuration. Nevertheless, computed names with CT branching usually offer superior clarity and debugging support.

## Ab initio primitives

In the same vein, it is possible to handle structures instead of text (as e.g. handling a DOM vs the html text).

Almost every objects handled by the compiler is actually accessible and modifiable within the language. It's valid for types, scopes, definition of methods, classes, and so on.

Compilation objects can thus be created *ab initio*, to be consecutively handled by the compiler.

```python
# creation
T := Type()
T.name = "MyType"
T.attributes << Type::Attribute off:0, name: "a", type: SI32

# the created type is stored and handled exactly like the others
i := T a: 100
info i # => T( a: 100 )
```


# Active libraries

## Selection

In Sane, functions implementations can be selected according to their a posteriori usage.

When functions (or methods) that are qualified switchable are called, Sane marks theirs outputs in the graph (including captured variables). Before code emission, these marks are sent to the registered "switch procedures", allowing to change the actual implementations.

There possibilities are used in a lot of essential optimisations. For instance, the concatenation operator works only with two variables (because it is an operator). If we want to concatenate more than two variables using this operator, we may end up with a lot of intermediate allocations and copies. A posteriori selection enable to gather the concatenations to avoid the waste.

```python
# a standard matrix class (with a `switchable` procedure)
class Matrix
    switchable
    def operator* that
        # -> normal multiplication procedure
    ...

# standard multiplications (left to right)
a := p * q * r * v

# addition of a "swither" (a different implementation, providing the same result)
# to call a procedure able to find the best ways to do the multiplications
globals.switchers.add
    oper: Matrix::operator*
    func: inps, outs =>
        # (simplified code)
        return [ nary_multiplier ... ]
```




# Examples of active libraries

## Auto-tuning

Example

```python
def my_test
    exec_time f 0 .. 10000 # representative parameter(s)
def f( range )
    simd_size := TuningParameter( my_test, [2,4,8] )
    # -> simd_size.val is fixed and known at compile time
    for v in simd_loop( range, simd_size.val )
        ...
```
        
## String storage

String storage and interpolation give a good example on transformations that take place during the compilation that helps develop truly convenient solution while ensuring to get the best performances.

STORAGE

Strings created using double quotes are by default of type String (which is a clone of the std::string defined in clang). It is very convenient for a vast majority of applications, but for large sizes, it may involve dynamic memory allocation. For some performance critical or embeded applications it is simply a no go.

Incidentally, allocation is understood by the compiler. If the there's no real need for this dynamic memory because it is finally copied elsewhere, the allocation and desallocation won't take place at all.

```python
# there's a *symbolic* allocation, actually not executed at all
# because only the content is used (to get a `static const char *`)
p := StaticCStr "string with length > 23 chars..."

# and now, we have a pointer in .data, on a zero-ended string
strlen p + 2 # returns length - 2
```

INTERPOLATION

String interpolations ("...${...}...") may also involve memory allocation which may be abad thing e.g. if not done with the right allocator... But it actually occurs only if really needed, depending on the target of the string.

In Sane, "...${...}..." creates a symbolic graph

```python
# extract of the Enum class definition
class Enum[ id, name, item_names ]
    static __nb_items := 0
    if __nb_items == 0
        for item_name in item_names 
            num := post_inc __nb_items
            static
            def \\get_$item_name
                Enum[ id, name, item_names ] __value: num

    __value ~= SI32
```
      


# Asynchronous code

async/await are cool but intrusive and procedural, whereas data driven would be more relevant.

Besides, a runtime can't be defined by a language, it has to be a library: needs are the same if you're on the embeded world, if you're developping a game, and so on...

