#include <bits/stdc++.h>
#include "memory.h"
#include "decoder.h"
#include "ROB.h"

unsigned int now_time = 0;
using psu = std::pair<std::string, unsigned int>;

unsigned int sext(unsigned int val, unsigned int w){
  if(val & (1 << w - 1)){
    for(int i = w; i < 32; ++i) 
      val ^= (1 << i);
  }
  return val;
}

class simulator{
public:
  memory m;
  simulator(): m(){
    
  }
  void issue_alu(instruction &th){
    instructionQueue.update();
    instructionQueue.issue();
    instructionQueue.push(th);
    return ;
  }
  void issue_load(instruction &th){
    update();
    if(th.opcode == 19){
      unsigned int location = rsts.get_value(th.rs1) + th.imm;
      rsts.store(th.rd, sext(m.get_info(location), 8), -1);
    } 
    else if(th.opcode == 20){
      unsigned int location = rsts.get_value(th.rs1) + th.imm;
      rsts.store(th.rd, m.get_info(location), -1);
    }
    else if(th.opcode == 21){
      unsigned int location = rsts.get_value(th.rs1) + th.imm;
      unsigned int v1 = m.get_info(location), v2 = m.get_info(location + 1);

    }
  }
  void issue_store(instruction &th){

  }
  void issue_other(instruction &th){

  }
  void update(){
    alu.update();
    rsts.update();
    reorderBuffer.update();
    reservationStation.update();
  }
  void run(){
    reorderBuffer.run();
    reservationStation.run();
  }
  void work(){
    decoder dc(&m);
    {
      type_to_uint.insert(psu("add", 0));
      type_to_uint.insert(psu("sub", 1));
      type_to_uint.insert(psu("and", 2));
      type_to_uint.insert(psu("or", 3));
      type_to_uint.insert(psu("xor", 4));
      type_to_uint.insert(psu("sll", 5));
      type_to_uint.insert(psu("srl", 6));
      type_to_uint.insert(psu("sra", 7));
      type_to_uint.insert(psu("slt", 8));
      type_to_uint.insert(psu("sltu", 9));
      type_to_uint.insert(psu("addi", 10));
      type_to_uint.insert(psu("andi", 11));
      type_to_uint.insert(psu("ori", 12));
      type_to_uint.insert(psu("xori", 13));
      type_to_uint.insert(psu("slli", 14));
      type_to_uint.insert(psu("srli", 15));
      type_to_uint.insert(psu("srai", 16));
      type_to_uint.insert(psu("slti", 17));
      type_to_uint.insert(psu("sltiu", 18));
      type_to_uint.insert(psu("lb", 19));
      type_to_uint.insert(psu("lbu", 20));
      type_to_uint.insert(psu("lh", 21));
      type_to_uint.insert(psu("lhu", 22));
      type_to_uint.insert(psu("lw", 23));
      type_to_uint.insert(psu("sb", 24));
      type_to_uint.insert(psu("sh", 25));
      type_to_uint.insert(psu("sw", 26));
      type_to_uint.insert(psu("beq", 27));
      type_to_uint.insert(psu("bge", 28));
      type_to_uint.insert(psu("bgeu", 29));
      type_to_uint.insert(psu("blt", 30));
      type_to_uint.insert(psu("bltu", 31));
      type_to_uint.insert(psu("bne", 32));
      type_to_uint.insert(psu("jal", 33));
      type_to_uint.insert(psu("jalr", 34));
      type_to_uint.insert(psu("auipc", 35));
      type_to_uint.insert(psu("lui", 36));
      type_to_uint.insert(psu("ebreak", 37));
      type_to_uint.insert(psu("ecall", 38));
    }
    while(true){
      update();
      run();
      instruction th = dc.decode(rsts.get_pc());
      if(th.opcode <= 18) issue_alu(th), rsts.increase_pc(4);
      else {
        if(!reorderBuffer.empty() || !reservationStation.empty()){
          continue;
        }
        if(th.opcode <= 23) issue_load(th);
        else if(th.opcode <= 26) issue_store(th);
        else issue_other(th);
      }
    }
  }

};

int main(){
  simulator my_simulator;
  my_simulator.work();
  return 0;
}