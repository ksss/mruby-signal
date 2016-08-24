/**
 * signal.c
 * This code was based on https://github.com/ruby/ruby/blob/trunk/signal.c
 */

#include <mruby.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <mruby/class.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#if defined(__BEOS__) || defined(__HAIKU__)
# undef SIGBUS
#endif

#ifndef NSIG
# define NSIG (_SIGMAX + 1)      /* For QNX */
#endif

#define sighandler_t void*

#define RETSIGTYPE void

static const struct signals {
  const char *signm;
  int signo;
} siglist [] = {
  {"EXIT", 0},
#ifdef SIGHUP
  {"HUP", SIGHUP},
#endif
  {"INT", SIGINT},
#ifdef SIGQUIT
  {"QUIT", SIGQUIT},
#endif
#ifdef SIGILL
  {"ILL", SIGILL},
#endif
#ifdef SIGTRAP
  {"TRAP", SIGTRAP},
#endif
#ifdef SIGABRT
  {"ABRT", SIGABRT},
#endif
#ifdef SIGIOT
  {"IOT", SIGIOT},
#endif
#ifdef SIGEMT
  {"EMT", SIGEMT},
#endif
#ifdef SIGFPE
  {"FPE", SIGFPE},
#endif
#ifdef SIGKILL
  {"KILL", SIGKILL},
#endif
#ifdef SIGBUS
  {"BUS", SIGBUS},
#endif
#ifdef SIGSEGV
  {"SEGV", SIGSEGV},
#endif
#ifdef SIGSYS
  {"SYS", SIGSYS},
#endif
#ifdef SIGPIPE
  {"PIPE", SIGPIPE},
#endif
#ifdef SIGALRM
  {"ALRM", SIGALRM},
#endif
#ifdef SIGTERM
  {"TERM", SIGTERM},
#endif
#ifdef SIGURG
  {"URG", SIGURG},
#endif
#ifdef SIGSTOP
  {"STOP", SIGSTOP},
#endif
#ifdef SIGTSTP
  {"TSTP", SIGTSTP},
#endif
#ifdef SIGCONT
  {"CONT", SIGCONT},
#endif
#ifdef SIGCHLD
  {"CHLD", SIGCHLD},
#endif
#ifdef SIGCLD
  {"CLD", SIGCLD},
#else
# ifdef SIGCHLD
  {"CLD", SIGCHLD},
# endif
#endif
#ifdef SIGTTIN
  {"TTIN", SIGTTIN},
#endif
#ifdef SIGTTOU
  {"TTOU", SIGTTOU},
#endif
#ifdef SIGIO
  {"IO", SIGIO},
#endif
#ifdef SIGXCPU
  {"XCPU", SIGXCPU},
#endif
#ifdef SIGXFSZ
  {"XFSZ", SIGXFSZ},
#endif
#ifdef SIGVTALRM
  {"VTALRM", SIGVTALRM},
#endif
#ifdef SIGPROF
  {"PROF", SIGPROF},
#endif
#ifdef SIGWINCH
  {"WINCH", SIGWINCH},
#endif
#ifdef SIGUSR1
  {"USR1", SIGUSR1},
#endif
#ifdef SIGUSR2
  {"USR2", SIGUSR2},
#endif
#ifdef SIGLOST
  {"LOST", SIGLOST},
#endif
#ifdef SIGMSG
  {"MSG", SIGMSG},
#endif
#ifdef SIGPWR
  {"PWR", SIGPWR},
#endif
#ifdef SIGPOLL
  {"POLL", SIGPOLL},
#endif
#ifdef SIGDANGER
  {"DANGER", SIGDANGER},
#endif
#ifdef SIGMIGRATE
  {"MIGRATE", SIGMIGRATE},
#endif
#ifdef SIGPRE
  {"PRE", SIGPRE},
#endif
#ifdef SIGGRANT
  {"GRANT", SIGGRANT},
#endif
#ifdef SIGRETRACT
  {"RETRACT", SIGRETRACT},
#endif
#ifdef SIGSOUND
  {"SOUND", SIGSOUND},
#endif
#ifdef SIGINFO
  {"INFO", SIGINFO},
#endif
  {NULL, 0}
};

static mrb_state *global_mrb;

