# mruby-signal

[![Build Status](https://travis-ci.org/ksss/mruby-signal.svg?branch=master)](https://travis-ci.org/ksss/mruby-signal)

Signal trap and callback in mruby

## Synopsis

```ruby
Signal.trap(:INT) { |signo|
  p "hello signal"
}
```

## Methods

* Signal.trap(sig, command){|signo| block }
* Signal.list
* Signal.signame(signo)

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

## Based on

[https://github.com/ruby/ruby/blob/trunk/signal.c](https://github.com/ruby/ruby/blob/trunk/signal.c)

## Passed the [ruby/spec](https://github.com/ruby/spec)

mruby-signal passed the below specs in ruby/spec

- [core/signal/*](https://github.com/ruby/spec/tree/master/core/signal)
- [core/exception/interrupt_spec.rb](https://github.com/ruby/spec/blob/master/core/exception/interrupt_spec.rb)
- [core/exception/signal_exception_spec.rb](https://github.com/ruby/spec/blob/master/core/exception/signal_exception_spec.rb)

## See also

[http://ruby-doc.org/core-2.3.1/Signal.html](http://ruby-doc.org/core-2.3.1/Signal.html)
