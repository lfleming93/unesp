def soma_vetor(slice)
  puts "Starting thread #{Thread.current.object_id}\n"

  soma = 0
  slice.each do |num|
    soma += num
  end

  puts "Done with thread #{Thread.current.object_id}\n"
  Thread.current[:soma] = soma
end

vector = (1..2_000_000)

chunk_size = (ARGV[0].nil?) ? 500_000 : ARGV[0].to_i
threads = []
started = Time.now

vector.each_slice(chunk_size) do |slice|
  threads << Thread.new { soma_vetor(slice) }
end

soma = 0
threads.each do |t|
  t.join
  soma += t[:soma]
end
ended = Time.now
total_time = (ended - started)

puts "Soma: #{soma}"
puts "Time: #{total_time}"
puts "Threads: #{threads.count}"

puts ""
puts ""
puts ""

soma = 0
started = Time.now

vector.each do |num|
  soma += num
end

ended = Time.now
total_time = (ended - started)

puts "Soma: #{soma}"
puts "Time: #{total_time}"

# puts ""
# puts ""
# puts "Inject"

# soma = 0
# started = Time.now

# soma = vector.inject(:+)

# ended = Time.now
# total_time = (ended - started)

# puts "Soma: #{soma}"
# puts "Time: #{total_time}"

