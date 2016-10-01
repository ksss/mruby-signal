class SignalException < Exception
  attr_reader :signo
  alias signm message

  # def initialize(message, signo)
  #   super(message)
  #   @signo = signo
  # end
  # call-seq:
  #    SignalException.new(sig_name)              ->  signal_exception
  #    SignalException.new(sig_number [, name])   ->  signal_exception
  #
  #  Construct a new SignalException object.  +sig_name+ should be a known
  #  signal name.
  def initialize(nm_or_no, name = nil)
    sig = nil

    if nm_or_no.respond_to?(:to_int)
      nm_or_no = nm_or_no.to_int
    end

    case nm_or_no
    when Integer
      signo = nm_or_no
      signame = Signal.signame(nm_or_no)
      if signame.nil?
        raise ArgumentError, "invalid signal number (#{nm_or_no})"
      end
      sig = if name
        name
      else
        "SIG#{signame}"
      end
    when String, Symbol
      if name
        raise ArgumentError, "wrong number of arguments (given 2, expected 1)"
      end
      signame = nm_or_no.to_s
      if signame[0, 3] == "SIG"
        signame = signame[3..-1]
      end
      signo = Signal.list[signame]
      if signo.nil?
        raise ArgumentError, "unsupported name `#{nm_or_no}'"
      end
      sig = "SIG#{signame}"
    else
      raise TypeError, "unsupported type `#{nm_or_no.class}'"
    end

    super(sig)
    @signo = signo
  end
end

class Interrupt < SignalException
  def initialize(message = "Interrupt")
    super(Signal.list["INT"], message)
  end
end
