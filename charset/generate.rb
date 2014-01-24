charsetName = ARGV[0]

charset = []

# read all files in charset directory
Dir[charsetName + '/*.txt'].each do |fname|
    # get character index from filename
    index = Integer(fname.gsub(charsetName + '/', '0x').gsub('.txt', ''));
        
    # read file
    file = File.new(fname)
    
    width = 0    
    bit = 0
	char = []
    while (line = file.gets)
    	line.strip!
		if line != '' and line.slice(0, 2) != '//'
			#
			if bit == 0
				width = line.size
				char = Array.new(width, 0)
			end

			# 
			width.times do |i|
				if line[i] != '.'
					char[i] = char[i] | (1 << bit)
				end
			end
			
			bit += 1
		end
    end
    file.close
    
    # store character
    if width > 0
    	charset[index] = char
    end
end

offsets = []
offset = 0

#
puts 'uint8_t charset_char[] = {'
for char in 0..127 do 
	if charset[char]
		print "// %x - " % char
		puts char.chr
		offsets[char] = offset
		charset[char].each do |col|
			puts "0b%08b," % col
			offset += 1
		end
	end
end
puts '};'
puts ''

puts 'uint8_t charset_width[] = {'
for char in 0..127 do 
	if charset[char]
		print charset[char].size.to_s
	else
		print 0
	end
	if char < 127
		print ","
	end
end
puts '};'
puts ''

puts 'uint16_t charset_offset[] = {'
for char in 0..127 do 
	if offsets[char]
		print offsets[char].to_s
	else
		print 0
	end
	if char < 127
		print ","
	end
end
puts '};'