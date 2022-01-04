# Riru-SsageHook
---
- 注入方面
  调用了Riru来对Zygote进行注入
    <details>
    <summary>Zygote</summary>

  > 安卓下进程都是从Zygote fork的. <br>
  > 当注入so到Zygote后,之后启动的进程就都会带有这个so. <br>
  > 可以自己注入Zygote进程或者使用Riru这个模块. <br>

    </details>

- Hook方面

  - 首先,引用Android官方的一些资料
    <details>
    <summary>Android官方的一些资料</summary>

    > ---
    > - 使用原生代码时,硬件很重要.NDK 提供各种 ABI 供您选择,可让您确保针对正确的架构和 CPU 进行编译.<br>
    > - 不同的 Android 设备使用不同的 CPU,而不同的 CPU 支持不同的指令集.CPU 与指令集的每种组合都有专属的应用二进制接口 (ABI).<br>
        您可以通过多种方式检查代码中的 CPU 功能,但每种方式都需要做出不同的取舍.<br>
    > ---
    > **支持的 ABI** <br>
    > |ABI|支持的指令集|备注|
    > |----|----|----|
    > |armeabi-v7a|armeabi<br>Thumb-2<br>Thumb-2<br>VFPv3-D16|与 ARMv5/v6 设备不兼容|
    > |arm64-v8a|AArch64||
    > |x86|x86 (IA-32)<br>MMX<br>SSE/2/3<br>SSSE3<br>|不支持 MOVBE 或 SSE4|
    > |x86_64|x86-64<br>MMX<br>SSE/2/3<br>SSSE3<br>SSE4.1、4.2<br>POPCNT||
    > ---
    > 有兴趣的请参考[Android官方资料](https://developer.android.com/ndk/guides/arch?hl=zh-cn)
    >
    > ---
    > **ABI：使用预处理器的预定义宏** <br>
    > 通常，在构建时使用 #if defined 及以下各项确定 ABI 最为方便：<br>
    >   - 对于 32 位 ARM，使用 __arm__ <br>
    >   - 对于 64 位 ARM，使用 __aarch64__ <br>
    >   - 对于 32 位 X86，使用 __i386__ <br>
    >   - 对于 64 位 X86，使用 __x86_64__ <br>
    >
    > 请注意：32 位 X86 称为 __i386__，而不是 __x86__，这可能与您预想的有所不同！<br>
    >
    > ---

    </details>

  - 而 inlinehook 显然和汇编指令集有不可分割的关联<br>
    因此 对于 hook 我们需要根据 abi 的不同 <br>
    再根据 汇编指令集的区别 分别使用不同的hook手法 <br>

  - 对于32位进程 也就是 abi 为 armeabi-v7a 的
    - 常见的汇编指令集合为 armeabi 的 arm 模式
    - 以及 Thumb-2 的 Thumb 模式

  - 对于64位进程 也就是 abi 为 arm64-v8a 的
    - 唯一的汇编指令集合为 AArch64 的 arm64 模式

  - 在本项目中
    - 采用一个跨平台 inlinehook 框架 Dobby(https://github.com/jmpews/Dobby)


---
Made By SsageParuders