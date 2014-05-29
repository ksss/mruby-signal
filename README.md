# mruby-signal

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

## See also

[http://ruby-doc.org/core-2.1.0/Signal.html](http://ruby-doc.org/core-2.1.0/Signal.html)
