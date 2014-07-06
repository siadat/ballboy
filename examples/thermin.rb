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

def play note, vel, duration
  puts "playing:%d" % note
  @output.puts(0x90, note, vel)
  sleep duration
  @output.puts(0x80, note, vel)
end

Thread.new {
  @output = UniMIDI::Output.use(:first)
  while true
    @beat += 1
    Thread.new {
      if @last_note != @next_note && @next_note != -1
        @last_note = @next_note
        next_note = @next_note
        puts "playing note:%d vel:%d duration:%d" % [note, @next_vel, @duration]
        play(next_note, @next_vel, @duration)
      else
        duration = 0.8
        note = @base_note
        vel = 10
        if @beat % @bar_a == 0
          if @beat % @bar_b == 0
            note = @base_note - 12
            vel = 30
            duration = 1.5
          else
            vel = 30.0/(@bar_b / @bar_a) * (@beat % @bar_b) / @bar_a
          end
        else
          vel = 0
        end
        play(note, @vel * vel, duration)
      end
    }

    sleep 1 / (@tempo/60.0)
  end
}

ARGF.each do |line|
  Thread.new {
    @last_played = Time.now
    values = line.split(/\s+/)
    if values.length == 3
      x, y, r = values.map{|v| v.split(':').last.to_i}
      i = (y/80.0).to_i
      @next_vel = 30 + (x / 10).to_i
      @next_note = 70 - ((i/@key.length) * 12 + @key[i % @key.length]) + 12
    end
  }
end
