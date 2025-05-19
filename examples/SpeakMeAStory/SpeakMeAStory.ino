// This sketch requires BackgroundAudio, see https://github.com/earlephilhower/BackgroundAudio

#include <PicoLlama.h>
#include <tinystories14M.bin.h>
#include <tokenizer0.bin.h>
#include <BackgroundAudioSpeech.h>
#include <libespeak-ng/voice/en.h>
#include <PWMAudio.h>

PicoLlama cria; // Cria is a baby llama, look it up!

// PWMAudio or I2S can be used below.  With PWMAudio a simple 3.5MM jack can be wired to the pin given and GND to get output.  See BackgroundAudio or the Arduino-Pico core documentation for more info.
PWMAudio audio(4);
BackgroundAudioSpeech BMP(audio);

// Because the tokens are partial words (and may in fact be bits of multiple words)
// we can't just speak each call.  So, we collect all the outputs and then speak it
// when we get a NULL (EOF) message.

char msg[8192];
char *ptr = msg;

void out(const char *str) {
  // We should readlly check that str can fit in remaining space, but in practice all outputs easily fit within this space.
  if (str) {
    strcpy(ptr, str);
    ptr += strlen(str);
    
    Serial.print(str); // As we think it, dump it to USB
  } else {
    // NULL == EOF
    Serial.println();
    BMP.speak(msg);
    ptr = msg;
  }
}
void setup() {
  delay(3000);
  cria.setModel(model_bin, sizeof(model_bin), tokenizer0_bin);
  cria.setOutputCB(out);
  cria.setSeed(rp2040.hwrand32());
  
  // Start the background player
  BMP.setVoice(voice_en);
  BMP.setRate(80);
  BMP.begin();
  BMP.speak("I am thinking of a story, give me a minute please.");
}

void loop() {
  cria.setSeed(rp2040.hwrand32());
  cria.generate();
}
