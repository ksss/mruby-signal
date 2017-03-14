#include <mruby.h>

extern mrb_state *mruby_signal_mrb;

void
mrb_mruby_signal_gem_test(mrb_state *mrb)
{
  mruby_signal_mrb = mrb;
}
