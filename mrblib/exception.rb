class SignalException < Exception
  def initialize(message, signo)
    super(message)
    @signo = signo
  end

  def signo
    @signo
  end
end

class Interrupt < SignalException
end
