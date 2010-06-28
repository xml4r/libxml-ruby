require File.join(File.dirname(__FILE__), "setup.rb")

# Run memory tests
# TODO: Ensure compile first (but only if needed) with make?

desc "Run memory leak test."
task :memtest do

  live = ENV['live']

  unless live
    $LOAD_PATH.unshift(File.expand_path('ext/libxml'))
    $LOAD_PATH.unshift(File.expand_path('lib'))
  end
  $LOAD_PATH.unshift('test')  # NEEDED?

  glob = 'test/ets_*.rb'

  Dir[glob].each do |fn|
    next if File.directory?(fn)
    puts "test #{fn}"
    f=Process.fork
    if f.nil?
      require fn
      exit
    end
    rss_k=0
    while (px=Process.wait2(f,Process::WNOHANG)).nil?
      sleep 2
      rss_k2=`ps -o rss= -p #{f}`.to_i
      rss_k=(rss_k2>rss_k) ? rss_k2 : rss_k
    end
    pid,status=px
    puts "\nProcess #{pid} exited status #{status.exitstatus}, max rss(k) #{rss_k}"
  end

end

