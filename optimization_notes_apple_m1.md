# Brief notes on Apple M1 Firestorm microarchitecture

Here is a brief explanation of the execution pipeline of Firestorm core (big core) in the Apple M1 processor. This was obtained mainly by interpreting the results of insn\_bench\_arch64 seeing some technical articles.

## Review articles

The most helpful articles were by AnandTech, delivering detailed reviews on the processor. They offer information about cache configuration, decoder width, the number of execution units, physical register file size, reorder buffer size, load and store queue depths, and many other things.

* [The 2020 Mac Mini Unleashed: Putting Apple Silicon M1 To The Test](https://www.anandtech.com/show/16252/mac-mini-apple-m1-tested)
* [Apple Announces The Apple Silicon M1: Ditching x86 - What to Expect, Based on A14](https://www.anandtech.com/show/16226/apple-silicon-m1-a14-deep-dive)

Also, I found Dougall Johnson provides a detailed investigation on the pipeline on [his website](https://dougallj.github.io/applecpu/firestorm.html). Several explanations are different from my understanding, but I believe he is doing a more accurate investigation than me. So please refer to his article as well.

## Decoder and renamer

M1 has a decoder with eight instruction width. As long as instructions are fed without delay, the decoder can consume eight of them per cycle. This characteristic is in contrast to that of x86\_64, where the decode of instructions that do not hit the loop buffer or uOP cache becomes a bottleneck. The renaming unit is also hardly become a bottleneck, too. It can process up to eight inter-register mov instructions in total for scalar and SIMD/fp domains. Chained movs are also eliminated, for ones in the scalar domain. NOP instruction is also removed in the decoder and consumes no execution unit.

An interesting thing about M1 is that it processes up to eight immediate-loading mov instructions per cycle, probably when it enqueues them into the reservation station. This makes immediate-loading mov not consume any execution unit. Also, operations clearing a vector register are processed eight per cycle without any execution unit, implying there is a physical zero register in the SIMD/fp domain, too.

Unlike most x86\_64 processors, M1 does not put a special treat on clearing registers using bitwise xor operation (EOR instruction). They are processed in the execution unit like other normal instructions. Dependency breakage from source to destination is also not detected for these operations.

## Instruction feeder and branch units

The details of the branch prediction mechanism are unknown. However, it's more powerful than the Neoverse-N1's one with no doubt and seems to show as good prediction performance as similar-generation x86\_64 processors such as AMD Zen 2 and Intel Sunny Cove. The branch misprediction penalty is probably 13 or 14 cycles.

The level 1 instruction (L1I) cache is 192KB large and feeds 8 instructions to the decoder per cycle. When both the L1I cache and the level 1 branch target buffer (L1 BTB) hit, the instruction feeder can jump to the predicted target every cycle without delay. A branch that hits L1I but misses L1 BTB delays the instruction feeding by two clock cycles. There seem 1024 entries in the first level BTB while the exact configuration is unclear.

On the execution unit side, M1 can process (dispatch and commit) up to two branch instructions per cycle. However, this throughput is obtained only when one of the two branch instructions is a not-taken conditional one, probably due to the limitation in the instruction feeder. The simplest unconditional branch (B instruction) does not consume integer execution units, but the others including BL, BLR, and RET consume one slot of flag-manipulating integer units. Branch instructions do not delay dispatching or committing other instructions. So the latency of branch instructions is usually hidden by the latency of the others.

## Load and store units

M1 has four load/store units in total: two for the load, one for the store, and one for both. Thus the maximum throughput is three for load instructions and two for stores. Each load/store unit has a 128-bit data path that accesses both the scalar and SIMD/fp domains. They also have address generation units, processing simple signed and unsigned offset forms without integer uOP. However, pre- and post-index forms generate an additional uOP to write the updated address back.

The latency of load operations appears to be three cycles in the scalar and five in the SIMD/fp domain. Unaligned loads, cross-cache loads, and sign extensions put a penalty of one clock cycle. In some cases, the unaligned load penalty seems to be hidden by that of sign extension, though the details are unknown. The page-crossing penalty is around 28 cycles. Turnaround latency for a load operation from the same address after a store (reading back via the fill buffer) looks like five or six cycles. A mechanism like memory aliasing doesn't seem to exist.

The memory unit handles the interleaving load and de-interleaving store operations comparatively efficiently. The permute unit is pipelined and can perform one three-register permute operation (LD3 or ST3) every cycle. The latency of the permutation looks to be two or three cycles.

I haven't investigated the depths of load queues and store queues. AnandTech's article reports that they have more than 150 and 100 entries, respectively.

## Arithmetic logic units

The execution unit configuration was estimated, from the results of insn\_bench\_aarch64. The supplementary measurement script is in the `extra/` directory.

### Scalar domain

Table 1 shows the capability of the six execution units in the scalar domain. The unit names (port names) of X0 to X5 are given arbitrarily. All the six units can process basic arithmetic and logical instructions. Also, these six units are independent of the load/store units.

|                                                              | X0   | X1   | X2   | X3   | X4   | X5   |
| ------------------------------------------------------------ | ---- | ---- | ---- | ---- | ---- | ---- |
| add, sub, logic, shift, ctz, rev                             | v    | v    | v    | v    | v    | v    |
| └ shift form                                                 | v    | v    | v    | v    | v    | v    |
| └ address update (pre-index and post-index load / store)     | v    | v    | v    | v    | v    | v    |
| mul, mulh                                                    |      |      |      | v    | v    |      |
| multiply-add, bitfield move (bfm), crc32                     |      |      |      | v    |      |      |
| div                                                          |      |      |      |      | v    |      |
| flag-seting add, add with carry, compare, conditional compare | v    | v    | v    |      |      |      |
| branch, conditional branch                                   | v    | v    |      |      |      |      |

Table 1: Estimated allocation table for the scalar domain.

M1 can process basic arithmetic and logical instructions in all units from X0 to X5 in a single clock cycle. What is striking is that variable-width shift, bit reversal, and count trailing zeros are also single-cycle operations. As referred to in the previous paragraph, they also handle the address register update operations for the pre- and post-index load/store instructions.

Shift-form arithmetic and logical instructions increase the latency by one cycle from the base form. Shift-forms are processed in all execution units from X0 to X5. The non-shifted operands are also dispatched together as the shifted side, indicating the shift part is not separated into a uOP.

It has two multiply units in X3 and X4. One of which (X3) can also handle multiply-add instructions. The multiply-add operation seems expressed as a single uOP, dispatched to a single unit every cycle and executed in three cycles. The X3 unit is also in charge of bitfield move and CRC32 and processes each instruction using one slot.

The X4 unit, the multiply unit without multiply-add capability, handles divide instructions instead. The latency of divide instructions seems seven cycles at the minimum. Consecutive divides are dispatched with a dead slot of one in between, resulting in a throughput of one instruction per two cycles. However, divide instructions do not seem to limit or delay the execution of non-divide instructions; the slot immediately after a divide is open for them, and the results are forwarded to the succeeding instructions before the divide instructions finish execution. Perhaps divide instruction has a dedicated execution pipeline and shares only the dispatch queue part with others, which is consistent with AnandTech's description.

Instructions that touch condition flags go into three units, X0, X1, and X3. I guess all three, not part of them, also process conditional compare instructions, whereas I have no definitive evidence about this. Conditional branch, branch-with-link, and return instructions also use a slot in the three (or perhaps two of the three) units.

### SIMD/fp domain

There are four execution ports in the SIMD/fp domain, which I denote V0 to V3. All ports are capable of most of the arithmetic and logic instructions.

|                         | V0   | V1   | V2   | V3   |
| ----------------------- | ---- | ---- | ---- | ---- |
| almost all instructions | v    | v    | v    | v    |
| fp div, recip, rsqrt    | v    |      |      |      |
| fp flag-setting comapre | v    |      |      |      |
| fp conditional select   | v    | v    |      |      |
| crypto                  | v    |      |      |      |
| fjcvtzs                 | v    |      |      |      |

Table 2: Estimated allocation table for the SIMD/fp domain.

All execution units, V0~V3, processes basic arithmetic and logical operations with a latency of two cycles and a throughput of four per clock cycle. Most others are three-cycle instructions, such as saturating and rounding arithmetic, horizontal, high-narrow form, and absolute difference operations. It is noticeable that integer multiplication and multiply-accumulate are processed in three cycles, too.

V0 is a fat unit that handles division and some special instructions. Instructions processed only by this unit are reciprocal, square root, flag operation comparison instruction, encryption such as SHA, and the javascript-style conversion instruction (FJCVTZS) in addition to division. Conditional selection instructions are processed in two units, V0 and V1.

Transfer between the scalar and SIMD/fp domains is intriguing. The forward transfer, scalar-to-SIMD/fp direction, uses a slot in the load units, and the reverse has dedicated paths. Latency appears to be six cycles in both directions. Throughput is three instructions every cycle for the forward and two for the opposite direction. A simple forward transfer (FMOV instruction) does not consume any unit other than the load, but broadcast and insert forms (DUP and INS instructions) consume an additional slot in the SIMD/fp domain. Reverse transfer uses one slot in the SIMD/fp and several in the scalar domain for all forms and types. In either direction, a transfer instruction may fuse with an operation immediately after the transfer, where the latency for one cycle is hidden.

Conversion from integer to floating point is executed on all units. If the write destination is a floating point register, a conversion instruction occupies only one slot in the SIMD/fp domain. In this case, the latency and throughput are three cycles and four per cycle, respectively, for all the instructions. When the destination is a scalar register, the latency increases by six cycles, and the throughput is limited to three per cycle. The additional characteristics are equivalent to that of the transfer instruction.

The same applies to the conversion from floating point numbers to integers. When the destination is a SIMD register, the latency and throughput are three cycles and four per cycle, respectively. Transferring to a scalar register adds the characteristics of the transfer instruction.

### Condition flags

M1 has independent condition flag files for the scalar and SIMD/fp domains. Flag transfer across domains will probably be penalized for six cycles each time, while in-domain chains have no penalty. The forward transfer uses a dedicated path that doesn't interfere with any other operation, and the reverse shares a route with the register transfer.

## Reorder buffer and commit unit

I haven't investigated the detailed characteristics of the reorder buffer and commit unit. According to the AnandTech article, the reorder buffer has more than 600 entries.

## Miscellaneous thoughts about tuning programs on M1

There are few specific parts in the M1 pipeline that are likely to be a bottleneck. In most cases, the program itself would be the most likely reason for clogging. Shortening critical path lengths at the expense of more instructions makes the program faster because we generally have unutilized slots left in the decoder and execution units. Because the instruction feeder and decoder have concise characteristics, this rule would apply to almost all CPU-intensive programs. It also makes tuning easier that few instructions have unusual execution latency and throughput.

However, some characteristics could sometimes become weak points. Needless to say, the total throughput of floating point arithmetic is inferior to that of x86\_64 processors. A code that densely alternates floating point compare and conditional branch may delay due to a large latency of transfer between the two domains, too. Integer SIMD instructions having minimum two-cycle latency can make a bottleneck when batch size is small. Such structures or natures are fundamentally difficult to remove by a compiler or by hand and would be examples of code that M1 is not good at.

M1 has simple and uniform characteristics overall, allowing us to focus on making algorithms and code better. Since the code tuning on M1 doesn't require a profound knowledge of the architecture's pitfalls, those accustomed to x86\_64 may even find it boring. Also, it doesn't give us the joy of discovering tricky applications of specific-purpose instructions, whereas it provides an attractive puzzle of reassembling algorithms with polymorphic general-purpose operations instead. We often find it the solution that a single structured instruction expresses a combination of multiple algorithmic elements, since the A64 ISA provides us rich variations in the bitshift, width-converting arithmetic, and load/store instructions. The A64 being a relatively orthogonal and multifunctional ISA emphasizes the combinatorial problem aspect of code tuning that seems slightly less common in the conventional x86\_64 architectures.

