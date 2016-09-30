class SignalException < Exception
  def initialize(message, signo)
    super(message)
    @signo = signo
  end
  alias signm message

  def signo
    @signo
  end
end

class Interrupt < SignalException
end
