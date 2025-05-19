# PicoLlama - Llama2 on-device model for the RP2350 + PSRAM

Run a LLM ("large" language model) right on your Pico2 + PSRAM device,
no network needed.  For very limited definitions of the word "large."

The onboard model needs to fit in onboard flash so is very small and limited,
but it is running the same algorithm as its larger LLAMA2-based brethren.

For use only with [arduino-pico](https://github.com/earlephilhower/arduino-pico)
due to the need for RP2350 and PSRAM support.

MIT licensed.  Based off of https://github.com/karpathy/llama2.c

## Overview

This library for Arduino implements a single-file LLAMA2.C to run
inference on a small, onboard LLAMA2 LLM model.  This model presently
generates text (simple stories) but can be replaced with domain-specific
ones for text, music, or more.

Examples are included which can generate text interactively and actually
read generated stories out loud using [BackgroundAudio](https://github.com/earlephilhower/BackgroundAudio).

## Requirements

This library needs 16MB of flash and 8MB of PSRAM.  Unfortunately there
is not enough SRAM on the chip itself to hold the LLM state so the OG
Pico2 boards will not function.  I've developed it on the Pimoroni PGA2350
but any other board with the needed flash and PSRAM will work.

## How it works on the Pico2

Flash is used to store the constant matrices while SRAM and PSRAM are
used to store the state of the LLM activations.  All math is done in FP32
(i.e. `float`) which has hardware support on the RP2350 ARM core.  (Note
that you can also use the RISC-V Hazard3 core by selecting it from the
menus but it will be ~4x slower due to lack of FPU).

Note that it may take several minutes to upload the full 16MB executable image.

## Generating custom models

For real-world models, large GPU clusters with gobs of memory running for
weeks 24/7 are required.  But for models running on the Pico2, due to their
size limitation, can be trained on a single NVidia 1650-class 4GB card in
a matter of hours!

See the [upstream README](README.upstream.md) for instructions on building
a Python `venv` to train in and using HuggingFace datasets.  Remember, the
final model needs to fit in under 16MB of flash (i.e. under 4 million
parameters total).  Once a `model.bin` has been generated, convert it
to a header and make it constant and 4-byte aligned:
````
xxd -i model.bin | sed 's/unsigned/const unsigned/' | sed 's/\[\]/[] __attribute__((aligned((4))))/' >  model.h
````

Note that while the models are run unmodified from llama2.c, the tokenizer
file has a minor change (0-terminating token strings so we don't need to
copy them into RAM) which required the following C file to be run on the
`tokenizer.bin` file to generate `tokenizer0.bin` (which then is processed
with the same commands as the model.bin file above)
````
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *in = fopen("tokenizer.bin", "rb");
    FILE *out = fopen("tokenizer0.bin", "wb");
    int i;
    float f;
    char buff[128];

    // max_token_length
    fread(&i, sizeof(int), 1, in);
    fwrite(&i, sizeof(int), 1, out);

    for (int a = 0; a < 32000; a++) {
        // vocab_scores[a]
        fread(&f, sizeof(float), 1, in);
        fwrite(&f, sizeof(float), 1, out);
        // len[a]
        fread(&i, sizeof(int), 1, in);
        fwrite(&i, sizeof(int), 1, out);
        // vocab[a] + \0
        fread(buff, i, 1, in);
        buff[i] = 0;
        fwrite(buff, i + 1, 1, out);
    }

    fclose(in);
    fclose(out);
}
````

## Speed and optimizations

On the included 14MB model each token (part of a word) takes around
850ms to generate, so it's not fast.  While I haven't looked at it
too closely, my feeling is that the QSPI bandwidth and latency during
the matrix operations is the limiting factor.

- Earle F. Philhower, III <earlephilhower@yahoo.com>
