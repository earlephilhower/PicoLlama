// Silly wrapper for llama2.c calls
// MIT licensed

#include <Arduino.h>
#include "model.h"

#ifndef RP2350_PSRAM_CS
#error This library needs PSRAM to function
#endif

class PicoLlama {
public:
    PicoLlama() {
    }

    void setOutputCB(void (*cb)(const char *)) {
        llama2_set_output(cb);
    }

    void setModel(const void *model, size_t len, const void *tokenizer) {
        llama2_set_model((const char *)model, len);
        llama2_set_tokenizer((const char *)tokenizer);
    }

    void setTemperature(float f) {
        llama2_set_temperature(f);
    }

    void setSeed(uint32_t s) {
        llama2_set_seed(s);
    }

    void generate(const char *prompt = "", unsigned int steps = 0) {
        llama2_set_prompt(prompt);
        llama2_set_steps(steps);
        llama2_init();
        llama2_generate();
        llama2_end();
    }
};
