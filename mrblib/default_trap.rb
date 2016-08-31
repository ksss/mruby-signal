trap(:INT) do
  raise Interrupt
end
