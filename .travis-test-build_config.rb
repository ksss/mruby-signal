MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gem :core => "mruby-bin-mruby"
  conf.gem :core => "mruby-print"
end

MRuby::Build.new('test') do |conf|
  toolchain :gcc

  enable_debug
  conf.enable_bintest
  conf.enable_test

  conf.gem :core => "mruby-print"
  conf.gem "../mruby-signal"
end
