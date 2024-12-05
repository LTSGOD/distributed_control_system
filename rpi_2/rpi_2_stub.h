#ifndef RPI2_STUB_H_
#define RPI2_STUB_H_

typedef int (*RPC_handler_ptr)(char* buf);

int init_stub();
void register_RPC_handler(RPC_handler_ptr handler, int handler_num);

// stub 종료
int terminateRPC(char* text);


#endif