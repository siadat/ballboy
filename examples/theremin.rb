#!/usr/bin/env ruby
require 'unimidi'

@last_played = Time.now - 2
@last_note = -1
@next_note = -1
@base_note = 61

@key = [0, 2, 4, 5, 7, 9, 11]
@tempo = 200
@beat = 0
@vel = 3
@bar_a, @bar_b = 3, 12
@duration = 1
@output = UniMIDI::Output.use(:first)

def play note, vel, duration
  @output.puts(0x90, note, vel)
  sleep duration
  @output.puts(0x80, note, vel)
end


ARGF.each do |line|
  next if line.match /^#/
  Thread.new {
    @last_played = Time.now
    values = line.split(/\s+/)
    if values.length == 3
      x, y, r = values.map{|v| v.split(':').last.to_i}
      i = (x/2.0).to_i
      @next_vel = 30 + (y / 10.0).to_i
      @next_note = 80 - ((i/@key.length) * 12 + @key[i % @key.length]) + 12
      puts "playing note:%d vel:%d duration:%d" % [@next_note, @next_vel, @duration]
      play(@next_note, @next_vel, @duration)
    end
  }
end
