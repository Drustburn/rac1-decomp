#include "common.h"
#include "callees.h"
#include "structs.h"

extern int D_00161308 MACRO_ADDR;
extern int D_00161314 MACRO_ADDR;
extern int movie_clear_gp SDATA(D_00161318);
extern char *D_0016130C MACRO_ADDR;
extern char *movie_gp_base SDATA(D_0016130C);
extern int func_00118BA0(int,int);
extern int func_0023BB90(int,int,int);
extern void func_0023B740(void*,void*,void*);
extern void func_0023BE38(void);
/* No recovered name. Sets movie globals, runs initialization and playback, then clears them. */
int Movie_Play(int a,int b,int c,char *state,int e) {
    D_00161308=c;
    D_0016130C=state;
    D_00161314=0;
    movie_clear_gp=0;
    func_00118BA0(func_00118BE0(),1);
    D_00161314=1;
    if(initAll(a,b,e)) {
        char *base=movie_gp_base;
        D_00161314=2;
        func_0023B740(base+0xD9048,base,base+0xD9040);
    }
    termAll();
    D_00161308=0;
    D_0016130C=0;
    return 0;
}
INCLUDE_ASM("asm/nonmatchings/text", func_0023B740);
extern int func_00118BC0(int);

/* switchThread -- calls func_00118BC0(1). */
void switchThread(void) {
    func_00118BC0(1);
}
extern int func_0023C2B0(char *);

/* isAudioOK -- true when the audio decoder at offset 0xD9100 of the movie state holds 0x1000
 * bytes or more. */
int isAudioOK(void) {
    return audioDecIsPageFull(D_0016130C + 0xD9100);
}
struct AudioDecoderState {
    char pad0[0xD90F8];
    int dmac_handler_id;
    int intc_handler_id;
};
struct AudioDecoderThreadArgs {
    char pad0[4];
    void (*entry)();
    void *stack;
    int stack_size;
    void *gp;
    int priority;
    char pad18[8];
    int gs_tag;
    char pad24[0xC];
};
extern int D_00161310 MACRO_ADDR;
extern char D_001E8E20[];
extern char D_00166D00[] MACRO_ADDR;
extern void func_0012B870(void);
extern int func_0023CD10(char *);
extern int func_0023DE98(void *, int, int, void *, int, int, void *, int);
extern int func_0023BFA0(void *, void *, int, int);
extern int func_0023DFA0(void *, int, int, void *, char *);
extern void func_0023E560(void *, int, int, int);
extern int func_00118B50(struct AudioDecoderThreadArgs *);
extern int func_00118B70(int, void *);
extern int func_0023CE18(void *, int, int);
extern int func_00118A90(int, void *, int);
extern int func_00119390(int);
extern int func_00118AB0(int, void *, int);
extern int func_00119460(int);
extern void func_0023C7A8();
extern void func_0023C910();
extern void func_0023E1F8();
extern int func_0023C9C0();
extern int func_0023CAF8();
/* initAll(int, int, int) */
int func_0023BB90(int stream_source, int source_mode, int callback_context) {
    struct AudioDecoderThreadArgs decoder_thread;
    int thread_id;
    int opened;
    *((volatile int *)0x1000E000) |= 3;
    *((volatile int *)0x1000E010) = 4;
    func_0023CD10(movie_gp_base);
    func_0012B870();
    func_0023DE98(((char *)D_0016130C) + 0xD9048, D_00161308 + 0x1C85C0, 0xEB768,
                  ((char *)D_0016130C) + 0x52040, D_00161308 + 0x1C7180, 0x100,
                  ((char *)D_0016130C) + 0xD6040, 0x200);
    func_0023BFA0(((char *)D_0016130C) + 0xD9100, ((char *)D_0016130C) + 0x50040, 0x2000,
                  D_00161308 + 0x1C8190);
    func_0023DFA0(((char *)D_0016130C) + 0xD9048, 0, 0, func_0023C9C0, D_0016130C);
    func_0023DFA0(((char *)D_0016130C) + 0xD9048, 3, callback_context, func_0023CAF8, D_0016130C);
    func_0023E560(((char *)D_0016130C) + 0xD9168,
                  (D_00161308 & 0x0FFFFFFF) | 0x20000000,
                  D_00161308 + 0x1A0000, 2);
    decoder_thread.stack = ((char *)D_0016130C) + 0xD2040;
    decoder_thread.entry = func_0023E1F8;
    decoder_thread.stack_size = 0x4000;
    decoder_thread.priority = 1;
    decoder_thread.gp = D_00166D00;
    decoder_thread.gs_tag = 0;
    thread_id = func_00118B50(&decoder_thread);
    D_00161310 = thread_id;
    func_00118B70(thread_id, ((char *)D_0016130C) + 0xD9048);
    if (func_0023CE18(((char *)D_0016130C) + 0xD9040, stream_source, source_mode) == 0) {
        opened = 0;
        func_001E9730(D_001E8E20);
    } else {
        opened = 1;
    }
    ((struct AudioDecoderState *)D_0016130C)->intc_handler_id = func_00118A90(2, func_0023C7A8, 0);
    func_00119390(2);
    ((struct AudioDecoderState *)D_0016130C)->dmac_handler_id = func_00118AB0(2, func_0023C910, 0);
    func_00119460(2);
    return opened;
}
extern int movie_thread_gp SDATA(D_00161310);
extern int func_00120F30(int);
extern void func_0023CD28(char*);
extern void func_0023E5B0(char*);
extern int func_00118B80(int);
extern int func_00118B60(int);
extern int func_001193F8(int);
extern int func_00118AD0(int,int);
extern int func_00119328(int);
extern int func_00118AA0(int,int);
extern int func_0023E008(char*);
extern int func_0023C060(char*);
extern int func_0023CE28(char*);
/* termAll(void): shuts down workers and decoders, then clears the DMA enable bit. */
void termAll(void) {
    func_00120F30(0);
    readBufDelete(D_0016130C);
    voBufDelete(D_0016130C+0xD9168);
    func_00118B80(movie_thread_gp);
    func_00118B60(movie_thread_gp);
    func_001193F8(2);
    func_00118AD0(2,*(int*)(D_0016130C+0xD90F8));
    func_00119328(2);
    func_00118AA0(2,*(int*)(D_0016130C+0xD90FC));
    videoDecDelete(D_0016130C+0xD9048);
    audioDecDelete(D_0016130C+0xD9100);
    strFileDelete(D_0016130C+0xD9040);
    func_00120F30(0);
    *(volatile unsigned int*)0x1000E000 &= ~2u;
}
extern void func_001E9730(char *, ...);
extern char D_001612F8[];

/* ErrMessage -- passes the message to func_001E9730 (STUB_printf) with the string at
 * D_001612F8 as its first argument. */
void ErrMessage(char *fmt) {
    STUB_printf(D_001612F8, fmt);
}
extern void func_0023C2C0(char *);

/* proceedAudio(void) -- audioDecSend on the audio decoder at offset 0xD9100 of the movie state. */
void proceedAudio(void) {
    audioDecSend(D_0016130C + 0xD9100);
}
