# What is Sane ?

Sane is a general purpose programming language designed for **programming efficiency** and **execution performance**.

The goal is to steal all the things that make C++ such a great language (memory model, templates, ...), with a **streamlined syntax** and **much wider meta-programming abilities**, leading to more **control** and **practicality**.

Sane is the acronym of *Software engineers Are Not Evil*. 

<!-- Zero-cost enforcements and abstractions are always present, but when it comes to compromises, new ideas or specific needs, we believe that the choice must come from teams  -->

<!-- enabling much better **control** (notably via **active libraries** for *powerful, transparent and controllable* extensions) and  (streamlined syntax, assumed meta and generative programming, deep introspection, etc...).

We believe that we are able to decide what kind of enforcements and abstractions we need, depending on the context. Zero-cost security enforcements are always 
 Things like enforced security, automated handling of asynchronicity, auto-vectorization or  can be absolutely awesome and strongly differentiating, but it depends of the kind of program, the stage (prototype, ...), the stakes, ... Thus, these kind of features are not in the core language but are in libraries 

-->

<!-- libraries. -->

# Base syntax

## Simplifications

To promote fast reading and writing, `()` for calls, functions definitions, ifs, ... become optional. Semicolon, braces and returns are under the same treatment.

```python
# 'a' and 'b' are (unconstrained) parameters of the function 'foo'
def foo a, b
    info a + b

# <=> foo( 5, min( 10, 15 ) )
foo 5, min 10, 15
```

## Line continuation

Line continuations are automatically handled, either because of operators or block structures.

```python
# b is the rhs of the '+'
foo 10, a +
    b

# argument list can be specified in block lines
# (',' in this case is not needed)
foo
    10
    a + b
```

If you're a frantic one-liner machine, you're still welcome to play the game :)

```python
# ';' acts as a carriage return
def foo a, b; info a + b

# '()' allows for block definitions
def bar a, b; ( c := a + b; 2 * c )
```

## Simplifications

As a rule of thumb, *obvious* simplifications are enabled whenever possible.

```python
# An example with lambda functions:
l0 := () => 17 # std def of a lambda func without parameter
l1 := => 17 # ':=' can't be an argument spec => we have an
            # eqivalent def

# works of course with all the kinds of operators
foo
    => 17 # unnamed arg
    arg: => 17 # named arg
```

## Parameters

Parameters of callable objects (`def`s, `classes`s, `lambda`s, ...) can have default values, constraints and can be selected using their names.

```python
# a must be a SI32 (32 bits signed integer)
def foo a: SI32, b? a + 1, c? b + 1
    a + b + c

# argument may be named. Default values are computed on the go
foo 15, c: 18
```

By default, arguments is passed as immutable references. The `mut` keyword enables to "deconstify" them.

```python
def foo a, mut b
    b += a # OK
    # a += 1 would generate a compilation error

b := 10
foo 15, b
info b # => 25
```

## Qualifiers

`private`, `protected`, `static` or any other qualifier of the same kind may be associated with one or several declarations at the same time.

> It enables a clearer separation between internal and public attributes, static and dynamic variables, and so on…

```python
class Mesh[ ElemTypes ]
    def area
        elems.sum e => e.with_coords( e.nodes.map n => nodes[ n ] ).area()

    # private section is visually separated by a different indentation
    private
        nodes: Vec[ Node ]
        elems: HetVec[ ...ElemTypes ] # HetVec = Heterogeneous Vector
```


# Templates

Every `def`, `class` and `lambda` definition are actually templates. For instance, `def` actually defines an object able to *generate at Compile-Time* (CT) a function or a method.

It behaves basically as C++ templates, excepted the syntax (simplifications, handling of traits, ...) and the very important fact that the compiler is actually a server enabling **per function caching and hashing**, to get far better binary sizes and compilation speeds.

## Surdefinitions

An arbitrary number or surdefinitions may lie on the same scope. Selection may depend on compile-time or run-time programmable priorities (`pertinence`) and programmable conditions (`when`).

```python
# computed priority (to choose )
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

Besides, argument constraint may act on run-time type information (leading to construction of multi-dimensional vtables if necessary, with of course optimizations if the types are actually known at compile-time).

> There are different kinds of vtables in Sane. Adding a pointer at the beginning is the default solution (very good on a general purpose) but several others exist, to adapt to the contexts.

```python
# run-time selection using bidimensionnal vtables
def intersection_area virtual a: Square, virtual b: Triangle
    ...

def weight a: GeometricObject, b: GeometricObject, density? 7800
    density * intersection_area a, b
```

## Classes

**Classes parameters can be of any kind**, as long as copy and equality can be CT handled (in a symbolic way or not).

Template parameters can then be of any type (strings, array, ...), even if it involves memory allocation, files or any kinds of symbolic content allowed to be CT handled.

```python
# `sizes` can be defined using any kind of array
class PoolBySize[ sizes: ArrayLike, mt? true, name? "anon" ]
    class Item
        free_ptr: NullableAT # nullable address
        page_vec: Vec[ AT ]  # contiguous array of addresses
    # StaticMap constructs a tree at compile time
    map: StaticMap[ sizes, Item ] 
    ...

# instantiation for a given set of sizes (handled at CT)
pool := PoolBySize[ [ 16, 24, 32 ] ]()
```

## Variadic arguments

Parameters in callables (`def`, `class`, ...) can be defined as variadic (`...var_name`), with optional constraints. It construct a CT known `Varargs` object, containing argument references with the optional names.

```python
def foo a, ...b: SI32, c: StringLike
    info a, b, c # -> 4, [5,arg_name:6], "bar"
    info b[ 0 ]          # -> 5
    info b[ "arg_name" ] # -> 6
    info b.values        # -> [5, 6]
    info b.names         # -> [arg_name]

