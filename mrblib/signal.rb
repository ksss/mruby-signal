module Signal
  def self.mutex
    @m ||= Mutex.new
  end

  def self.mrb_state_lock
    mutex.lock
    self.lock
    mutex.unlock
  end

  def self.mrb_state_unlock
    mutex.lock
    self.unlock
    mutex.unlock
  end
end
