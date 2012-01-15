class Database

  SCHEMA = 
    [
     {
       :table => :flow,
       :fields => [
                   {:name => 'time', :type => 'integer'},
                   {:name => 'bps_in', :type => 'integer'},
                   {:name => 'bps_out', :type => 'integer'}
                  ]
     }
    ]
  
  def initialize(filename)
    @filename = filename
    if !File.exists?(@filename)
      create
    else
      @db = SQLite3::Database.new(@filename)
    end
  end
    
  def create
    
    File.unlink(@filename) if File.exists?(@filename)
    @db = SQLite3::Database.new(@filename)
    SCHEMA.each do |table_def|
      table_name = table_def[:table]
      indexes = []
      fields = []

      # generate table definition
      table_def[:fields].each do |field_def|
        fields << "#{field_def[:name]} #{field_def[:type]}"

        # add single field indexes
        if field_def[:indexed]
          indexes << "#{table_name}_#{field_def[:name]}_idx ON #{table_name}(#{field_def[:name]})"
        end
      end

      # add explicit indexes
      if table_def[:indexes]
        table_def[:indexes].each do |index_def|
          name = index_def[:name] || [table_name,index_def[:fields],'idx'].join("_")
          indexes << "#{name} ON #{table_name}(#{index_def[:fields].join(",")})"
        end
      end
      
      # create the table
      sql = "create table #{table_name} (#{fields.join(",")})"
      puts sql
      @db.execute(sql)

      # create indexes
      indexes.each do |index_def|
        sql = "create index #{index_def}"
        puts sql
        @db.execute(sql)
      end
    end
  end    

  def write(time, bps_in, bps_out)
    @db.execute("insert into flow values (?,?,?)", time, bps_in, bps_out)
  end

end  

