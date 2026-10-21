/* NON_MATCHING func_L00_002DF410 -- src/overlays/shared/vendor_002D9438.c
 * Best so far: BYTES 178/4220 (95.8% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-21: Lombyte's draft (FUN_L00_002ddf60, nanotech_ball.c, GPL-3.0) with the PAL symbols: BYTES 178/4220,
 *   where the staged candidate was SIZE 4208/4220 (its notes, on the reversed 20-point loop, are in the history).
 *   D_L00_00161C54 must be MACRO_ADDR (retail reaches it through the lui macro, and once through $gp in a delay
 *   slot); plain, the function is 8 bytes short. Left: register choice in the particle loops (+0x61C..+0x968):
 *   retail derives the table at D_L00_001E6290 from D_L00_001E6280 (+0x10) in one place, and keeps the ring
 *   counters in $s2/$s3 the other way round.
 */
typedef struct { short idx; short angle; float f; float phase; float phase_step; float speed; char *p; } E04_2DF410;
typedef unsigned int Q41_2DF410 __attribute__((mode(TI)));

typedef union { struct { float x,y,z,w; } f; Q41_2DF410 q; } V41_2DF410 __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x34]; u16 flags; u8 pad36[0x8A]; V41_2DF410 basis[3];
} M41_2DF410;

typedef struct {
    u8 pad0[0x640]; V41_2DF410 matrix[3]; u8 pad670[0x1A10];
    M41_2DF410 *moby; int camera_mode; u8 pad2088[0x240]; s16 special;
} Hero41_2DF410;

typedef struct {
    float unknown0; int mode; u8 pad8[2]; u8 r,g; u8 padC[4];
    V41_2DF410 velocity; float acceleration;
} Tail41_2DF410;

