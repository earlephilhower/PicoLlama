// Generated using the command line:
// $ ./torch/bin/python3 -m train.py --compile=False --dim=96 --n_layers=5 --n_heads=6 --n_kv_heads=6 --eval_iters=10 --learning_rate=1e-3 --max_seq_len=256 --eval_interval=100 --batch_size=16
// ...12 hours later...
// $ xxd -i model.bin | sed 's/unsigned/const unsigned/' | sed 's/\[\]/[] __attribute__((aligned((4))))/' >  model.h

// Split into 2 files due to GitHub size limitations.  Your own models would not do this
#include "tinystories14M.bin.a.h"
#include "tinystories14M.bin.b.h"
