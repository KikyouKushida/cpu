#ifndef decoder_
#define decoder_

#include <bits/stdc++.h>
#include "memory.h"

const unsigned int emp = -1;

std::map <std::string, unsigned int> type_to_uint;

void type_error(unsigned int location){
  std::cout << "Instruction Error in location " << location << "\n";
  exit(0);
}

struct instruction{
  unsigned int opcode, imm, rd, funct3, rs1, rs2, funct7;
  std::string type;
  instruction(){
    rs1 = rs2 = emp;
  }
};

class decoder{
private:
  static unsigned int get_value(unsigned int value, unsigned char l, unsigned char r){
    assert(l >= 0 && l <= r && r <= 31);
    unsigned char tmp = ((1 << r + 1) - 1) & value;
    return tmp >> l;
  }
  memory *m;
public:
  decoder(memory *m_){
    m = m_;
  }
  instruction decode(unsigned int location){
    unsigned char tmp[4];
    instruction ans;
    for(int i = 0; i <= 3; ++i) tmp[i] = m->get_info(location + i);
    ans.opcode = get_value(tmp[0], 0, 6);
    if(ans.opcode == 0x33){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1),
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1),
      ans.rs2 = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 0) << 4),
      ans.funct7 = get_value(tmp[3], 1, 7);
      if(ans.funct3 == 0x0){
        if(ans.funct7 == 0x0) ans.type = "add";
        else if(ans.funct7 == 0x20) ans.type = "sub";
        else type_error(location);
      }
      else if(ans.funct3 == 0x7) ans.type = "and";
      else if(ans.funct3 == 0x6) ans.type = "or";
      else if(ans.funct3 == 0x4) ans.type = "xor";
      else if(ans.funct3 == 0x1) ans.type = "sll";
      else if(ans.funct3 == 0x5){
        if(ans.funct7 == 0x0) ans.type = "srl";
        else if(ans.funct7 == 0x20) ans.type = "sra";
        else type_error(location);
      }
      else if(ans.funct3 == 0x2) ans.type = "slt";
      else if(ans.funct3 == 0x3) ans.type = "sltu";
      else {
        type_error(location);
        assert(0);
      }
    }//R
    else if(ans.opcode == 0x13){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1),
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1);
      if(ans.funct3 == 0x1 || ans.funct3 == 0x5){
        ans.imm = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 0) << 4),
        ans.funct7 = get_value(tmp[3], 1, 7);
      }
      else {
        ans.imm = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 7) << 4);
      }
      if(ans.funct3 == 0x0) ans.type = "addi";
      else if(ans.funct3 == 0x7) ans.type = "andi";
      else if(ans.funct3 == 0x6) ans.type = "ori";
      else if(ans.funct3 == 0x4) ans.type = "xori";
      else if(ans.funct3 == 0x1 && ans.funct7 == 0x0) ans.type = "slli";
      else if(ans.funct3 == 0x5 && ans.funct7 == 0x0) ans.type = "srli";
      else if(ans.funct3 == 0x5 && ans.funct7 == 0x20) ans.type = "srai";
      else if(ans.funct3 == 0x2) ans.type = "slti";
      else if(ans.funct3 == 0x3) ans.type = "sltiu";
      else {
        type_error(location);
        assert(0);
      }
    }//I + I*
    else if(ans.opcode == 0x3){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1),
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1);
      ans.imm = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 7) << 4);
      if(ans.funct3 == 0x0) ans.type = "lb";
      else if(ans.funct3 == 0x4) ans.type = "lbu";
      else if(ans.funct3 == 0x1) ans.type = "lh";
      else if(ans.funct3 == 0x5) ans.type = "lhu";
      else if(ans.funct3 == 0x2) ans.type = "lw";
      else {
        type_error(location);
        assert(0);
      }
    }//I
    else if(ans.opcode == 0x23){  
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1),
      ans.rs2 = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 0) << 4);
      ans.imm = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1) ^ (get_value(tmp[3], 1, 7) << 5);
      if(ans.funct3 == 0x0) ans.type = "sb";
      else if(ans.funct3 == 0x1) ans.type = "sh";
      else if(ans.funct3 == 0x2) ans.type = "sw";
      else {
        type_error(location);
        assert(0);
      }
    }//S
    else if(ans.opcode == 0x63){
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1),
      ans.rs2 = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 0) << 4);
      ans.imm = (get_value(tmp[1], 0, 3) << 1) ^ (get_value(tmp[3], 1, 6) << 5) ^ (get_value(tmp[0], 7, 7) << 11) ^ (get_value(tmp[3], 7, 7) << 12);              
      if(ans.funct3 == 0x0) ans.type = "beq";
      else if(ans.funct3 == 0x5) ans.type = "bge";
      else if(ans.funct3 == 0x7) ans.type = "bgeu";
      else if(ans.funct3 == 0x4) ans.type = "blt";
      else if(ans.funct3 == 0x6) ans.type = "bltu";
      else if(ans.funct3 == 0x1) ans.type = "bne";
      else {
        type_error(location);
        assert(0);
      }
    }//B
    else if(ans.opcode == 0x6f){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1);
      ans.imm = (get_value(tmp[2], 5, 7) << 1) ^ (get_value(tmp[3], 0, 6) << 4) ^ (get_value(tmp[2], 4, 4) << 11) ^ (get_value(tmp[1], 4, 7) << 12) ^ (get_value(tmp[2], 0, 3) << 16) ^ (get_value(tmp[3], 7, 7) << 20);
      ans.type = "jal";
    }//J
    else if(ans.opcode == 0x67){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1),
      ans.funct3 = get_value(tmp[1], 4, 6),
      ans.rs1 = get_value(tmp[1], 7, 7) ^ (get_value(tmp[2], 0, 3) << 1);
      ans.imm = get_value(tmp[2], 4, 7) ^ (get_value(tmp[3], 0, 7) << 4);
      ans.type = "jalr";
    }//I
    else if(ans.opcode == 0x17){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1);
      ans.imm = (get_value(tmp[1], 4, 7) << 12) ^ (get_value(tmp[2], 0, 7) << 16) ^ (get_value(tmp[3], 0, 7) << 24);
      ans.type = "auipc";
    }//U
    else if(ans.opcode == 0x37){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1);
      ans.imm = (get_value(tmp[1], 4, 7) << 12) ^ (get_value(tmp[2], 0, 7) << 16) ^ (get_value(tmp[3], 0, 7) << 24);
      ans.type = "lui";
    }
    else if(ans.opcode == 0x73){
      ans.rd = get_value(tmp[0], 7, 7) ^ (get_value(tmp[1], 0, 3) << 1);
      ans.imm = (get_value(tmp[1], 4, 7) << 12) ^ (get_value(tmp[2], 0, 7) << 16) ^ (get_value(tmp[3], 0, 7) << 24);
      if(ans.imm == 0x0) ans.type = "ebreak";
      else if(ans.imm == 0x1) ans.type = "ecall";
      else {
        type_error(location);
        assert(0);
      }
    }
    else {
      std::cout << "Instruction Error in location " << location << "\n";
      exit(0);
    }
    return ans;
  }
  
};



#endif
