class SignalException < Exception
  def initialize(message, signo)
    super(message)
    @signo = signo
  end
  attr_reader :signo
  alias signm message
end

class Interrupt < SignalException
end
