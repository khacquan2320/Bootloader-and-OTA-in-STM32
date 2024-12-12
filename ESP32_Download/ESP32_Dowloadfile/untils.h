// xu ly file hex 
int8_t char_to_byte(char c)
{
    if ( c >= '0' && c <= '9') return (c - '0');
    else if ( c >= 'A' && c <= 'F') return (c - 'A' + 10);
    else if ( c >= 'a' && c <= 'f') return (c - 'a' + 10);
    return -1;
}
//":100000007006002089010008B90A0008B50A000836"
void convert_string_intel_hex_to_array_hex(char *input, uint8_t *output )
{
  uint8_t index =0;
  if(*input == ':')
  {
      input++;
      while(*input != '\0')
      {
          uint8_t hex_value = char_to_byte(*input)<<4;//4 bit cao
          input++;
          hex_value |= char_to_byte(*input);//4 bit thap
          input++;
          output[index++] = hex_value;
      }
  }
}

int8_t check_some(uint8_t *buff,uint8_t len)
{
    uint8_t sum=0;
    for(uint8_t i=0; i<len-1 ;i++)
    {
        sum += buff[i];
    }
    sum = ~sum;
    sum += 1;
    if(sum == buff[len-1])
    {
        return 0;
    }
    return -1;
}

void swap(uint8_t *a, uint8_t *b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}
void swap_4_byte(uint8_t *data, uint8_t len)
{
    for(int i=0; i<len; i += 4)
    {
        swap(&data[i+0], &data[i+3]);
        swap(&data[i+1], &data[i+2]);
    }
}