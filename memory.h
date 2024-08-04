#ifndef memory_
#define memory_

#include <bits/stdc++.h>

int to_hex(char c){
  if(c >= '0' && c <= '9') return c - '0';
  else return c - 'A' + 10;
}

class memory{
private:
  unsigned char info[1048576]; // i do not know how big it should be
public:
  unsigned char get_info(int id){
    assert(id >= 0 && id <= 1048575);
    return info[id];
  }
  memory(){
    unsigned int current_location = 0, value = 0;
    bool flag = false;
    while(true){
      unsigned char c = getchar();
      if(c == EOF) break;
      if(c == '@'){
        std::cin >> std::hex >> current_location;
        //
      } else {
        value = (value << 4) ^ to_hex(c);
        if(flag == true){
          info[current_location] = value;
          current_location += 1;
        } flag = !flag;
      }
    }
  }
};


#endif