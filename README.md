# mruby-signal

Signal trap and callback in mruby

## Synopsis

```ruby
Signal.trap(:INT) {
  p "hello signal"
}
```

## Methods

Signal.trap(sig, command){ block }
Signal.list
Signal.signame(signm)

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
