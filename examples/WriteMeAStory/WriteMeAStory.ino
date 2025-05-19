#include <PicoLlama.h>
#include <tinystories14M.bin.h>
#include <tokenizer0.bin.h>

PicoLlama cria; // Cria is a baby llama, look it up!

void out(const char *str) {
  if (str) {
    Serial.print(str);
  } else {
    // NULL == EOF
    Serial.print("\r\n\r\n");
  }
}

void setup() {
  delay(3000);
  cria.setModel(model_bin, sizeof(model_bin), tokenizer0_bin);
  cria.setOutputCB(out);
  cria.setSeed(rp2040.hwrand32());
  Serial.println("I want to write a story about a cookie...");
  cria.generate("Lily loved her cookie");
}

void loop() {
  Serial.setTimeout(10000);
  Serial.println("\r\n\r\nEnter your own prompt and I'll write a story using it:");
  cria.setSeed(rp2040.hwrand32());
  String s = Serial.readStringUntil('\n');
  cria.generate(s.c_str());
}
