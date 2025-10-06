# Decompilation Help

## Recognizing Macros

### GTE Macros

During decompilation, there may be times where a function has some weird, out-of-place instructions that aren't being handled properly. In assembly, the mnemonics will often be one of the following:

```assembly
mtc2
mfc2
ctc2
swc2
```

In decompilation, these will manifest as errors:

```c
M2C_ERROR(/* unknown instruction: mtc2 $t4, $25 */);
M2C_ERROR(/* unknown instruction: mtc2 $t5, $26 */);
M2C_ERROR(/* unknown instruction: mtc2 $t6, $27 */);
M2C_ERROR(/* unknown instruction: mtc2 $t4, $9 */);
M2C_ERROR(/* unknown instruction: mtc2 $t5, $10 */);
M2C_ERROR(/* unknown instruction: mtc2 $t6, $11 */);
M2C_ERROR(/* unknown instruction: gpl 0x1 */);
```

If you're decompiling a function and encounter these, then what you've found is a [**GTE**](https://psx-spx.consoledev.net/geometrytransformationenginegte/) (**G**eometry **T**ransformation **E**ngine) macro. These macros can be found in [SDK.h](/include/Libs/SDK.h) and are always prefixed with `gte_`. The GTE is a specialized coprocessor that is designed to quickly perform complex mathematical operations, which are usually graphics related tasks such as lighting calculations or 3D transformations. If you encounter a GTE macro that isn't present in the repo, you can find the correct macro from the [gte_macro collection](https://github.com/Decompollaborate/rabbitizer/blob/fb4ab5e24994a5987c5789983789d8d3dfc7a40f/docs/r3000gte/gte_macros.h).

### Other Macros

When setting up a function to decompile, in the `decls.h` file you may see patterns such as:

```c
{ //line: 13
  // register: $v0
  register short _x0;
  // register: $v1
  register short _y0;
  // register: $a1
  register short _z0;
  // register: $a2
  register short _x1;
  // register: $t0
  register short _y1;
  // register: $t1
  register short _z1;
  // register: $a3
  // size: 0x6
  register struct _Position *_v;
} //line: 13
```

If you see patterns like this appear in the outline of a function, chances are this is a non-GTE macro. Most of these are simple operations for vectors, positions, and rotations. These macros can be found in [MATH3D.h](../include/Game/MATH3D.h).

## Compiler Patterns

As you go through and decompile code, you'll often begin to see that there are
common patterns that emerge during the initial decompile. These are usually
the result of some compiler optimization.

### Array Accesses

These are the most common by far. To fully understand why this pattern manifests, it's helpful to understand the way that array element are ordered in memory.

Under the hood, array elements are stored contiguously in memory. So the address for an array element is:

```c
Array Address + (Index * Element Size)
```

So if we had an array of integers `myArray`, a given element `i` is located at:

```c
myArray + (i * 4); // integers are 4 bytes in size
```

Even though this code will work, the C devs gave us a much more convenient way to do this:

```c
myArray[i];
```

There's tons of array accesses throughout the project. But when you start decompiling, the initial decompile will usually spit out the verbose format because it isn't always sure about what is and isn't an array access or some other pointer manipulation.

So something like:

```c
G2Matrix segMatrix;
G2Anim *anim;
G2AnimController *controller;

segMatrix = *(anim->segMatrices + (controller->segNumber * 32));
```

This looks like an array access. If we look in [TYPES.h](/include/Game/TYPES.h), we'll see that the size of a `G2Matrix` is 0x20 bytes (32 in decimal), confirming our findings.

So we can now rewrite this as:

```c
G2Matrix segMatrix;
G2Anim *anim;
G2AnimController *controller;

segMatrix = anim->segMatrices[controller->segNumber];
```

### Absolute Value

This is rather common, and quite easy to spot.
There are several different variations of this but they all follow the same general structure.
If you see a block of code similar to this:

```c
int a;
int b;

a = b;

if (a < 0)
{
  a = -a;
}
```

Then it can usually be rewritten as:

```c
int a;
int b;

a = abs(b);
```

### Loops

There are a few different ways that loops can be decompiled.

First, the `for` loop.

### Bit Clearing

### Fixed-point Arithmetic

### Load Optimization

## Misc

### Calculating Stack Offsets

### The Permuter

The permuter is a tool that will, as the name implies, create thousands of
small permutations of your current source code with the goal of achieving a better match.
This is immensely helpful if you have a function mostly matched, but are stuck with some odd instruction ordering or register usage.
