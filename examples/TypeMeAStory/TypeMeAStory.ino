#include <PicoLlama.h>
#include <tinystories14M.bin.h>
#include <tokenizer0.bin.h>
#include <Keyboard.h>

PicoLlama cria; // Cria is a baby llama, look it up!

void out(const char *str) {
  if (str) {
    Keyboard.print(str);
  } else {
    // NULL == EOF
  }
}


void setup() {
  delay(3000);
  cria.setModel(model_bin, sizeof(model_bin), tokenizer0_bin);
  cria.setOutputCB(out);
  Keyboard.begin();
  Serial.println("Press BOOTSEL to type a story");
}

void loop() {
  if (BOOTSEL) {
    cria.setSeed(millis());
    cria.generate();
    while (BOOTSEL) {
      /* wait for release */
    }
  }
}
