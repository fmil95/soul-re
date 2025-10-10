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

$$Address_{Element} = Address_{Array} + (Index * Size_{Element})$$

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

### Resolving Stack Variables

One of the quickest wins you can get when starting a new function is resolving the stack variables. All it takes is a bit of math.

We have 3 key sources of information when decompiling a function:

1. The assembly - Gives the size of the stack
2. The initial decompile - Outlines the stack variables and their position within the stack
3. The `decls` file - Gives the names+types of the variables, as well as their offset from the top of the stack

With these pieces of information, all we need to do to resolve the names and types of our stack variables is follow this formula:

$$Position_{Element} = Size_{Stack} - Offset_{Element}$$

Let's take a look at a real function from this project: `CanBePickedUp` from [PHYSOBS.c](/src/Game/PHYSOBS.c). Looking at the function's assembly, the very first instruction allocates our stack:

```assembly
addiu   sp,sp,-0x60   ; Reserve 0x60 bytes for the stack
```

This gives us a vital piece of information: The stack is 0x60 bytes in size.
Now let's take a look at the initial decompile:

```c
s32 CanBePickedUp(void* arg0, void* arg1, s32 arg2) {
  short sp10;
  short* sp24;
  short* sp28;
  short sp40;
  short sp42;
  short sp44;
  short sp48;
  short sp4A;
  short sp4C;

  // rest of the code
}
```

There are several variables that are named `spXX`. The `XX` is the _position on the stack_ that this value lives at.

Now let's look at the corresponding function outline in the `decls` file:

```c
int CanBePickedUp(struct _Instance *instance, struct _Instance *Force, int LinkNode)
{ //line: 1
  { //line: 24
    // address: -80
    // size: 0x2C
    auto struct _PCollideInfo pcollideInfo;
    // address: -32
    // size: 0x6
    auto struct _Position newPos;
    // address: -24
    // size: 0x6
    auto struct _Position oldPos;
    { //line: 47
    } //line: 56
  } //line: 58
} //line: 62
```

Let's walk through the first variable: `pcollideInfo`. It's annotated as 80 bytes from the top of the stack. So we know where it is relative to the top of the stack, and we know how big the stack is, so let's do a little math:

- The stack is 0x60 bytes
- `pCollideInfo` is 80 bytes from the top of the stack.
- First, let's convert that to hexidecimal: 80 -> 0x50
- 0x60 - 0x50 = **0x10**

Wait a second, we have a variable `sp10`! We now know that `sp10` is a `PCollideInfo` object called `pcollideInfo`.

Let's do the same for the other 2 stack variables, we end up with:

```c
s32 CanBePickedUp(Instance *instance, Instance *Force, int LinkNode) {
  PCollideInfo pcollideInfo; // sp10
  short* sp24;
  short* sp28;
  Position newPos; // sp40
  short sp42;
  short sp44;
  Position oldPod; // sp48
  short sp4A;
  short sp4C;

  // rest of the code
}
```

You'll notice there are more `sp` variables in the decompile than there are in the function outline. Those leftover variables the _fields within the stack variables themselves_.

Let's take a look at `newPos`. It is at 0x40 on the stack, and there are 2 more values after that at 0x42 and 0x44. Note that all of these values are 2 bytes apart.

Next, let's take a look at the definition of the `Position` struct in [TYPES.h](/include/Game/TYPES.h):

```c
// size: 0x6
typedef struct _Position
{
  // offset: 0x0000
  short x;
  // offset: 0x0002
  short y;
  // offset: 0x0004
  short z;
} Position;
```

Look at that! A `Position` is made of 3 shorts (which are 2 bytes in size). So `sp42`, which is the next 2 bytes, represents the `y` field, and 2 bytes after that is the `z` field. We can now resolve all of the references to `sp42` and `sp44` as `newPos.y` and `newPos.z` respectively!

After doing this to the rest of the stack variables, we are now left with:

```c
s32 CanBePickedUp(Instance *instance, Instance *Force, int LinkNode) {
  PCollideInfo pcollideInfo;
  Position newPos;
  Position oldPod;

  // rest of the code
}
```

Which exactly matches the outline derived from the debug symbols! Resolving stack variables and references to their fields is a great way to immediately get a _lot_ of context with only a little bit of math.

### The Permuter

The permuter is a tool that will continuously create small permutations of your current source code with the goal of achieving a better match. This is immensely helpful if you have a function mostly matched, but are stuck on some odd instruction ordering or register usage.

Using the permuter is simple, just copy the link to your decomp scratch and run the following terminal command:

```bash
python tools/scripts/permuter.py <LINK TO YOUR SCRATCH>
```
