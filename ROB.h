#ifndef ROB_
#define ROB_
#include <bits/stdc++.h>
#include "decoder.h"
#include "register.h"

class Alu;
class ReservationStation;
class ReorderBuffer;
class Registers;
class InstructionQueue;

Alu alu;
ReorderBuffer reorderBuffer;
ReservationStation reservationStation;
Registers rsts;
InstructionQueue instructionQueue;


class InstructionQueue{
private:
  std::queue <instruction> queue, next_queue;
  InstructionQueue(){}
public:
  void update(){
    queue = next_queue;
  }
  void push(instruction &ins){
    queue.push(ins);
    return ;
  }
  void issue(){
    if(reservationStation.full() || reorderBuffer.full() || queue.empty()) return ;
    unsigned int ROB_id = reorderBuffer.put_in(queue.front());
    reservationStation.put_in(queue.front(), ROB_id);
    next_queue.pop();
    return ;
  }
};


class Alu{
private:
  unsigned int val, next_val, ROB_id, next_ROB_id;
public:
  void update(){
    val = next_val;
    ROB_id = next_ROB_id;
  }
  void run(){
    if(ROB_id != -1){
      /*
      
        call ROB
      
      */
      reservationStation.finish(ROB_id, val);
      reorderBuffer.finish(ROB_id, val);
      next_ROB_id = -1;
    }
  }
  Alu(){
    val = next_val = 0;
    ROB_id = next_ROB_id = -1;
  }
  // void run(instruction ins){
  //   switch(type_to_uint[ins.type]){
  //     case 0: next_val = ins.rs1 + ins.rs2; break;
  //     case 1: next_val = ins.rs1 - ins.rs2; break;
  //     case 2: next_val = ins.rs1 & ins.rs2; break;
  //     case 3: next_val = ins.rs1 | ins.rs2; break;
  //     case 4: next_val = ins.rs1 ^ ins.rs2; break;
  //     case 5: next_val = ins.rs1 << ins.rs2; break;
  //     case 6: next_val = ins.rs1 >> ins.rs2; break;
  //     case 7: {
  //       if(ins.rs1 & (1u << 31)){
  //         unsigned int tmp = 0;
  //         for(int i = 31; i >= 31 - ins.rs2 + 1; --i) tmp ^= (1u << i);
  //         next_val = (ins.rs1 >> ins.rs2) ^ tmp;
  //       }
  //       else next_val = ins.rs1 >> ins.rs2;
  //       break;
  //     }
  //     case 8: next_val = (ins.rs1 < ins.rs2) ? 1 : 0; break;
  //     case 9: next_val = (ins.rs1 < ins.rs2) ? 1 : 0; break;
  //     case 10: next_val = ins.rs1 + ins.imm; break;
  //     case 11: next_val = ins.rs1 & ins.imm; break;
  //     case 12: next_val = ins.rs1 | ins.imm; break;
  //     case 13: next_val = ins.rs1 ^ ins.imm; break;
  //     case 14: next_val = ins.rs1 << ins.imm; break;
  //     case 15: next_val = ins.rs1 >> ins.imm; break;
  //     case 16: {
  //       if(ins.rs1 & (1u << 31)){
  //         unsigned int tmp = 0;
  //         for(int i = 31; i >= 31 - ins.imm + 1; --i) tmp ^= (1u << i);
  //         next_val = (ins.rs1 >> ins.imm) ^ tmp;
  //       }
  //       else next_val = ins.rs1 >> ins.imm;
  //       break;
  //     }
  //     case 17: next_val = (ins.rs1 < ins.imm) ? 1 : 0; break;
  //     case 18: next_val = (ins.rs1 < ins.imm) ? 1 : 0; break;
  //   }

  //  }
  void run(RSdata &ins){
    switch(ins.op){
      case 0: next_val = ins.v1 + ins.v2; break;
      case 1: next_val = ins.v1 - ins.v2; break;
      case 2: next_val = ins.v1 & ins.v2; break;
      case 3: next_val = ins.v1 | ins.v2; break;
      case 4: next_val = ins.v1 ^ ins.v2; break;
      case 5: next_val = ins.v1 << ins.v2; break;
      case 6: next_val = ins.v1 >> ins.v2; break;
      case 7: {
        if(ins.v1 & (1u << 31)){
          unsigned int tmp = 0;
          for(int i = 31; i >= 31 - ins.v2 + 1; --i) tmp ^= (1u << i);
          next_val = (ins.v1 >> ins.v2) ^ tmp;
        }
        else next_val = ins.v1 >> ins.v2;
        break;
      }
      case 8: next_val = (ins.v1 < ins.v2) ? 1 : 0; break;
      case 9: next_val = (ins.v1 < ins.v2) ? 1 : 0; break;
      case 10: next_val = ins.v1 + ins.v2; break;
      case 11: next_val = ins.v1 & ins.v2; break;
      case 12: next_val = ins.v1 | ins.v2; break;
      case 13: next_val = ins.v1 ^ ins.v2; break;
      case 14: next_val = ins.v1 << ins.v2; break;
      case 15: next_val = ins.v1 >> ins.v2; break;
      case 16: {
        if(ins.v1 & (1u << 31)){
          unsigned int tmp = 0;
          for(int i = 31; i >= 31 - ins.v2 + 1; --i) tmp ^= (1u << i);
          next_val = (ins.v1 >> ins.v2) ^ tmp;
        }
        else next_val = ins.v1 >> ins.v2;
        break;
      }
      case 17: next_val = (ins.v1 < ins.v2) ? 1 : 0; break;
      case 18: next_val = (ins.v1 < ins.v2) ? 1 : 0; break;
    }
    return ;
  }
};

