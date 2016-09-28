assert 'Kernel#trap' do
  block = Proc.new {}
  assert_equal "DEFAULT", trap(:HUP, :SIG_DFL)
  assert_equal "DEFAULT", trap(:HUP, &block)
  assert_equal block, trap(:HUP, block)
  assert_raise(ArgumentError) { trap(:HUP) }
  assert_equal block, trap(:HUP, :SIG_DFL)
end

assert 'Signal#trap' do
  a = []
  pr = Proc.new{ a << :proc_trap }
  saved_trap = Signal.trap(:HUP, pr)

  assert_equal pr, Signal.trap(:HUP, saved_trap)
  Signal.trap :HUP, :SIG_DFL
  assert_equal "DEFAULT", Signal.trap(:HUP, :SIG_DFL)

  Signal.trap :SIGHUP, pr
  assert_equal pr, Signal.trap(:HUP, saved_trap)

  assert_nil Signal.trap(0){}
  Signal.trap(0, "SIG_DFL")
  assert_nil Signal.trap("EXIT"){}
  assert_raise(ArgumentError){ Signal.trap -1 }
end

assert "Signal#list" do
  list = Signal.list
  assert_kind_of Hash, list
  assert_true 0 < list.length
end

assert "Signal#signame" do
  assert_equal "HUP", Signal.signame(1)
  assert_equal "INT", Signal.signame(2)
  assert_equal "QUIT", Signal.signame(3)
  assert_equal nil, Signal.signame(-1)
end