static mrb_bool
reserved_signal_p(int signo)
{
#ifdef SIGSEGV
  if (signo == SIGSEGV)
    return 1;
#endif
#ifdef SIGBUS
  if (signo == SIGBUS)
    return 1;
#endif
#ifdef SIGILL
  if (signo == SIGILL)
    return 1;
#endif
#ifdef SIGFPE
  if (signo == SIGFPE)
    return 1;
#endif

#ifdef SIGVTALRM
  if (signo == SIGVTALRM)
    return 1;
#endif

  return 0;
}

static const char*
signo2signm(mrb_int no)
{
  const struct signals *sigs;

  for (sigs = siglist; sigs->signm; sigs++) {
    if (sigs->signo == no)
      return sigs->signm;
  }
  return 0;
}

static int
signm2signo(const char *nm)
{
  const struct signals *sigs;

  for (sigs = siglist; sigs->signm; sigs++) {
    if (strcmp(sigs->signm, nm) == 0)
      return sigs->signo;
  }
  return 0;
}

static int
trap_signm(mrb_state *mrb, mrb_value vsig)
{
  int sig = -1;
  const char *s;

  switch (mrb_type(vsig)) {
    case MRB_TT_FIXNUM:
      sig = mrb_fixnum(vsig);
      if (sig < 0 || sig >= NSIG) {
        mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid signal number (%S)", vsig);
      }
      break;
    case MRB_TT_SYMBOL:
      s = mrb_sym2name(mrb, mrb_symbol(vsig));
      if (!s) mrb_raise(mrb, E_ARGUMENT_ERROR, "bad signal");
      goto str_signal;
    default:
      s = RSTRING_PTR(vsig);

str_signal:
      if (memcmp("SIG", s, 3) == 0)
        s += 3;
      sig = signm2signo(s);
      if (sig == 0 && strcmp(s, "EXIT") != 0)
        mrb_raise(mrb, E_ARGUMENT_ERROR, "unsupported signal");
      break;
  }
  return sig;
}

static void
mrb_signal_exec(int sig)
{
  mrb_state *mrb = global_mrb;
  struct RClass *mrb_mSignal = mrb_module_get(mrb, "Signal");

  mrb_value trap_list = mrb_iv_get(mrb, mrb_obj_value(mrb_mSignal), mrb_intern_lit(mrb, "trap_list"));
  mrb_value command = mrb_ary_ref(mrb, trap_list, sig);
  mrb_funcall(mrb, command, "call", 1, mrb_fixnum_value(sig));
}

static RETSIGTYPE
sighandler(int sig)
{
  mrb_signal_exec(sig);
}

static sighandler_t
trap_handler(mrb_state *mrb, mrb_value *cmd, int sig)
{
  sighandler_t func = sighandler;
  mrb_value command;

  if (mrb_nil_p(*cmd)) {
    func = SIG_IGN;
  }
  else {
    command = *cmd;
    if (mrb_type(command) == MRB_TT_SYMBOL) {
      command = mrb_sym2str(mrb, mrb_symbol(command));
      if (mrb_undef_p(command))
        mrb_raise(mrb, E_ARGUMENT_ERROR, "bad handler");
    }
    if (!mrb_nil_p(command)) {
      *cmd = command;
      switch (RSTRING_LEN(command)) {
        case 7:
          if (memcmp(RSTRING_PTR(command), "SIG_IGN", 7) == 0) {
            func = SIG_IGN;
            *cmd = mrb_nil_value();
          }
          else if (memcmp(RSTRING_PTR(command), "SIG_DFL", 7) == 0) {
            func = SIG_DFL;
            *cmd = mrb_nil_value();
          }
          break;
        case 4:
          if (memcmp(RSTRING_PTR(command), "EXIT", 4) == 0) {
            *cmd = mrb_nil_value();
          }
          break;
      }
    }
  }
  return func;
}

static sighandler_t
mrb_signal(mrb_state *mrb, int signum, sighandler_t handler)
{
  struct sigaction sigact, old;

  global_mrb = mrb;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_handler = handler;
  sigact.sa_flags = 0;

  if (sigaction(signum, &sigact, &old) < 0) {
    if (errno != 0 && errno != EINVAL) {
      mrb_bug(mrb, "sigaction %S", mrb_fixnum_value(errno));
    }
  }
  return old.sa_handler;
}

