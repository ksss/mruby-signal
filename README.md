# mruby-signal

Signal trap and callback in mruby

## Synopsis

```ruby
Signal.trap(:INT) {
  p "hello signal"
}
p Signal.list #=> {"EXIT"=>0, "HUP"=>1, "INT"=>2, "QUIT"=>3, "ILL"=>4, "TRAP"=>5, "ABRT"=>6, "IOT"=>6, "EMT"=>7, "FPE"=>8, "KILL"=>9, "BUS"=>10, "SEGV"=>11, "SYS"=>12, "PIPE"=>13, "ALRM"=>14, "TERM"=>15, "URG"=>16, "STOP"=>17, "TSTP"=>18, "CONT"=>19, "CHLD"=>20, "CLD"=>20, "TTIN"=>21, "TTOU"=>22, "IO"=>23, "XCPU"=>24, "XFSZ"=>25, "VTALRM"=>26, "PROF"=>27, "WINCH"=>28, "USR1"=>30, "USR2"=>31, "INFO"=>29}
```

## Methods

* Signal.trap(sig, command){ block }
* Signal.list
* Signal.signame(signm)

## Installation

### use github repository

Write in /mruby/build_config.rb

```ruby
MRuby::Build.new do |conf|
  conf.gem :github => 'ksss/mruby-signal', :branch => 'master'
end
```

## License

MIT

## See also

[http://ruby-doc.org/core-2.1.0/Signal.html](http://ruby-doc.org/core-2.1.0/Signal.html)
