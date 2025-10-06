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
