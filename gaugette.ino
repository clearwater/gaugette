#define CMDBUF_LEN 40
#define TUPLE_LEN 3
#define value_t unsigned int

void setup(void) {
  Serial.begin(9600);
  Serial.println("Gaugette!");
}

void skip_whitespace(char **line, int *len)
{
  while (*len > 0 && **line == ' ') {
    (*len)--;
    (*line)++;
  }
}

boolean parse_command(char **line, int *len, char *cmd)
{
  boolean result = false;
  skip_whitespace(line, len);
  if (*len > 0) {
    *cmd = **line;
    (*len)--;
    (*line)++;
    result = true;
  }
  return result;
}

boolean parse_integer(char **line, int *len, value_t *value)
{
  boolean result = false;
  *value = 0;
  while (*len>0 && **line>='0' && **line<='9') {
    *value = *value * 10 + (**line - '0');
    (*len)--;
    (*line)++;
    result = true;
  }
  return result;
}

boolean parse_tuple(char **line, int *len, value_t *tuple)
{
  boolean result = false;
  skip_whitespace(line, len);
  for (int i=0; i<TUPLE_LEN; i++) {
    if (parse_integer(line, len, tuple+i+1)) {
      result = true;
      tuple[0] = i+1;
      if (*len>0 && **line == ',') {
        // found comma, so more integers expected
        (*len)--;
        (*line)++;
        result = false;  // need another int to succeed
      } else {
        break;
      }
    } else {
      break;
    }
  }        
  return result;
}
void command_handler(char command, value_t *address, value_t *value)
{
  Serial.print("Command ");
  Serial.println(command);
    
  Serial.print("Address ");
  for (int i=0;i<address[0];i++) {
    Serial.print(" ");
    Serial.print(address[i+1], DEC);
  }
  Serial.println();
    
  Serial.print("Value ");
  for (int i=0;i<value[0];i++) {
    Serial.print(" ");
    Serial.print(value[i+1], DEC);
  }
  Serial.println();
}

void parse_line(char *line, int len)
{
  char command;
  value_t address[TUPLE_LEN+1];
  value_t value[TUPLE_LEN+1];
  
  Serial.println("Got a line");
  if (parse_command(&line, &len, &command) &&
      parse_tuple(&line, &len, address) &&
      parse_tuple(&line, &len, value)) {
        command_handler(command, address, value);
  } else {
    Serial.println("$Error Invalid command syntax");
  }
}

void parse_input()
{
  static char line[CMDBUF_LEN];  // NOT zero terminated
  static int len = 0;
  
  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      parse_line(line, len);
      len = 0;
    } else if (len < CMDBUF_LEN) {
      line[len++] = c;
    }
  }
}

void loop(void) {
  parse_input();
}



