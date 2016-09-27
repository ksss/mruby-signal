Signal.list.each do |sig, no|
  case sig
  when "INT"
    Signal.trap(sig) { raise Interrupt, "" }
  end
end