typedef struct Particle41_2DF410 {
    u8 pad0[0xA]; s16 life; float size; V41_2DF410 pos; Tail41_2DF410 tail;
} Particle41_2DF410;
extern float func_L00_001EB6A8_2DF410(void *,float,float,float,float,float) __asm__("func_L00_001EB6A8");
extern void func_L00_0025C710_2DF410(void *,void *,void *,float) __asm__("func_L00_0025C710");
extern void func_L00_002688A8_2DF410(void *) __asm__("func_L00_002688A8");
extern void func_L00_002DF168_2DF410(char *a, char *b) __asm__("func_L00_002DF168");
extern float absolute_float_2DF410(float) __asm__("func_001F9B88");
extern void add_vector_xyz_2DF410(void *, void *, void *) __asm__("func_001F9BD8");
extern void clear_u64_value_2DF410(void *) __asm__("func_001F9BC0");
extern int collision_line_ddf60_2DF410(void *, void *, int, int, int) __asm__("func_L00_001EFFF0");
extern f32 convert_integer_to_float_2DF410(s32) __asm__("func_001FA888");
extern void cross_vectors_xyz_2DF410(void *, void *, void *) __asm__("func_001F9CA0");
extern void enqueue_callback_list_1_alt_2DF410(void (*)(void), int) __asm__("func_001F49B0");
extern float fast_add_rotations_2DF410(float, float) __asm__("func_001FA748");
extern f32 fast_subtract_rotations_2DF410(f32, f32) __asm__("func_001FA790");
extern f32 fast_arc_sin_2DF410(f32) __asm__("func_001F9FC0");
extern f32 fast_sin_2DF410(f32) __asm__("func_001F9FA8");
extern f32 fast_vec_dot_2DF410(void *a, void *b) __asm__("func_001F9C78");
extern f32 fast_vec_length_2DF410(void *) __asm__("func_001F9CB8");
extern void fast_vec_normalize_2DF410(void *, void *, f32) __asm__("func_L00_001FF4B0");
extern void fast_vec_scale_2DF410(void *, void *, f32) __asm__("func_001F9C30");
extern void func_002156E0_2D9EA0_2DF410(void *, void *, float, void *) __asm__("func_002156E0");
extern void lerp_vector_2DF410(void *, void *, void *, f32) __asm__("func_001F9C08");
extern Particle41_2DF410 * new_particle41_2DF410(void *,float,float,float,int,void *,float,int) __asm__("func_L00_0026DEA0");
extern f32 random_float_between_2DF410(f32, f32) __asm__("func_002140F8");
extern s32 random_integer_below_2DF410(s32) __asm__("func_002140B0");
extern s32 scale_frames_ddf60_2DF410(s32) __asm__("func_001F9850");
extern void subtract_vector_xyz_2DF410(void *, void *, void *) __asm__("func_001F9BF0");
extern void transform_vector_by_basis_2DF410(void *, void *, void *) __asm__("func_001F9EC0");
extern s32 truncate_float_to_s32_2DF410(f32) __asm__("func_001FA898");
extern f32 vector_distance_2DF410(void *, s32 *) __asm__("func_001F9D48");
extern V41_2DF410 D_L00_001E4E90_2DF410[] __asm__("D_L00_001E4E90");
extern E04_2DF410 D_L00_001E4FD0_2DF410[] __asm__("D_L00_001E4FD0");
extern char D_L00_00161C30_2DF410[] __asm__("D_L00_00161C30") MACRO_ADDR;
extern char D_L00_00161C40_2DF410[] __asm__("D_L00_00161C40") MACRO_ADDR;
extern char D_L00_001670D0_2DF410[] __asm__("D_L00_001670D0");
extern char D_L00_00173F60_2DF410[] __asm__("D_L00_00173F60");
extern char D_L00_001E6230_2DF410[] __asm__("D_L00_001E6230");
extern char D_L00_001E6280_2DF410[] __asm__("D_L00_001E6280");
extern char D_L00_001E6290_2DF410[] __asm__("D_L00_001E6290");
extern char D_L00_001E6370_2DF410[] __asm__("D_L00_001E6370");
extern float D_0015EE60_2DF410 __asm__("D_0015EE60") MACRO_ADDR;
extern float D_L00_00161C54_2DF410 __asm__("D_L00_00161C54") MACRO_ADDR;
extern float D_L00_00161C50_2DF410 __asm__("D_L00_00161C50") MACRO_ADDR;
extern float D_L00_001E6330_2DF410[] __asm__("D_L00_001E6330");
extern short D_L00_001E6358_2DF410[] __asm__("D_L00_001E6358");
extern int D_L00_00161C24_2DF410 SDATA(D_L00_00161C24);
extern int D_L00_00161C28_2DF410 SDATA(D_L00_00161C28);
extern void func_L00_002DED98_2DF410(void) __asm__("func_L00_002DED98");
extern Hero41_2DF410 hero_2DF410 __asm__("D_0013F450");

/* Suck Cannon tube: bends the suction tube between the gun and its target and updates its particles. Adapted
   from Lombyte (GPL-3.0) for PAL: src/overlays/shared/gameplay/entities/nanotech_ball.c, FUN_L00_002ddf60 (a near miss
   there too). */