static mrb_value
trap(mrb_state *mrb, mrb_value mod, int sig, sighandler_t func, mrb_value command)
{
  sighandler_t oldfunc;
  mrb_value oldcmd;
  mrb_value trap_list;
  mrb_sym id_trap_list;

  id_trap_list = mrb_intern_lit(mrb, "trap_list");
  trap_list = mrb_iv_get(mrb, mod, id_trap_list);
  if (mrb_nil_p(trap_list)) {
    trap_list = mrb_ary_new_capa(mrb, NSIG);
  }
  oldfunc = mrb_signal(mrb, sig, func);
  oldcmd = mrb_ary_ref(mrb, trap_list, (mrb_int)sig);

  if (mrb_nil_p(oldcmd)) {
    if (oldfunc == SIG_IGN)
      oldcmd = mrb_str_new_cstr(mrb, "IGNORE");
    else if (oldfunc == SIG_DFL)
      oldcmd = mrb_str_new_cstr(mrb, "DEFAULT");
    else
      oldcmd = mrb_nil_value();
  }
  else if (mrb_undef_p(oldcmd)) {
    oldcmd = mrb_str_new_cstr(mrb, "EXIT");
  }

  mrb_ary_set(mrb, trap_list, (mrb_int)sig, command);
  mrb_iv_set(mrb, mod, id_trap_list, trap_list);

  return oldcmd;
}


static mrb_value
signal_trap(mrb_state *mrb, mrb_value mod)
{
  mrb_value command, block;
  int sig;
  mrb_value *argv;
  mrb_int argc;
  sighandler_t func;
  struct RClass *mrb_mSignal = mrb_module_get(mrb, "Signal");

  mrb_get_args(mrb, "*&", &argv, &argc, &block);

  if (argc != 1 && argc != 2)
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "wrong number of arguments (1..2 for %S)", mrb_fixnum_value(argc));

  sig = trap_signm(mrb, argv[0]);
  if (reserved_signal_p(sig)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "can't trap reserved signal");
  }

  if (argc == 1) {
    command = block;
    func = sighandler;
  }
  else {
    command = argv[1];
    func = trap_handler(mrb, &command, sig);
  }

  return trap(mrb, mrb_obj_value(mrb_mSignal), sig, func, command);
}

static mrb_value
signal_list(mrb_state *mrb, mrb_value mod)
{
  mrb_value h = mrb_hash_new(mrb);
  const struct signals *sigs;

  for (sigs = siglist; sigs->signm; sigs++) {
    mrb_hash_set(mrb, h, mrb_str_new_cstr(mrb, sigs->signm), mrb_fixnum_value(sigs->signo));
  }
  return h;
}

static mrb_value
signal_signame(mrb_state *mrb, mrb_value mod)
{
  mrb_int signo;

  mrb_get_args(mrb, "i", &signo);
  return mrb_str_new_cstr(mrb, signo2signm(signo));
}

static void
mrb_trap_exit(mrb_state *mrb)
{
  struct RClass *mrb_mSignal = mrb_module_get(mrb, "Signal");
  mrb_value trap_list = mrb_iv_get(mrb, mrb_obj_value(mrb_mSignal), mrb_intern_lit(mrb, "trap_list"));
  mrb_value command;
  if (mrb_nil_p(trap_list)) {
    return;
  }

  command = mrb_ary_ref(mrb, trap_list, 0);
  if (!mrb_nil_p(command)) {
    mrb_ary_set(mrb, trap_list, 0, mrb_nil_value());
    mrb_funcall(mrb, command, "call", 1, mrb_fixnum_value(0));
  }
}

void
mrb_mruby_signal_gem_init(mrb_state* mrb) {
  struct RClass *signal = mrb_define_module(mrb, "Signal");

  mrb_define_class_method(mrb, signal, "trap", signal_trap, MRB_ARGS_ANY());
  mrb_define_class_method(mrb, signal, "list", signal_list, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, signal, "signame", signal_signame, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, mrb->kernel_module, "trap", signal_trap, MRB_ARGS_ANY());
}

void
mrb_mruby_signal_gem_final(mrb_state* mrb) {
  mrb_trap_exit(mrb);
}
