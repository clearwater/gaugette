
#ifndef Command_h
#define Command_h

#include <Arduino.h>
#define COMMAND_TUPLE_LEN 3
typedef unsigned int value_t;

class Command
{
  public:
    char command;
    value_t address[COMMAND_TUPLE_LEN+1];
    value_t value[COMMAND_TUPLE_LEN+1];
  
  private:
    void skipWhitespace(char**, int*);
    boolean parseCommand(char **line, int *len, char *cmd);
    boolean parseInteger(char **line, int *len, value_t *value);
    boolean parseTuple(char **line, int *len, value_t *tuple);
    boolean parseLine(char *line, int len);
  public:
    boolean parseInput();
    void dump();
};

#endif