struct RSdata {
  unsigned int busy, ROB_id, op, q1, q2, v1, v2;
  RSdata(){
   busy = 0; 
  }
  RSdata &operator=(const RSdata &other){
    busy = other.busy;
    ROB_id = other.ROB_id;
    op = other.op;
    q1 = other.q1;
    q2 = other.q2;
    v1 = other.v1;
    v2 = other.v2;
    return *this;
  }
};

class ReservationStation{
private:
  static const int MAX_N = 37;
  RSdata data[MAX_N], next_data[MAX_N];
  unsigned int max_size;
public:
  void update(){
    for(int i = 0; i < max_size; ++i) data[i] = next_data[i];
    return ;
  }
  ReservationStation(){
    max_size = MAX_N - 5;
  }
  bool full(){
    for(int i = 1; i <= max_size; ++i) if(!data[i].busy) return false;
    return true;
  }
  bool empty(){
    for(int i = 1; i <= max_size; ++i) if(data[i].busy) return false;
    return true;
  }
  void put_in(instruction &ins, unsigned int ROB_id){
    unsigned int to;
    for(int i = 1; i <= max_size; ++i) if(!data[i].busy) to = i;
    data[to].busy = true;
    data[to].ROB_id = ROB_id;
    data[to].op = ins.opcode;
    if(ins.rs1 != emp){
      data[to].q1 = rsts.get_q(ins.rs1);
      data[to].v1 = rsts.get_value(ins.rs1);
    }
    else {
      data[to].q1 = 0;
      data[to].v1 = ins.imm;
    }
    if(ins.rs2 != emp){
      data[to].q2 = rsts.get_q(ins.rs2);
      data[to].v2 = rsts.get_value(ins.rs2);
    }
    else {
      data[to].q2 = 0;
      data[to].v2 = ins.imm;
    }
  }
  void run(){
    for(int i = 1; i <= max_size; ++i)
      if(data[i].busy && data[i].q1 == 0 && data[i].q2 == 0){
        alu.run(data[i]);
        next_data[i].busy = 0;
        break;
      }
  }
  void finish(unsigned int ROB_id, unsigned int value){
    for(int i = 1; i <= max_size; ++i)
      if(next_data[i].busy){
        if(next_data[i].q1 == ROB_id){
          next_data[i].q1 = 0;
          next_data[i].v1 = value;
        }
        if(next_data[i].q2 == ROB_id){
          next_data[i].q2 = 0;
          next_data[i].v2 = value;
        }
      }
    return ;
  }// called by Alu
};

class ReorderBuffer{
private:
  static const unsigned int MAX_N = 37;
  unsigned int max_size;
  struct ROB{
    bool finished;
    unsigned int type, ROB_id, result, where;
  };
  ROB queue[MAX_N], next_queue[MAX_N];
  unsigned int head, tail, current_id;
  unsigned int next_head, next_tail, next_current_id;
public:
  void update(){
    head = next_head;
    tail = next_tail;
    for(int i = 1; i <= max_size; ++i) queue[i] = next_queue[i];
    current_id = next_current_id;
    return ;
  }
  ReorderBuffer(){
    head = 0, tail = 0;
    next_head = 0, next_tail = 0;
    max_size = MAX_N - 5;
    current_id = 0;
  }
  bool full(){
    return (tail + 1) % max_size == head ? true : false;
  }
  bool empty(){
    return head == tail ? true : false;
  }
  unsigned int put_in(instruction &ins){
    ROB new_one;
    new_one.where = ins.rd;
    new_one.type = type_to_uint[ins.type];
    next_current_id = current_id + 1; 
    new_one.ROB_id = next_current_id;
    new_one.finished = false;
    next_queue[next_tail]= new_one;
    next_tail = (tail + 1) % max_size;
    return next_current_id;
  }
  void run(){
    if(head == tail){
      // queue is now empty.
      return ;
    }
    if(queue[head].finished){
      // this instruction in the head of the queue has been finished
      commit(queue[head]);
      next_head = (head + 1) % max_size;
    }
    return ;
  }
  void commit(ROB rob){
    if(rob.type <= 18) rsts.store(rob.where, rob.result, rob.ROB_id); // Arithmetic operations
    else if(1){

    }
    else {

    }
  }
  void finish(unsigned int ROB_id, unsigned int value){
    for(int i = next_head; i != next_tail; i = (i + 1) % max_size)
      if(queue[i].finished == false && queue[i].ROB_id == ROB_id)
        queue[i].finished = true,
        queue[i].result = value;
    return ;
  }
};

class Registers{
private:
  unsigned int value[32], next_value[32], q[32], next_q[32], pc, next_pc;
public:
  unsigned int get_pc(){
    return pc;
  }
  void increase_pc(unsigned int offset){
    pc = pc + offset;
    return ;
  }
  unsigned int get_value(unsigned int location){
    return value[location];
  }
  unsigned int get_q(unsigned int location){
    return q[location];
  }
  Registers(){
    pc = 0;
    for(int i = 0; i < 32; ++i){
      value[i] = next_value[i] = 0;
      q[i] = next_q[i] = 0;
    }
  }
  void update(){
    pc = next_pc;
    for(int i = 0; i < 32; ++i) value[i] = next_value[i];
    for(int i = 0; i < 32; ++i) q[i] = next_q[i];
    return ;
  }
  unsigned int load(unsigned int location){
    if(location < 0 || location > 31) assert(0);
    return value[location];
  }
  void store(unsigned int location, unsigned int rvalue, unsigned int ROB_id){
    if(location < 0 || location > 31) assert(0);
    if(location == 0) return ;
    next_value[location] = rvalue;
    if(ROB_id == next_q[location]) next_q[location] = 0;
    return ; 
  }
};



#endif