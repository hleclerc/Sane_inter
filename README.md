# What is Sane ?

Sane is a general purpose programming language designed for **programming efficiency** and **execution performance**.

The goal is to steal all the things that make C++ such a great language (memory model, templates, ...), with a **streamlined syntax** and **much wider meta-programming abilities**, leading to more **control** and **practicality**.

Sane is the acronym of *Software engineers Are Not Evil*. 

This documentation starts with the base syntax, and then goes into the advanced concepts (generative programming, active libraries, handling of asynchronous execution, ...)

<!-- TOC -->

- [What is Sane ?](#what-is-sane-)
- [Base syntax](#base-syntax)
    - [Simplifications](#simplifications)
    - [Line continuation](#line-continuation)
    - [Simplifications](#simplifications-1)
    - [Parameters](#parameters)
    - [Qualifiers](#qualifiers)
- [Templates](#templates)
    - [Surdefinitions](#surdefinitions)
    - [Classes](#classes)
    - [Variadic arguments](#variadic-arguments)
    - [Spread operator](#spread-operator)
- [Objects](#objects)
    - [Constructors](#constructors)
    - [Getsetters](#getsetters)
    - [Strings, arrays and maps](#strings-arrays-and-maps)
- [Memory](#memory)
    - [Rvalues](#rvalues)
    - [Heap and stack](#heap-and-stack)
- [Generative pprogramming](#generative-pprogramming)
    - [Compile-time execution](#compile-time-execution)
    - [Computed names](#computed-names)
    - [CT Symbolic computations](#ct-symbolic-computations)
    - [ct_eval](#ct_eval)
    - [Ab initio primitives](#ab-initio-primitives)
- [Active libraries](#active-libraries)
    - [Selection](#selection)
- [Examples of active libraries](#examples-of-active-libraries)
    - [Auto-tuning](#auto-tuning)
    - [String storage](#string-storage)
- [Asynchronous code](#asynchronous-code)

<!-- /TOC -->

<!-- Zero-cost enforcements and abstractions are always present, but when it comes to compromises, new ideas or specific needs, we believe that the choice must come from teams  -->

<!-- enabling much better **control** (notably via **active libraries** for *powerful, transparent and controllable* extensions) and  (streamlined syntax, assumed meta and generative programming, deep introspection, etc...).

We believe that we are able to decide what kind of enforcements and abstractions we need, depending on the context. Zero-cost security enforcements are always 
 Things like enforced security, automated handling of asynchronicity, auto-vectorization or  can be absolutely awesome and strongly differentiating, but it depends of the kind of program, the stage (prototype, ...), the stakes, ... Thus, these kind of features are not in the core language but are in libraries 

-->

<!-- libraries. -->

# Base syntax

## Simplifications

To promote fast reading and writing, `()` for calls, functions definitions, ifs, ... become optional. Semicolon, braces and returns are under the same treatment.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># &#39;a&#39; and &#39;b&#39; are (unconstrained) parameters of the function &#39;foo&#39;</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>
    <span style="color: #000000">info</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">b</span>

<span style="color: #938d7f; font-style: italic"># &lt;=&gt; foo( 5, min( 10, 15 ) )</span>
<span style="color: #000000">foo</span> <span style="color: #93488f">5</span>, <span style="color: #000000">min</span> <span style="color: #93488f">10</span>, <span style="color: #93488f">15</span>
</pre></div>


## Line continuation

Line continuations are automatically handled, either because of operators or block structures.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># b is the rhs of the &#39;+&#39;</span>
<span style="color: #000000">foo</span> <span style="color: #93488f">10</span>, <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span>
    <span style="color: #000000">b</span>

<span style="color: #938d7f; font-style: italic"># argument list can be specified in block lines</span>
<span style="color: #938d7f; font-style: italic"># (&#39;,&#39; in this case is not needed)</span>
<span style="color: #000000">foo</span>
    <span style="color: #93488f">10</span>
    <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">b</span>
</pre></div>


If on a contrary you're a frantic one-liner machine, you're still welcome to play the game :)

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># &#39;;&#39; acts as a carriage return</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>; <span style="color: #000000">info</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">b</span>

<span style="color: #938d7f; font-style: italic"># &#39;()&#39; allows for block definitions</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">bar</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>; ( <span style="color: #000000">c</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">b</span>; <span style="color: #93488f">2</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">c</span> )
</pre></div>


## Simplifications

As a rule of thumb, *obvious* simplifications are enabled whenever possible.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># An example with lambda functions:</span>
<span style="color: #000000">l0</span> <span style="color: #6d3642; font-weight: bold">:=</span> () <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #93488f">17</span> <span style="color: #938d7f; font-style: italic"># std def of a lambda func without parameter</span>
<span style="color: #000000">l1</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #93488f">17</span> <span style="color: #938d7f; font-style: italic"># &#39;:=&#39; can&#39;t be an argument spec =&gt; we have an</span>
            <span style="color: #938d7f; font-style: italic"># eqivalent def</span>

<span style="color: #938d7f; font-style: italic"># works of course with all the kinds of operators</span>
<span style="color: #000000">foo</span>
    <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #93488f">17</span> <span style="color: #938d7f; font-style: italic"># unnamed arg</span>
    <span style="color: #000000">arg</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #93488f">17</span> <span style="color: #938d7f; font-style: italic"># named arg</span>
</pre></div>


## Parameters

Parameters of callable objects (`def`s, `classes`s, `lambda`s, ...) can have default values, constraints and can be selected using their names.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># a must be a SI32 (32 bits signed integer)</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">SI32</span>, <span style="color: #000000">b</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">1</span>, <span style="color: #000000">c</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">1</span>
    <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">c</span>

<span style="color: #938d7f; font-style: italic"># argument may be named. Default values are computed on the go</span>
<span style="color: #000000">foo</span> <span style="color: #93488f">15</span>, <span style="color: #000000">c</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #93488f">18</span>
</pre></div>


By default, arguments is passed as immutable references. The `mut` keyword enables to "deconstify" them.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, <span style="color: #000000">mut</span> <span style="color: #000000">b</span>
    <span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">+=</span> <span style="color: #000000">a</span> <span style="color: #938d7f; font-style: italic"># OK</span>
    <span style="color: #938d7f; font-style: italic"># a += 1 would generate a compilation error</span>

<span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">10</span>
<span style="color: #000000">foo</span> <span style="color: #93488f">15</span>, <span style="color: #000000">b</span>
<span style="color: #000000">info</span> <span style="color: #000000">b</span> <span style="color: #938d7f; font-style: italic"># =&gt; 25</span>
</pre></div>


## Qualifiers

`private`, `protected`, `static` or any other qualifier of the same kind may be associated with one or several declarations at the same time.

> It enables a clearer separation between internal and public attributes, static and dynamic variables, and so on…

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">class</span> <span style="color: #000000">Mesh</span>[ <span style="color: #000000">ElemTypes</span> ]
    <span style="color: #00aaff">def</span> <span style="color: #000000">area</span>
        <span style="color: #000000">elems</span>.<span style="color: #000000">sum</span> <span style="color: #000000">e</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #000000">e</span>.<span style="color: #000000">with_coords</span>( <span style="color: #000000">e</span>.<span style="color: #000000">nodes</span>.<span style="color: #000000">map</span> <span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #000000">nodes</span>[ <span style="color: #000000">n</span> ] ).<span style="color: #000000">area</span>()

    <span style="color: #938d7f; font-style: italic"># private section is visually separated by a different indentation</span>
    <span style="color: #005782">private</span>
        <span style="color: #000000">nodes</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Vec</span>[ <span style="color: #000000">Node</span> ]
        <span style="color: #000000">elems</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">HetVec</span>[ ...<span style="color: #000000">ElemTypes</span> ] <span style="color: #938d7f; font-style: italic"># HetVec = Heterogeneous Vector</span>
</pre></div>



# Templates

Every `def`, `class` and `lambda` definition are actually templates. For instance, `def` actually defines an object able to *generate at Compile-Time* (CT) a function or a method.

It behaves basically as C++ templates, excepted the syntax (simplifications, handling of traits, ...) and the very important fact that the compiler is actually a server enabling **per function caching and hashing**, to get far better binary sizes and compilation speeds.

## Surdefinitions

An arbitrary number or surdefinitions may lie on the same scope. Selection may depend on compile-time or run-time programmable priorities (`pertinence`) and programmable conditions (`when`).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># computed priority (to choose )</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">pow</span> <span style="color: #000000">mat</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Matrix</span>, <span style="color: #000000">n</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">PositiveInteger</span> 
            <span style="color: #005782">pertinence</span> <span style="color: #6d3642; font-weight: bold">-</span> <span style="color: #000000">cost_mult</span>( <span style="color: #000000">mat</span>, <span style="color: #000000">mat</span> ) <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">log</span> <span style="color: #000000">n</span>
    <span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">&amp;</span> <span style="color: #93488f">1</span> <span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #000000">mat</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">pow</span>( <span style="color: #000000">mat</span>, <span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">-</span> <span style="color: #93488f">1</span> )
          <span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">pow</span>( <span style="color: #000000">mat</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">mat</span>, <span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">/</span> <span style="color: #93488f">2</span> )

<span style="color: #938d7f; font-style: italic"># + computed condition (handled at compile time in this case)</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">pow</span> <span style="color: #000000">mat</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Matrix</span>, <span style="color: #000000">n</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Number</span>
            <span style="color: #005782">when</span> <span style="color: #000000">mat</span>.<span style="color: #000000">sub_types</span>.<span style="color: #000000">some</span>( <span style="color: #000000">T</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #000000">T</span>.<span style="color: #000000">is_approximate</span> )
            <span style="color: #005782">pertinence</span> <span style="color: #6d3642; font-weight: bold">-</span> <span style="color: #000000">cost_eigen</span>( <span style="color: #000000">mat</span> )
    <span style="color: #000000">e</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">eigen</span> <span style="color: #000000">mat</span>
    <span style="color: #000000">e</span>.<span style="color: #000000">P</span>.<span style="color: #000000">trans</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">pow</span>( <span style="color: #000000">e</span>.<span style="color: #000000">D</span>, <span style="color: #000000">n</span> ) <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">e</span>.<span style="color: #000000">P</span>
</pre></div>


Besides, argument constraint may act on run-time type information (leading to construction of multi-dimensional vtables if necessary, with of course optimizations if the types are actually known at compile-time).

> There are different kinds of vtables in Sane. Adding a pointer at the beginning is the default solution (very good on a general purpose) but several others exist, to adapt to the contexts.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># run-time selection using bidimensionnal vtables</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">intersection_area</span> <span style="color: #005782">virtual</span> <span style="color: #000000">a</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Square</span>, <span style="color: #005782">virtual</span> <span style="color: #000000">b</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Triangle</span>
    ...

<span style="color: #00aaff">def</span> <span style="color: #000000">weight</span> <span style="color: #000000">a</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">GeometricObject</span>, <span style="color: #000000">b</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">GeometricObject</span>, <span style="color: #000000">density</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #93488f">7800</span>
    <span style="color: #000000">density</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">intersection_area</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>
</pre></div>


## Classes

**Classes parameters can be of any kind**, as long as copy and equality can be CT handled (in a symbolic way or not).

Template parameters can then be of any type (strings, array, ...), even if it involves memory allocation, files or any kinds of symbolic content allowed to be CT handled.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># `sizes` can be defined using any kind of array</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">PoolBySize</span>[ <span style="color: #000000">sizes</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">ArrayLike</span>, <span style="color: #000000">mt</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #005782">true</span>, <span style="color: #000000">name</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #118811">&quot;anon&quot;</span> ]
    <span style="color: #00aaff">class</span> <span style="color: #000000">Item</span>
        <span style="color: #000000">free_ptr</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">NullableAT</span> <span style="color: #938d7f; font-style: italic"># nullable address</span>
        <span style="color: #000000">page_vec</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Vec</span>[ <span style="color: #000000">AT</span> ]  <span style="color: #938d7f; font-style: italic"># contiguous array of addresses</span>
    <span style="color: #938d7f; font-style: italic"># StaticMap constructs a tree at compile time</span>
    <span style="color: #000000">map</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">StaticMap</span>[ <span style="color: #000000">sizes</span>, <span style="color: #000000">Item</span> ] 
    ...

<span style="color: #938d7f; font-style: italic"># instantiation for a given set of sizes (handled at CT)</span>
<span style="color: #000000">pool</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">PoolBySize</span>[ [ <span style="color: #93488f">16</span>, <span style="color: #93488f">24</span>, <span style="color: #93488f">32</span> ] ]()
</pre></div>


## Variadic arguments

Parameters in callables (`def`, `class`, ...) can be defined as variadic (`...var_name`), with optional constraints. It construct a CT known `Varargs` object, containing argument references with the optional names.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, ...<span style="color: #000000">b</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">SI32</span>, <span style="color: #000000">c</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">StringLike</span>
    <span style="color: #000000">info</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>, <span style="color: #000000">c</span> <span style="color: #938d7f; font-style: italic"># -&gt; 4, [5,arg_name:6], &quot;bar&quot;</span>
    <span style="color: #000000">info</span> <span style="color: #000000">b</span>[ <span style="color: #93488f">0</span> ]          <span style="color: #938d7f; font-style: italic"># -&gt; 5</span>
    <span style="color: #000000">info</span> <span style="color: #000000">b</span>[ <span style="color: #118811">&quot;arg_name&quot;</span> ] <span style="color: #938d7f; font-style: italic"># -&gt; 6</span>
    <span style="color: #000000">info</span> <span style="color: #000000">b</span>.<span style="color: #000000">values</span>        <span style="color: #938d7f; font-style: italic"># -&gt; [5, 6]</span>
    <span style="color: #000000">info</span> <span style="color: #000000">b</span>.<span style="color: #000000">names</span>         <span style="color: #938d7f; font-style: italic"># -&gt; [arg_name]</span>

<span style="color: #000000">foo</span> <span style="color: #93488f">4</span>, <span style="color: #93488f">5</span>, <span style="color: #000000">arg_name</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #93488f">6</span>, <span style="color: #118811">&quot;bar&quot;</span>
</pre></div>



## Spread operator

The `...` operator can be used expand lists, notably for calls (as long as sizes can be determined at CT) and list definitions. It works with every kinds of lists with a spread method.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># ex. 1: function call with a Varargs </span>
<span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, ...<span style="color: #000000">b</span>
    <span style="color: #000000">bar</span> ...<span style="color: #000000">b</span>, <span style="color: #93488f">654</span>

<span style="color: #938d7f; font-style: italic"># ex. 2: spread works also with lists, maps, ...</span>
<span style="color: #000000">u</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">2</span> .. <span style="color: #93488f">10</span> <span style="color: #938d7f; font-style: italic"># (range operator)</span>
<span style="color: #000000">v</span> <span style="color: #6d3642; font-weight: bold">:=</span> [ <span style="color: #93488f">1</span>, ...<span style="color: #000000">u</span>, <span style="color: #93488f">10</span> ]
</pre></div>



# Objects

## Constructors

As with C++, class attributes can be directly initialized only once during the construction arguments.

Pre-construction can be written within a `wpc` (*With Pre Construction*) block, with calls to init_of:

* `init_of attr_name, [ ctor_args ]` enables to initialize an argument
* `init_of self, [ ctor_args ]` enables to call another constructor of the same class.

If there is no `wpc` instruction, the pre-construction is automatically determined using the last *visible* `init_of` call of the block.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">class</span> <span style="color: #000000">MyClass</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">construct</span>
        <span style="color: #938d7f; font-style: italic"># explicit pre-construction block</span>
        <span style="color: #005782">wpc</span>
            <span style="color: #005782">init_of</span> <span style="color: #000000">a</span>, <span style="color: #93488f">564</span> 
            <span style="color: #005782">init_of</span> <span style="color: #000000">b</span>, <span style="color: #93488f">654</span>, <span style="color: #93488f">12</span>
        <span style="color: #938d7f; font-style: italic"># -&gt; &quot;normal&quot; construction (*all* arg are pre-initialized)</span>
        <span style="color: #000000">print</span> <span style="color: #000000">v</span>

    <span style="color: #938d7f; font-style: italic"># equivalent ctor with automatic wpc block</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">construct</span>
        <span style="color: #938d7f; font-style: italic"># implicit pre-construction block</span>
        <span style="color: #005782">init_of</span> <span style="color: #000000">a</span>, <span style="color: #93488f">564</span> 
        <span style="color: #005782">init_of</span> <span style="color: #000000">b</span>, <span style="color: #93488f">654</span>, <span style="color: #93488f">12</span>
        <span style="color: #938d7f; font-style: italic"># -&gt; &quot;normal&quot; construction (after the last init_of)</span>
        <span style="color: #000000">print</span> <span style="color: #000000">v</span>

    <span style="color: #938d7f; font-style: italic"># wpc allows for the use of arbitrarily complex interleaved instructions</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">construct</span>
        <span style="color: #005782">wpc</span>
            <span style="color: #000000">u</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">7</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">5</span> 
            <span style="color: #005782">for</span> <span style="color: #000000">n</span> <span style="color: #005782">in</span> <span style="color: #118811">&quot;ab&quot;</span>
                <span style="color: #005782">init_of</span> <span style="color: #000000">$n</span>, <span style="color: #93488f">654</span>, <span style="color: #000000">u</span>
            <span style="color: #000000">my_c_init</span>()

    <span style="color: #00aaff">def</span> <span style="color: #000000">my_c_init</span>
        <span style="color: #005782">init_of</span> <span style="color: #000000">c</span>, <span style="color: #93488f">564</span> <span style="color: #938d7f; font-style: italic"># (this instruction forces the inlining of my_c_init)</span>

    <span style="color: #000000">a</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Foo</span>
    <span style="color: #000000">b</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Bar</span>
    <span style="color: #000000">c</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Baz</span>
</pre></div>


## Getsetters

*Symbolic attributes* can be added using `get_...`, `set_...`, `mod_...` and `typeof_...` method names.

> Getters and setters globally enable a wide reduction of parenthesis bloating. More importantly, it helps writing more polymorphic and generic code, easing evolution and encapsulation.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># an example with getters and setters</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">Complex</span>[ <span style="color: #000000">T</span><span style="color: #6d3642; font-weight: bold">?</span> <span style="color: #000000">FP32</span> ]
    <span style="color: #00aaff">def</span> <span style="color: #000000">get_mag</span>
        <span style="color: #000000">sqrt</span> <span style="color: #000000">real</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">real</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">imag</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">imag</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">get_arg</span>
        <span style="color: #000000">atan2</span> <span style="color: #000000">imag</span>, <span style="color: #000000">real</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">set_mag</span> <span style="color: #000000">new_mag</span>
        <span style="color: #000000">old_arg</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">arg</span>
        <span style="color: #000000">real</span> <span style="color: #6d3642; font-weight: bold">=</span> <span style="color: #000000">new_mag</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">cos</span> <span style="color: #000000">old_arg</span>
        <span style="color: #000000">imag</span> <span style="color: #6d3642; font-weight: bold">=</span> <span style="color: #000000">new_mag</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">sin</span> <span style="color: #000000">old_arg</span>
    <span style="color: #000000">real</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">T</span>
    <span style="color: #000000">imag</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">T</span>

<span style="color: #000000">c</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">Complex</span> <span style="color: #93488f">1</span>, <span style="color: #93488f">2</span>
<span style="color: #000000">info</span> <span style="color: #000000">c</span>.<span style="color: #000000">mag</span> <span style="color: #938d7f; font-style: italic"># =&gt; 2.24</span>
<span style="color: #000000">c</span>.<span style="color: #000000">mag</span> <span style="color: #6d3642; font-weight: bold">=</span> <span style="color: #93488f">4</span>  <span style="color: #938d7f; font-style: italic"># =&gt; calls set_mod( 4 )</span>
</pre></div>


`typeof_...` enables to get type information without the need for a getter.

`mod_...` can be seen as a generalized setter, and takes as parameter a function, instead of a value.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># an example with getters and setters</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">NodeList</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">mod_x</span> <span style="color: #000000">func</span>
        <span style="color: #005782">for</span> <span style="color: #000000">p</span> <span style="color: #005782">in</span> <span style="color: #000000">points</span>
            <span style="color: #000000">func</span> <span style="color: #000000">p</span>.<span style="color: #000000">x</span>
    <span style="color: #000000">points</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Vec</span>[ <span style="color: #000000">Point</span> ]

<span style="color: #000000">node_list</span>.<span style="color: #000000">x</span> <span style="color: #6d3642; font-weight: bold">+=</span> <span style="color: #93488f">10</span> <span style="color: #938d7f; font-style: italic"># &#39;operator+=&#39; is a register &quot;mod function&quot;</span>
</pre></div>


## Strings, arrays and maps

Strings, arrays and maps follow the same construction principles:

* they are designed for maximum flexibility + ease of use for the most common cases
* while enabling zero-cost conversions (relevant types may be totally different, depending on the context)

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># generic case for a string (a mutable &#39;String&#39;)</span>
<span style="color: #000000">s</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #118811">&quot;les { 2 * 4 } scarole&quot;</span>
<span style="color: #000000">s</span> <span style="color: #6d3642; font-weight: bold">+=</span> <span style="color: #118811">&quot;s&quot;</span> <span style="color: #938d7f; font-style: italic"># &quot;les 8 scaroles&quot;</span>
<span style="color: #938d7f; font-style: italic"># but the operations can be handled by the compiler,</span>
<span style="color: #938d7f; font-style: italic"># totally removing intermediate memory allocation</span>
<span style="color: #000000">t</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">static_string</span> <span style="color: #000000">s</span> <span style="color: #938d7f; font-style: italic"># -&gt; in the .text or .data section</span>
</pre></div>


Arrays and maps are fully generic

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># heterogeneous arrays (no forced conversion)</span>
<span style="color: #005782">for</span> <span style="color: #000000">a</span> <span style="color: #005782">in</span> [ <span style="color: #118811">&quot;f&quot;</span>, <span style="color: #93488f">1</span> ]; <span style="color: #000000">print</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">1</span> <span style="color: #938d7f; font-style: italic"># -&gt; f1 2 </span>
<span style="color: #938d7f; font-style: italic"># costlesss conversions (using a std lib function)</span>
<span style="color: #005782">for</span> <span style="color: #000000">a</span> <span style="color: #005782">in</span> <span style="color: #000000">vec</span> [ <span style="color: #118811">&quot;f&quot;</span>, <span style="color: #93488f">1</span> ]; <span style="color: #000000">print</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">1</span> <span style="color: #938d7f; font-style: italic"># -&gt; f1 11 </span>
<span style="color: #938d7f; font-style: italic"># choice of the target type</span>
<span style="color: #005782">for</span> <span style="color: #000000">a</span> <span style="color: #005782">in</span> <span style="color: #000000">Vec</span>[ <span style="color: #000000">String</span> ] [ <span style="color: #118811">&quot;f&quot;</span>, <span style="color: #93488f">1</span> ]; <span style="color: #000000">print</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">1</span> <span style="color: #938d7f; font-style: italic"># -&gt; f1 11</span>
</pre></div>


<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># works the same way for maps</span>
<span style="color: #005782">import</span> <span style="color: #000000">HashTable</span>

<span style="color: #000000">v</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">17</span>
<span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> { <span style="color: #93488f">1</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #93488f">2</span>, <span style="color: #118811">&quot;f&quot;</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #118811">&quot;g&quot;</span>, <span style="color: #000000">v</span> } <span style="color: #938d7f; font-style: italic"># &lt;=&gt; &quot;v&quot;: v for the last item</span>
<span style="color: #938d7f; font-style: italic"># can be converted with compile time support</span>
<span style="color: #000000">info</span> <span style="color: #000000">HashTable</span>[ <span style="color: #000000">String</span>, <span style="color: #000000">String</span> ] <span style="color: #000000">a</span> <span style="color: #938d7f; font-style: italic"># -&gt; { &quot;1&quot;: &quot;2&quot;, &quot;f&quot;: &quot;g&quot;, &quot;v&quot;: &quot;17&quot; }</span>
</pre></div>

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">a</span>, <span style="color: #000000">b</span>
    <span style="color: #000000">info</span> <span style="color: #000000">rvalue</span> <span style="color: #000000">a</span> <span style="color: #938d7f; font-style: italic"># -&gt; true (s + 2 has been created for the call and will be destroyed just after)</span>
    <span style="color: #000000">info</span> <span style="color: #000000">rvalue</span> <span style="color: #000000">b</span> <span style="color: #938d7f; font-style: italic"># -&gt; false (b is a reference on a variable referenced elsewhere)</span>

<span style="color: #000000">s</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #118811">&quot;...&quot;</span>
<span style="color: #000000">foo</span> <span style="color: #000000">s</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">2</span>, <span style="color: #000000">s</span>
</pre></div>


The `move` function enables to force the transfer of the ressources. `forward x` is equivalent to `move x` if `x` is an rvalue. In the other case, it returns the reference on `x`.

<!-- Besides, Sane automates the "last usage tracking" of mutable variables (using the Abstract Syntax Tree). Variables  -->

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">v</span>
    <span style="color: #000000">info</span> <span style="color: #000000">rvalue</span> <span style="color: #000000">v</span>
    <span style="color: #000000">w</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">Vec</span>[ <span style="color: #000000">SI32</span> ] <span style="color: #000000">forward</span> <span style="color: #000000">v</span> <span style="color: #938d7f; font-style: italic"># content of v will be transfered (not copied !) to w</span>

<span style="color: #000000">v</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">vec</span> [ <span style="color: #93488f">1</span>, <span style="color: #93488f">2</span>, <span style="color: #93488f">3</span> ]
<span style="color: #000000">foo</span> <span style="color: #000000">v</span>      <span style="color: #938d7f; font-style: italic"># rvalue: false. Content of v will be copied (with a potential mem alloc)</span>
<span style="color: #000000">foo</span> <span style="color: #000000">move</span> <span style="color: #000000">v</span> <span style="color: #938d7f; font-style: italic"># rvalue: true.  No copy, no mem alloc</span>
</pre></div>


## Heap and stack

Variables can be created on the heap, with the (heap or mixed) allocator of your choice. `new` and `delete` are regular standard functions (not operators) that use the standard allocator (which is by default a kind of slub allocator, with pros and cons... but... you have the choice :) ).

If not specified, variables are of course created on *a* stack. By default, variables are created on the current stack. Nevertheless, as an optimization, if they are intended to be returned, they can be created directly on a caller stack. Beside, they can be created on intermediate or partial in presence of a variant of coroutine.

<!-- Paris 13eme bnf -> Catsanevas, poste grands instruments, telescopes et satellites, grands volumes, 1 ou plusieurs projets. Dataprocessing center, veille techno. -->
<!-- Gif, saclay DR4, RER B -> -->
<!-- 2 sortes de postes, Noémie, FSEP (arbitrage au niveau des instituts, plus tard) -->

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">foo</span>
    <span style="color: #000000">vec</span> <span style="color: #93488f">0</span> .. <span style="color: #93488f">1</span><span style="color: #000000">e6</span>
<span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">foo</span>() <span style="color: #938d7f; font-style: italic"># the vector is directly is thus stack (not the stack of f)</span>
</pre></div>


Besides, syntax for pointers and references are not exactly the same than in C++, for faster reading (and conformance with the base syntax).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># new is a regular function (explaining the &#39;,&#39; after the type)</span>
<span style="color: #000000">p</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">new</span> <span style="color: #000000">String</span>, <span style="color: #118811">&quot;123&quot;</span> <span style="color: #938d7f; font-style: italic"># -&gt; type = Ptr[ String ]</span>
<span style="color: #938d7f; font-style: italic"># &#39;@&#39; allows for getting the references</span>
<span style="color: #000000">info</span> @<span style="color: #000000">p</span>
<span style="color: #938d7f; font-style: italic"># &#39;-&gt;&#39; works as usual</span>
<span style="color: #000000">info</span> <span style="color: #000000">p</span><span style="color: #6d3642; font-weight: bold">-&gt;</span><span style="color: #000000">size</span>
<span style="color: #938d7f; font-style: italic"># &#39;\&#39; allows for getting a pointer</span>
<span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #118811">&quot;456&quot;</span>
<span style="color: #000000">q</span> <span style="color: #6d3642; font-weight: bold">:=</span> \<span style="color: #000000">a</span> <span style="color: #938d7f; font-style: italic"># -&gt; a Ptr[ String ] pointing in a</span>
</pre></div>


# Generative pprogramming

Metaprogramming is at the heart of the facilities offered to ease and speed up the development processes, while ensuring the best performances.

## Compile-time execution

The Sane compiler is able to execute any Sane code during the compilation. It will do it

* if explicitely required (`kv` function),
* if mandatory (notably for *generative programming* or e.g. for template parameters),
* or if trivial (trivial simplifications that do not penalize the compilation time)

Of course, CT execution will not occur on data intended to be handled at run-time (e.g. files, unless explicitly stated -- notably it content is known at compile time).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># a good part of the CT simplifications are automatic </span>
<span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> [ <span style="color: #93488f">1</span>, <span style="color: #93488f">2</span>, <span style="color: #93488f">3</span> ].<span style="color: #000000">size</span>
<span style="color: #005782">if</span> <span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">!=</span> <span style="color: #93488f">3</span>
    <span style="color: #000000">never_compiled</span>() <span style="color: #938d7f; font-style: italic"># because size is trivially known at CT</span>

<span style="color: #938d7f; font-style: italic"># they can also be triggered explicitly (`kv` function)</span>
<span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">0</span>
<span style="color: #005782">for</span> <span style="color: #000000">i</span> <span style="color: #005782">in</span> <span style="color: #93488f">0</span> .. <span style="color: #93488f">10</span>
    <span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">+=</span> <span style="color: #000000">i</span>
<span style="color: #005782">if</span> <span style="color: #000000">kv</span> <span style="color: #000000">b</span> <span style="color: #6d3642; font-weight: bold">!=</span> <span style="color: #93488f">45</span> <span style="color: #938d7f; font-style: italic"># kv( b ) is known to be 45 at CT</span>
    <span style="color: #000000">never_compiled</span>() <span style="color: #938d7f; font-style: italic"># </span>

<span style="color: #938d7f; font-style: italic"># it can work with files if explicitly stated</span>
<span style="color: #000000">c</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">load</span>( <span style="color: #118811">&quot;fs&quot;</span> ).<span style="color: #000000">read_file_sync</span> <span style="color: #118811">&quot;my_file&quot;</span>, <span style="color: #000000">kv</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #005782">true</span>
<span style="color: #000000">s</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">c</span>.<span style="color: #000000">split</span>().<span style="color: #000000">filter</span>( <span style="color: #000000">x</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #000000">x</span>.<span style="color: #000000">size</span> <span style="color: #6d3642; font-weight: bold">==</span> <span style="color: #93488f">3</span> ).<span style="color: #000000">size</span>
<span style="color: #000000">info</span> <span style="color: #000000">kv</span> <span style="color: #000000">s</span> <span style="color: #938d7f; font-style: italic"># computed entirely at CT</span>

<span style="color: #938d7f; font-style: italic"># triggering can be implicitly </span>
<span style="color: #000000">i</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">MyClass</span>[ <span style="color: #000000">s</span> ] <span style="color: #938d7f; font-style: italic"># we store the symbolic repr</span>
<span style="color: #000000">j</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">MyClass</span>[ <span style="color: #93488f">17</span> ] <span style="color: #938d7f; font-style: italic"># but here, the equality operator forces the computation</span>
</pre></div>


<!-- Generative Programming involves generally programs that generate programs. In lot of cases, the point is to ensure that some computations are done before runtime, notably for performance, syntax, factorizations, interoperability…

When programs for code generation are separate from the program to be finally executed, one could use the term external code generation. External code generation is an obvious and very flexible, but generally leads to uncomfortable maintainability issues (problems with tracing, with clarity, with builders generally not designed for that, …).

With the handling of compile time computations, Sane provides tools for embedded code generation. In enables the largest scope of possibilities while leading code far clearer and easier to maintain. -->

## Computed names

All variable names (whether it’s for look up or for creation) may be computed (using `$`s).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># creation of a variable named &#39;i18n&#39;</span>
<span style="color: #000000">i$</span>{ <span style="color: #93488f">2</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #93488f">9</span> }<span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">43</span>
</pre></div>


Besides, a `\[+]` in a variable declaration enable to declare it in a parent scope (`\` for the first parent, `\\` for the second one, ...), even in a class block (notably to define methods and attributes, static or dynamic).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># (extract of the class used for enums is the std libraries)</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">Enum</span>[ <span style="color: #000000">id</span>, <span style="color: #000000">item_names</span> ]
    <span style="color: #938d7f; font-style: italic"># class blocks are executed only once (for a set of template parameters)</span>
    <span style="color: #005782">static</span> <span style="color: #000000">__nb_items</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">0</span>
    <span style="color: #005782">for</span> <span style="color: #000000">name</span> <span style="color: #005782">in</span> <span style="color: #000000">item_names</span> 
        <span style="color: #000000">num</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">post_inc</span> <span style="color: #000000">__nb_items</span>
        <span style="color: #005782">static</span> <span style="color: #938d7f; font-style: italic"># (works also with non-static defs and attrs)</span>
        <span style="color: #00aaff">def</span> \<span style="color: #000000">get_$name</span>
            <span style="color: #000000">Enum</span>[ <span style="color: #000000">id</span>, <span style="color: #000000">name</span>, <span style="color: #000000">item_names</span> ] <span style="color: #000000">__value</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">num</span>

    <span style="color: #00aaff">def</span> <span style="color: #000000">write_to_stream</span> <span style="color: #000000">os</span> 
        <span style="color: #000000">os</span> <span style="color: #6d3642; font-weight: bold">&lt;&lt;</span> <span style="color: #000000">item_names</span>[ <span style="color: #000000">__value</span> ]

    <span style="color: #000000">__value</span> <span style="color: #6d3642; font-weight: bold">~=</span> <span style="color: #000000">SI32</span>
      
<span style="color: #000000">T</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">Enum</span>[ <span style="color: #93488f">0</span>, [ <span style="color: #118811">&quot;A&quot;</span>, <span style="color: #118811">&quot;B&quot;</span> ] ]
<span style="color: #000000">info</span> <span style="color: #000000">T</span><span style="color: #6d3642; font-weight: bold">::</span><span style="color: #000000">A</span> <span style="color: #938d7f; font-style: italic"># =&gt; A (thanks to the generated `static def get_A` in the class scope)</span>
</pre></div>


## CT Symbolic computations

Sane give access to SSA graphs either for reading, modification or creation.

Thus we can for instance get a symbolic representation of what a function does, as we can generate code from the construction of a SSA graph.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># extract of a function to differentiate at a graph level</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">diff</span> <span style="color: #000000">node</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">CtGraph</span>
    <span style="color: #000000">switch</span> <span style="color: #000000">node</span>
        <span style="color: #000000">Symbolic</span><span style="color: #6d3642; font-weight: bold">::</span><span style="color: #000000">mul</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span> <span style="color: #000000">node</span>.<span style="color: #000000">children</span>[ <span style="color: #93488f">0</span> ] <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">diff</span> <span style="color: #000000">node</span>.<span style="color: #000000">children</span>[ <span style="color: #93488f">1</span> ] <span style="color: #6d3642; font-weight: bold">+</span>
                         <span style="color: #000000">node</span>.<span style="color: #000000">children</span>[ <span style="color: #93488f">1</span> ] <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">diff</span> <span style="color: #000000">node</span>.<span style="color: #000000">children</span>[ <span style="color: #93488f">0</span> ] 
        ... 

<span style="color: #938d7f; font-style: italic"># a simple function</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">foo</span> <span style="color: #000000">n</span>, <span style="color: #000000">x</span>
    <span style="color: #000000">res</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">typeof</span>( <span style="color: #000000">x</span> ) <span style="color: #93488f">0</span>
    <span style="color: #005782">for</span> <span style="color: #000000">i</span> <span style="color: #005782">in</span> <span style="color: #93488f">1</span> .. <span style="color: #000000">n</span>
        <span style="color: #000000">res</span> <span style="color: #6d3642; font-weight: bold">+=</span> <span style="color: #000000">pow</span> <span style="color: #000000">x</span>, <span style="color: #000000">i</span>
    <span style="color: #000000">res</span>

<span style="color: #938d7f; font-style: italic"># graph representation + differentiation of foo</span>
<span style="color: #000000">x</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">ct_symbol</span> <span style="color: #000000">FP64</span>
<span style="color: #000000">n</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">ct_symbol</span> <span style="color: #000000">SI32</span>
<span style="color: #000000">sym_diff</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">diff</span> <span style="color: #000000">ct_graph</span>( <span style="color: #000000">foo</span>( <span style="color: #000000">x</span>, <span style="color: #000000">n</span> ) ), <span style="color: #000000">x</span>

<span style="color: #938d7f; font-style: italic"># compile time optimized differentiation of foo</span>
<span style="color: #000000">foo_diff</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">sym_diff</span>.<span style="color: #000000">subs</span> [ <span style="color: #000000">x</span>, <span style="color: #000000">n</span> ], [ <span style="color: #000000">xv</span>, <span style="color: #000000">nv</span> ]
</pre></div>


These symbolic representations can be seen as the level above the AST representation (also accessible via std introspection as shown later). They enable what one can call "code instrospection and filtering".

It is for instance used in the module `vectorize` (providing ways to simplify the writing of loops with SIMD instructions). In all the cases, transformations are triggered by standard calls and are a library concern (meaning that you have the choice to select them, to modify or create new ones for full control and extensibility).

## ct_eval

For the extreme cases, it is possible to pass an arbitrary string to the compiler (as long of course as it can be CT computed).

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">5</span>
<span style="color: #000000">ct_eval</span> <span style="color: #118811">&quot;def foo; </span>${ <span style="color: #93488f">1</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #000000">a</span> }<span style="color: #118811">&quot;</span>
<span style="color: #000000">info</span> <span style="color: #000000">foo</span>() <span style="color: #938d7f; font-style: italic"># =&gt; 6</span>
</pre></div>


> This construction is probably the closest to the one we can find in "code generation", with a reduction of problems coming from information scattering and tangled build configuration. Nevertheless, computed names with CT branching usually offer superior clarity and debugging support.

## Ab initio primitives

In the same vein, it is possible to handle structures instead of text (as e.g. handling a DOM vs the html text).

Almost every objects handled by the compiler is actually accessible and modifiable within the language. It’s valid for types, scopes, definition of methods, classes, and so on.

Compilation objects can thus be created *ab initio*, to be consecutively handled by the compiler.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># creation</span>
<span style="color: #000000">T</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">Type</span>()
<span style="color: #000000">T</span>.<span style="color: #000000">name</span> <span style="color: #6d3642; font-weight: bold">=</span> <span style="color: #118811">&quot;MyType&quot;</span>
<span style="color: #000000">T</span>.<span style="color: #000000">attributes</span> <span style="color: #6d3642; font-weight: bold">&lt;&lt;</span> <span style="color: #000000">Type</span><span style="color: #6d3642; font-weight: bold">::</span><span style="color: #000000">Attribute</span> <span style="color: #000000">off</span><span style="color: #6d3642; font-weight: bold">:</span><span style="color: #93488f">0</span>, <span style="color: #000000">name</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #118811">&quot;a&quot;</span>, <span style="color: #000000">type</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">SI32</span>

<span style="color: #938d7f; font-style: italic"># the created type is stored and handled exactly like the others</span>
<span style="color: #000000">i</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">T</span> <span style="color: #000000">a</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #93488f">100</span>
<span style="color: #000000">info</span> <span style="color: #000000">i</span> <span style="color: #938d7f; font-style: italic"># =&gt; T( a: 100 )</span>
</pre></div>



# Active libraries

## Selection

In Sane, functions implementations can be selected according to their a posteriori usage.

When functions (or methods) that are qualified switchable are called, Sane marks theirs outputs in the graph (including captured variables). Before code emission, these marks are sent to the registered “switch procedures”, allowing to change the actual implementations.

There possibilities are used in a lot of essential optimisations. For instance, the concatenation operator works only with two variables (because it is an operator). If we want to concatenate more than two variables using this operator, we may end up with a lot of intermediate allocations and copies. A posteriori selection enable to gather the concatenations to avoid the waste.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># a standard matrix class (with a `switchable` procedure)</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">Matrix</span>
    <span style="color: #005782">switchable</span>
    <span style="color: #00aaff">def</span> <span style="color: #000000">operator</span><span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">that</span>
        <span style="color: #938d7f; font-style: italic"># -&gt; normal multiplication procedure</span>
    ...

<span style="color: #938d7f; font-style: italic"># standard multiplications (left to right)</span>
<span style="color: #000000">a</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">p</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">q</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">r</span> <span style="color: #6d3642; font-weight: bold">*</span> <span style="color: #000000">v</span>

<span style="color: #938d7f; font-style: italic"># addition of a &quot;swither&quot; (a different implementation, providing the same result)</span>
<span style="color: #938d7f; font-style: italic"># to call a procedure able to find the best ways to do the multiplications</span>
<span style="color: #000000">globals</span>.<span style="color: #000000">switchers</span>.<span style="color: #000000">add</span>
    <span style="color: #000000">oper</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">Matrix</span><span style="color: #6d3642; font-weight: bold">::</span><span style="color: #000000">operator</span><span style="color: #6d3642; font-weight: bold">*</span>
    <span style="color: #000000">func</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">inps</span>, <span style="color: #000000">outs</span> <span style="color: #6d3642; font-weight: bold">=&gt;</span>
        <span style="color: #938d7f; font-style: italic"># (simplified code)</span>
        <span style="color: #005782">return</span> [ <span style="color: #000000">nary_multiplier</span> ... ]
</pre></div>





# Examples of active libraries

## Auto-tuning

Example

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #00aaff">def</span> <span style="color: #000000">my_test</span>
    <span style="color: #000000">exec_time</span> <span style="color: #000000">f</span> <span style="color: #93488f">0</span> .. <span style="color: #93488f">10000</span> <span style="color: #938d7f; font-style: italic"># representative parameter(s)</span>
<span style="color: #00aaff">def</span> <span style="color: #000000">f</span>( <span style="color: #000000">range</span> )
    <span style="color: #000000">simd_size</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">TuningParameter</span>( <span style="color: #000000">my_test</span>, [<span style="color: #93488f">2</span>,<span style="color: #93488f">4</span>,<span style="color: #93488f">8</span>] )
    <span style="color: #938d7f; font-style: italic"># -&gt; simd_size.val is fixed and known at compile time</span>
    <span style="color: #005782">for</span> <span style="color: #000000">v</span> <span style="color: #005782">in</span> <span style="color: #000000">simd_loop</span>( <span style="color: #000000">range</span>, <span style="color: #000000">simd_size</span>.<span style="color: #000000">val</span> )
        ...
</pre></div>

        
## String storage

String storage and interpolation give a good example on transformations that take place during the compilation that helps develop truly convenient solution while ensuring to get the best performances.

STORAGE

Strings created using double quotes are by default of type String (which is a clone of the std::string defined in clang). It is very convenient for a vast majority of applications, but for large sizes, it may involve dynamic memory allocation. For some performance critical or embeded applications it is simply a no go.

Incidentally, allocation is understood by the compiler. If the there’s no real need for this dynamic memory because it is finally copied elsewhere, the allocation and desallocation won’t take place at all.

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># there&#39;s a *symbolic* allocation, actually not executed at all</span>
<span style="color: #938d7f; font-style: italic"># because only the content is used (to get a `static const char *`)</span>
<span style="color: #000000">p</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">StaticCStr</span> <span style="color: #118811">&quot;string with length &gt; 23 chars...&quot;</span>

<span style="color: #938d7f; font-style: italic"># and now, we have a pointer in .data, on a zero-ended string</span>
<span style="color: #000000">strlen</span> <span style="color: #000000">p</span> <span style="color: #6d3642; font-weight: bold">+</span> <span style="color: #93488f">2</span> <span style="color: #938d7f; font-style: italic"># returns length - 2</span>
</pre></div>


INTERPOLATION

String interpolations ("...${...}...") may also involve memory allocation which may be abad thing e.g. if not done with the right allocator… But it actually occurs only if really needed, depending on the target of the string.

In Sane, "...${...}..." creates a symbolic graph

<div class="highlight" style="background: #ffffff"><pre style="line-height: 125%"><span></span><span style="color: #938d7f; font-style: italic"># extract of the Enum class definition</span>
<span style="color: #00aaff">class</span> <span style="color: #000000">Enum</span>[ <span style="color: #000000">id</span>, <span style="color: #000000">name</span>, <span style="color: #000000">item_names</span> ]
    <span style="color: #005782">static</span> <span style="color: #000000">__nb_items</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #93488f">0</span>
    <span style="color: #005782">if</span> <span style="color: #000000">__nb_items</span> <span style="color: #6d3642; font-weight: bold">==</span> <span style="color: #93488f">0</span>
        <span style="color: #005782">for</span> <span style="color: #000000">item_name</span> <span style="color: #005782">in</span> <span style="color: #000000">item_names</span> 
            <span style="color: #000000">num</span> <span style="color: #6d3642; font-weight: bold">:=</span> <span style="color: #000000">post_inc</span> <span style="color: #000000">__nb_items</span>
            <span style="color: #005782">static</span>
            <span style="color: #00aaff">def</span> \\<span style="color: #000000">get_$item_name</span>
                <span style="color: #000000">Enum</span>[ <span style="color: #000000">id</span>, <span style="color: #000000">name</span>, <span style="color: #000000">item_names</span> ] <span style="color: #000000">__value</span><span style="color: #6d3642; font-weight: bold">:</span> <span style="color: #000000">num</span>

    <span style="color: #000000">__value</span> <span style="color: #6d3642; font-weight: bold">~=</span> <span style="color: #000000">SI32</span>
</pre></div>

      


# Asynchronous code

async/await are cool but intrusive and procedural, whereas data driven would be more relevant.

Besides, a runtime can’t be defined by a language, it has to be a library: needs are the same if you’re on the embeded world, if you’re developping a game, and so on…

