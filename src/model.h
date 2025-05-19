#include <Arduino.h>

void llama2_set_model(const char *model, uint32_t len);
void llama2_set_tokenizer(const char *token);
void llama2_set_temperature(float f);
void llama2_set_topp(float f);
void llama2_set_steps(int s);
void llama2_set_prompt(const char *s);
void llama2_set_seed(unsigned long long l);
void llama2_set_output(void (*o)(const char *));
void llama2_init();
void llama2_generate();
void llama2_end();