void func_L00_002DF410(void *moby,void *position) {
    V41_2DF410 step,up,orient[3],ray_end,start,end,axis,tmp_a,tmp_b,
        frame[3],tmp_c,motion,tmp_d,tmp_e,interp,cross_c,basis_c,tmp_f,unused_a,unused_b;
    V41_2DF410 *orientation;
    int i,j,n,segment,next_ring;
    float t,angle,turn,delta,dot,length,radius,phase,amount,speed;
    Particle41_2DF410 *p;
    V41_2DF410 *anchor,*previous,*source;


    func_L00_002DF168_2DF410(moby,position);
    if((((Hero41_2DF410 *)&hero_2DF410)->camera_mode==30 || ((Hero41_2DF410 *)&hero_2DF410)->camera_mode==1) && (((Hero41_2DF410 *)&hero_2DF410)->moby->flags&1)) {
        qcopy(&orient[0],D_L00_001670D0_2DF410);
        qcopy(&orient[1],D_L00_001670D0_2DF410+16);
        qcopy(&orient[2],D_L00_001670D0_2DF410+32);
        orientation=&orient[0];
    } else if(((Hero41_2DF410 *)&hero_2DF410)->special==1) {
        fast_vec_scale_2DF410(&orient[0],&((M41_2DF410 *)moby)->basis[1],-1.0f);
        { M41_2DF410 *m=moby;
        qcopy_nc(&orient[1],&m->basis[0]);
        qcopy_nc(&orient[2],&m->basis[2]); }
        orientation=&orient[0];
    } else {
        qcopy(&orient[0],&((Hero41_2DF410 *)&hero_2DF410)->matrix[0]);
        qcopy(&orient[1],&((Hero41_2DF410 *)&hero_2DF410)->matrix[1]);
        qcopy(&orient[2],&((Hero41_2DF410 *)&hero_2DF410)->matrix[2]);
        orientation=&orient[0];
    }
    fast_vec_normalize_2DF410(&step,orientation,D_L00_00161C54_2DF410);
    fast_vec_normalize_2DF410(&up,&orient[2],1.0f);
    qcopy(D_L00_001E6290_2DF410,position);
    fast_vec_normalize_2DF410(&ray_end,orientation,D_L00_00161C54_2DF410*10.0f);
    add_vector_xyz_2DF410(&ray_end,&ray_end,position);
    if(collision_line_ddf60_2DF410(position,&ray_end,20,(int)((Hero41_2DF410 *)&hero_2DF410)->moby,0)) {
        int n,i,k;
        n=truncate_float_to_s32_2DF410(vector_distance_2DF410(D_L00_00173F60_2DF410,position)/D_L00_00161C54_2DF410);
        for(i=n;i<10;i++) {
            if(D_L00_001E6358_2DF410[i]>64)D_L00_001E6358_2DF410[i]=64;
            D_L00_001E6358_2DF410[i]=(u16)D_L00_001E6358_2DF410[i]-4;
            if(D_L00_001E6358_2DF410[i]<0)D_L00_001E6358_2DF410[i]=0;
        }
        for(k=n-1;k>=0;k--)D_L00_001E6358_2DF410[k]=D_L00_001E6358_2DF410[k+1]+32;
    } else {
        int i;
        D_L00_001E6358_2DF410[0]=(u16)D_L00_001E6358_2DF410[0]+4;
        for(i=1;i<10;i++)D_L00_001E6358_2DF410[i]=D_L00_001E6358_2DF410[i-1]-32;
    }
    transform_vector_by_basis_2DF410(&start,&(*(V41_2DF410 *)D_L00_00161C30_2DF410),orientation);
    add_vector_xyz_2DF410(&start,&start,&step);
    transform_vector_by_basis_2DF410(&end,&(*(V41_2DF410 *)D_L00_00161C40_2DF410),orientation);
    add_vector_xyz_2DF410(&end,&end,&step);
    t=(float)D_L00_00161C24_2DF410/convert_integer_to_float_2DF410(scale_frames_ddf60_2DF410(60));
    D_L00_00161C24_2DF410++;
    step.f.x=start.f.x+(end.f.x-start.f.x)*t;
    step.f.y=start.f.y+(end.f.y-start.f.y)*t;
    step.f.z=start.f.z+(end.f.z-start.f.z)*t;
    if(D_L00_00161C24_2DF410>=scale_frames_ddf60_2DF410(60)) {
        qcopy(&(*(V41_2DF410 *)D_L00_00161C30_2DF410),&(*(V41_2DF410 *)D_L00_00161C40_2DF410));
        fast_vec_normalize_2DF410(&axis,orientation,D_L00_00161C54_2DF410);
        tmp_a.q=0; tmp_b.q=0;
        tmp_a.f.z=0.1f; tmp_a.f.w=1.0f;
        tmp_b.f.x=1.0f; tmp_b.f.w=1.0f;
        angle=fast_add_rotations_2DF410(D_L00_00161C50_2DF410,random_float_between_2DF410(45.0f,315.0f)*0.0174532924f);
        func_002156E0_2D9EA0_2DF410(&(*(V41_2DF410 *)D_L00_00161C40_2DF410),&tmp_a,angle,&tmp_b);
        D_L00_00161C50_2DF410=angle;
        D_L00_00161C24_2DF410=0;
    }
    (*(float *)&D_L00_00161C28_2DF410)=fast_add_rotations_2DF410((*(float *)&D_L00_00161C28_2DF410),0.1f);
    qcopy(&axis,&orient[2]);
    qcopy(&frame[2],&axis);
    cross_vectors_xyz_2DF410(&frame[1],&step,&frame[2]);
    fast_vec_normalize_2DF410(&frame[1],&frame[1],1.0f);
    cross_vectors_xyz_2DF410(&frame[0],&frame[2],&frame[1]);
    { int k;
    for(k=0;k<20;k++) {
        fast_vec_scale_2DF410(&tmp_a,&D_L00_001E4E90_2DF410[k],0.1f);
        transform_vector_by_basis_2DF410(&tmp_a,&tmp_a,frame);
        add_vector_xyz_2DF410(&((V41_2DF410 *)D_L00_001E6370_2DF410)[k],D_L00_001E6290_2DF410,&tmp_a);
    }
    }
    { int i;
    for(i=1;i<10;i++) {
        anchor=(V41_2DF410 *)(D_L00_001E6290_2DF410+i*16);
        previous=(V41_2DF410 *)(D_L00_001E6280_2DF410+i*16);
        subtract_vector_xyz_2DF410(&tmp_a,anchor,previous);
        qcopy(&tmp_b,&step);
        cross_vectors_xyz_2DF410(&tmp_c,&tmp_b,&tmp_a);
        dot=fast_vec_dot_2DF410(&tmp_a,&tmp_b);
        { float target=1.5707964f-fast_arc_sin_2DF410(dot/(fast_vec_length_2DF410(&tmp_a)*D_L00_00161C54_2DF410));
        turn=func_L00_001EB6A8_2DF410(&D_L00_001E6330_2DF410[i],0.0f,target,0.06f,0.2f,0.0f);
        delta=fast_subtract_rotations_2DF410(target,turn); }
        if(absolute_float_2DF410(delta)>0.31415927f)turn=fast_add_rotations_2DF410(turn,fast_subtract_rotations_2DF410(delta,0.31415927f));
        func_002156E0_2D9EA0_2DF410(&motion,&tmp_a,turn,&tmp_c);
        fast_vec_normalize_2DF410(&tmp_d,&motion,D_L00_00161C54_2DF410);
        lerp_vector_2DF410(&motion,&motion,&tmp_d,1.0f);
        { float len=fast_vec_length_2DF410(&motion);
        if(len>2.0f)fast_vec_scale_2DF410(&motion,&motion,2.0f/len); }
        if(i>=6)lerp_vector_2DF410(&motion,&motion,&tmp_a,(float)i/convert_integer_to_float_2DF410(10)+0.0f);
        add_vector_xyz_2DF410(anchor,previous,&motion);
        qcopy_nc(&tmp_e,&step);
        qcopy_nc(&basis_c,&axis);
        fast_vec_normalize_2DF410(&interp,&tmp_e,1.0f);
        cross_vectors_xyz_2DF410(&cross_c,&interp,&basis_c);
        fast_vec_normalize_2DF410(&cross_c,&cross_c,1.0f);
        cross_vectors_xyz_2DF410(&basis_c,&cross_c,&interp);
        qcopy(&axis,&basis_c);
        radius=(float)i*0.3f;
        phase=(float)i*0.7f;
        { V41_2DF410 *dst=((V41_2DF410 *)D_L00_001E6370_2DF410)+i*20; unsigned int j;
        for(j=0;j<20;j++) {
            angle=radius+0.1f;
            angle+=fast_sin_2DF410(fast_add_rotations_2DF410((*(float *)&D_L00_00161C28_2DF410)+phase,0.0f))*0.1f;
            fast_vec_scale_2DF410(&tmp_f,(char *)D_L00_001E4E90_2DF410+j*16,angle);
            transform_vector_by_basis_2DF410(&tmp_f,&tmp_f,&interp);
            add_vector_xyz_2DF410(dst+j,D_L00_001E6290_2DF410+i*16,&tmp_f);
        }
        }
        fast_vec_normalize_2DF410(&step,&motion,D_L00_00161C54_2DF410);
    }
    }
    enqueue_callback_list_1_alt_2DF410(func_L00_002DED98_2DF410,(int)moby);
    { int count,sparks;
    count=0; sparks=0;
    for(i=0;i<200;i++) {
        if(!D_L00_001E4FD0_2DF410[i].p && count<5) {
            for(segment=9;segment>=0 && D_L00_001E6358_2DF410[segment]<64;segment--);
            if(segment>0) {
                if(segment>=2 && sparks<2) {
                    { int k=random_integer_below_2DF410(20);
                    amount=random_float_between_2DF410(0.2f,1.0f);
                    k=(int)&((V41_2DF410 *)D_L00_001E6370_2DF410)[k];
                    lerp_vector_2DF410(&tmp_a,(V41_2DF410 *)k,(V41_2DF410 *)k+segment*20,amount); }
                    lerp_vector_2DF410(&tmp_c,D_L00_001E6290_2DF410,D_L00_001E6290_2DF410+segment*16,amount);
                    speed=random_float_between_2DF410(60000.0f,180000.0f);
                    p=new_particle41_2DF410(&tmp_a,0.0f,1.0f,1.03f,random_integer_below_2DF410(5)+1,&tmp_b,speed,0x404040);
                    if(p) {
                        Tail41_2DF410 *tail=&p->tail;
                        subtract_vector_xyz_2DF410(&motion,&tmp_a,&tmp_c);
                        sparks++;
                        fast_vec_normalize_2DF410(&tail->velocity,&motion,0.01f);
                        tail->velocity.f.z+=0.02f;
                    }
                }
                qcopy(&tmp_a,D_L00_001E6290_2DF410+segment*16);
                clear_u64_value_2DF410(&tmp_b);
                speed=random_float_between_2DF410(60000.0f,180000.0f);
                { Particle41_2DF410 *main=new_particle41_2DF410(&tmp_a,0.0f,1.0f,1.0f,6,&tmp_b,speed,0x7F404040);
                D_L00_001E4FD0_2DF410[i].p=(char *)main;
                if(main) { Tail41_2DF410 *tail=&main->tail; main->life=200; tail->mode=2; count++; tail->r=127; tail->g=200; } }
                D_L00_001E4FD0_2DF410[i].idx=segment;
                D_L00_001E4FD0_2DF410[i].f=0;
                D_L00_001E4FD0_2DF410[i].phase=0;
                D_L00_001E4FD0_2DF410[i].angle=random_integer_below_2DF410(20);
                D_L00_001E4FD0_2DF410[i].phase_step=random_float_between_2DF410(0.1f,0.8f);
                D_L00_001E4FD0_2DF410[i].speed=random_float_between_2DF410(0.1f,0.4f);
            }
        }
        if(D_L00_001E4FD0_2DF410[i].p) {

            subtract_vector_xyz_2DF410(&tmp_a,D_L00_001E6280_2DF410+D_L00_001E4FD0_2DF410[i].idx*16,D_L00_001E6290_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
            length=fast_vec_length_2DF410(&tmp_a);
            D_L00_001E4FD0_2DF410[i].f+=D_L00_001E4FD0_2DF410[i].speed;
            ((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->life=200;
            if(length<D_L00_001E4FD0_2DF410[i].f) {
                D_L00_001E4FD0_2DF410[i].idx=(u16)D_L00_001E4FD0_2DF410[i].idx-1;
                if(D_L00_001E4FD0_2DF410[i].idx<=0) { func_L00_002688A8_2DF410(D_L00_001E4FD0_2DF410[i].p); D_L00_001E4FD0_2DF410[i].p=0; }
                else {
                    D_L00_001E4FD0_2DF410[i].f-=length;
                    subtract_vector_xyz_2DF410(&tmp_a,D_L00_001E6280_2DF410+D_L00_001E4FD0_2DF410[i].idx*16,D_L00_001E6290_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
                    length=fast_vec_length_2DF410(&tmp_a);
                }
            }
                if(D_L00_001E4FD0_2DF410[i].p) {

                amount=D_L00_001E4FD0_2DF410[i].f/length;
                if(D_L00_001E4FD0_2DF410[i].idx==2)((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->life=100;
                else if(D_L00_001E4FD0_2DF410[i].idx<2) {
                    ((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->life=truncate_float_to_s32_2DF410((1.0f-amount)*100.0f);
                    if(((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->life<61)((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->life=60;
                }

                if(D_L00_001E4FD0_2DF410[i].idx==1) {
                    subtract_vector_xyz_2DF410(&tmp_b,position,&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos);
                    fast_vec_normalize_2DF410(&tmp_b,&tmp_b,D_L00_001E4FD0_2DF410[i].speed);
                    add_vector_xyz_2DF410(&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos,&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos,&tmp_b);
                    ((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->size*=1.0f+D_0015EE60_2DF410*-0.0500000119f;
                } else {
                    fast_vec_scale_2DF410(&tmp_a,&tmp_a,amount);
                    add_vector_xyz_2DF410(&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos,D_L00_001E6290_2DF410+D_L00_001E4FD0_2DF410[i].idx*16,&tmp_a);
                    j=D_L00_001E4FD0_2DF410[i].angle;
                    subtract_vector_xyz_2DF410(&tmp_b,(char *)((V41_2DF410 *)D_L00_001E6370_2DF410)+D_L00_001E4FD0_2DF410[i].idx*320+j*16,D_L00_001E6290_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
                    subtract_vector_xyz_2DF410(&tmp_c,(char *)((V41_2DF410 *)D_L00_001E6230_2DF410)+D_L00_001E4FD0_2DF410[i].idx*320+j*16,D_L00_001E6280_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
                    func_L00_0025C710_2DF410(&motion,&tmp_b,&tmp_c,amount);
                    j=(j+19)%20;
                    subtract_vector_xyz_2DF410(&tmp_d,(char *)((V41_2DF410 *)D_L00_001E6370_2DF410)+D_L00_001E4FD0_2DF410[i].idx*320+j*16,D_L00_001E6290_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
                    subtract_vector_xyz_2DF410(&tmp_e,(char *)((V41_2DF410 *)D_L00_001E6230_2DF410)+D_L00_001E4FD0_2DF410[i].idx*320+j*16,D_L00_001E6280_2DF410+D_L00_001E4FD0_2DF410[i].idx*16);
                    func_L00_0025C710_2DF410(&interp,&tmp_d,&tmp_e,amount);
                    func_L00_0025C710_2DF410(&motion,&motion,&interp,D_L00_001E4FD0_2DF410[i].phase);
                    D_L00_001E4FD0_2DF410[i].phase+=D_L00_001E4FD0_2DF410[i].phase_step;
                    if(D_L00_001E4FD0_2DF410[i].phase>=1.0f) { D_L00_001E4FD0_2DF410[i].phase-=1.0f; D_L00_001E4FD0_2DF410[i].angle=j; }
                    add_vector_xyz_2DF410(&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos,&((Particle41_2DF410 *)D_L00_001E4FD0_2DF410[i].p)->pos,&motion);
                }
            }
        }
    }
}
}
