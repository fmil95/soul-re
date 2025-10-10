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

As you go through and decompile code, you'll begin to see that there are common patterns that emerge during the initial decompile.

### Array Accesses

These are the most common by far. To fully understand why this pattern manifests, it's helpful to understand the way that array elements are ordered in memory.

Under the hood, array elements are stored contiguously in memory. So the address for an array element is:

```latex
ElementAddress = ArrayAddress + (Index * ElementSize)
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
a = abs(b);
```

### Loops

There are a few different ways that loops can be decompiled.

First, the `for` loop.

### Clearing Bitflags

When looking at bitwise operations on flags, you may see a flag AND-ed with some very high value:

```c
camera->flags &= 0xF7FF;
```

Although this code works, that is likely _not_ what the developer actually wrote, and doesn't accurately reflect what is really going on here. Let's look at the binary representation of this value:

```c
   F    7    F    F
1111 0111 1111 1111
```

We can see that every bit is set except one. What's actually happening here is a single bit is being cleared. When clearing bitflags, it's very uncommon for the actual inverse value to be written in code, instead being represented as the bitwise negation `~` of the bit in question:

```c
   0    8    0    0
0000 1000 0000 0000
```

So the actual code should look something like this:

```c
camera->flags &= ~0x800;
```

Although the 2 code snippets above do the exact same thing, semantically the second one makes much more sense. This is especially evident when flags have explicit names or definitions:

```c
#define MY_FLAG 0x80       // Binary 1000 0000

camera->flags |= MY_FLAG;  // Setting the flag
camera->flags &= ~MY_FLAG; // Clearing the flag
```

### Fixed-point Arithmetic

### Load Optimization

## Misc

### Calculating Stack Offsets

### The Permuter

The permuter is a tool that will continuously create small permutations of your current source code with the goal of achieving a better match. This is immensely helpful if you have a function mostly matched, but are stuck on some odd instruction ordering or register usage.

Using the permuter is simple, just copy the link to your decomp scratch and run the following terminal command:

```bash
python tools/scripts/permuter.py <LINK TO YOUR SCRATCH>
```