foo 4, 5, arg_name: 6, "bar"
```


## Spread operator

The `...` operator can be used expand lists, notably for calls (as long as sizes can be determined at CT) and list definitions. It works with every kinds of lists with a spread method.

```python
# ex. 1: function call with a Varargs 
def foo a, ...b
    bar ...b, 654

# ex. 2: spread works also with lists, maps, ...
u := 2 .. 10 # (range operator)
v := [ 1, ...u, 10 ]
```


# Objects

## Constructors

As with C++, class attributes can be directly initialized only once during the construction arguments.

Pre-construction can be written within a `wpc` (*With Pre Construction*) block, with calls to init_of:

* `init_of attr_name, [ ctor_args ]` enables to initialize an argument
* `init_of self, [ ctor_args ]` enables to call another constructor of the same class.

If there is no `wpc` instruction, the pre-construction is automatically determined using the last *visible* `init_of` call of the block.

```python
class MyClass
    def construct
        # explicit pre-construction block
        wpc
            init_of a, 564 
            init_of b, 654, 12
        # -> "normal" construction (*all* arg are pre-initialized)
        print v

    # equivalent ctor with automatic wpc block
    def construct
        # implicit pre-construction block
        init_of a, 564 
        init_of b, 654, 12
        # -> "normal" construction (after the last init_of)
        print v

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

> Getters and setters globally enable a wide reduction of parenthesis bloating. More importantly, it helps writing more polymorphic and generic code, easing evolution and encapsulation.

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

`typeof_...` enables to get type information without the need for a getter.

`mod_...` can be seen as a generalized setter, and takes as parameter a function, instead of a value.

```python
# an example with getters and setters
class NodeList
    def mod_x func
        for p in points
            func p.x
    points: Vec[ Point ]

node_list.x += 10 # 'operator+=' is a register "mod function"
```

## Strings, arrays and maps

Strings, arrays and maps follow the same construction principles:

* they are designed for maximum flexibility + ease of use for the most common cases
* while enabling zero-cost conversions (relevant types may be totally different, depending on the context)

```python
# generic case for a string (a mutable 'String')
s := "les { 2 * 4 } scarole"
s += "s" # "les 8 scaroles"
# but the operations can be handled by the compiler,
# totally removing intermediate memory allocation
t := static_string s # -> in the .text or .data section
```

Arrays and maps are fully generic

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

It is possible to know if a value is shared, or owned by a given scope (`rvalue`). It notably enable to know if the resources can be transfered to avoid unnecessary copies

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

<!-- Paris 13eme bnf -> Catsanevas, poste grands instruments, telescopes et satellites, grands volumes, 1 ou plusieurs projets. Dataprocessing center, veille techno. -->
<!-- Gif, saclay DR4, RER B -> -->
<!-- 2 sortes de postes, Noémie, FSEP (arbitrage au niveau des instituts, plus tard) -->

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

# Generative pprogramming

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

<!-- Generative Programming involves generally programs that generate programs. In lot of cases, the point is to ensure that some computations are done before runtime, notably for performance, syntax, factorizations, interoperability…

When programs for code generation are separate from the program to be finally executed, one could use the term external code generation. External code generation is an obvious and very flexible, but generally leads to uncomfortable maintainability issues (problems with tracing, with clarity, with builders generally not designed for that, …).

With the handling of compile time computations, Sane provides tools for embedded code generation. In enables the largest scope of possibilities while leading code far clearer and easier to maintain. -->

## Computed names

All variable names (whether it’s for look up or for creation) may be computed (using `$`s).

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

## ct_eval

For the extreme cases, it is possible to pass an arbitrary string to the compiler (as long of course as it can be CT computed).

```python
a := 5
ct_eval "def foo; ${ 1 + a }"
info foo() # => 6
```

> This construction is probably the closest to the one we can find in "code generation", with a reduction of problems coming from information scattering and tangled build configuration. Nevertheless, computed names with CT branching usually offer superior clarity and debugging support.

## Ab initio primitives

In the same vein, it is possible to handle structures instead of text (as e.g. handling a DOM vs the html text).

Almost every objects handled by the compiler is actually accessible and modifiable within the language. It’s valid for types, scopes, definition of methods, classes, and so on.

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

When functions (or methods) that are qualified switchable are called, Sane marks theirs outputs in the graph (including captured variables). Before code emission, these marks are sent to the registered “switch procedures”, allowing to change the actual implementations.

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

Incidentally, allocation is understood by the compiler. If the there’s no real need for this dynamic memory because it is finally copied elsewhere, the allocation and desallocation won’t take place at all.

```python
# there's a *symbolic* allocation, actually not executed at all
# because only the content is used (to get a `static const char *`)
p := StaticCStr "string with length > 23 chars..."

# and now, we have a pointer in .data, on a zero-ended string
strlen p + 2 # returns length - 2
INTERPOLATION

String interpolations ("...${...}...") may also involve memory allocation which may be abad thing e.g. if not done with the right allocator… But it actually occurs only if really needed, depending on the target of the string.

In Sane, "...${...}..." creates a symbolic graph

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
```python
      


# Asynchronous code

async/await are cool but intrusive and procedural, whereas data driven would be more relevant.

Besides, a runtime can’t be defined by a language, it has to be a library: needs are the same if you’re on the embeded world, if you’re developping a game, and so on...

