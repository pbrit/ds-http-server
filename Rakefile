#dnl ACLOCAL_AMFLAGS = -I .
#bin_PROGRAMS = hw
#hw_SOURCES = src/main.cpp src/server.cpp src/session.cpp 

require 'awesome_print'

# Configure

INCLUDE_FLAGS = '-I./dependencies/http-parser'
LIBRARY_FLAGS = '-L./dependencies/http-parser -lhttp_parser'

INPUT_SRC = FileList['src/*.cpp'].to_a

INPUT_OBJECT = INPUT_SRC.map { |file| file.gsub(/src/,'out').gsub(/cpp/,'o') }

# End of configure

file 'rserver' => INPUT_OBJECT do |task|

	#ap task

	sh <<-SH
		g++ -o #{task.name} #{LIBRARY_FLAGS} #{task.prerequisites.join ' '}
	SH
end

INPUT_SRC.each do |src_file|
	
	file src_file.gsub(/src/,'out').gsub(/cpp/,'o') => src_file do |task|
		
		#Rake::Task['utils.make_out'].execute

		sh <<-SH
			g++ #{INCLUDE_FLAGS} -c #{task.prerequisites.join ''} -o #{task.name}
		SH
	end
end

=begin
namespace 'utils' do
	task 'make_out' do 
		require 'fileutils'

		FileUtils.mkdir_p 'out'

	end
end
=end


