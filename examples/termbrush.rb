#!/usr/bin/env ruby
termwidth = `tput cols`.to_i

def normalize min, max, value, normalized_max
  value = min if value < min
  value = max if value > max
  normalized_max  * (value - min) / (max - min)
end

ARGF.each do |line|
  next if line.match /^#/ 
  x, y, r = line.split(/\s+/).map{|v| v.split(':').last.to_i}
  left = 100 - x
  left_new = normalize(0, 100, left, termwidth)
  color0 = 16
  color = color0 + normalize(10, 60, r, 230 - color0)

  len = case true
  when r < 10
    1
  else
    (5 * r/10.0).to_i
  end

  brush = " " * len
  brush = "\033[7m\033[38;5;#{color}m#{brush}"

  leftmargin = (left_new - len/2.0).to_i
  leftmargin = 0 if leftmargin < 0

  toprint = ' ' * leftmargin + brush

  if left == -1
    puts
  else
    print toprint[0, toprint.length + termwidth - (leftmargin + len) - 1] + "\033[0;00m\n"
  end

end
