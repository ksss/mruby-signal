Signal.list.each do |sig, no|
  case sig
  when "INT"
    Signal.trap(sig) { raise Interrupt, "" }
  when "HUP", "TERM", "QUIT", "ALRM", "USR1", "USR2"
    Signal.trap(sig) { raise SignalException.new("SIG#{sig}", no) }
  end
end
