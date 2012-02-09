class PfSense
  
  # Under pfsense 1.2 we can GET /ifstats.php?if=vr0 without authentication
  # Under pfsense 2.0 we need to login first.

  def initialize(host)
    @host = host
    @curl = Curl::Easy.new()
    @curl.enable_cookies = true
    @curl.follow_location = false
    @last = {}
  end

  def login(username, password)
    @curl.url = "http://#{@host}/"
     data = {
      'usernamefld'=>username,
      'passwordfld'=>password,
      'login'=>'Login'}
    urlData = data.collect{|key,value| Curl::PostField.content(key,value)}
    @curl.http_post(urlData)
    # pfsense http response code is 200 for failure, 302 for success
    raise "pfSense authentication failed for #{username}@#{@host}" unless @curl.response_code == 302
  end

  def read(interface)
    @curl.url = "http://#{@host}/ifstats.php?if=#{interface}"
    @curl.http_get
    if @curl.response_code == 200
      time, bytes_in, bytes_out = @curl.body_str.strip.split("|").map{|n| n.to_f}
      last_time, last_bytes_in, last_bytes_out = @last[interface]
      if last_time
        delta_time = time - last_time
        bps_in = (bytes_in-last_bytes_in)/delta_time
        bps_out = (bytes_out-last_bytes_out)/delta_time
      else
        bps_in = nil
        bps_out = nil
      end
      @last[interface] = [time, bytes_in, bytes_out]
      [bps_in, bps_out]
    end
  end
end

