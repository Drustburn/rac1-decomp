/* NON_MATCHING func_0022FDC0 -- src/game/space.c
 * Best so far: BYTES 438/2012 (same size; 78% of the bytes match), checked 2026-10-21.
 * Ported from Lombyte's match of the US build (src/gameplay/state/update_level_gameplay_frame.c,
 * update_level_gameplay_frame), which uses another compiler (a rebuilt 2.9-ee-991111b), with PAL's
 * symbols and this build's addressing: D_0015EF4A as D_0015EF48[1] through a 4-byte MACRO_ADDR
 * alias (an offset keeps the store out of the branch-likely slot, the size keeps it off $gp),
 * D_00160560 sized so it stays off $gp, the two C names of D_0018CC20 as in Lombyte.
 * What is left is register allocation: retail keeps %hi(D_0018CC20) in $s3 and %hi(D_0013E130) in
 * $s7 (shared with `subframe` later), &render_sequence in $s1 across func_001F99B0; ours ranks
 * them differently ($s4/$s3/$s2), which shifts every saved register and the archive loop's
 * temporaries. A permuter run (25 min) found nothing natural: reading current_vu_chain into a
 * local before the history resets moves 18 registers but costs 8 bytes (a saved register).
 */
typedef unsigned int u128_FDC0 __attribute__((mode(TI)));
typedef union {
    u128_FDC0 quadword;
    f32 components[4];
} Vector4_FDC0;
typedef struct {
    Vector4_FDC0 rows[4];
} Matrix4x4_FDC0;

typedef struct {
    u8 pad00[0xC];
    u8 count; /* 0x0C */
    u8 pad0D[0x24 - 0x0D];
    f32 base_scale; /* 0x24 */
    u8 pad28[0x48 - 0x28];
    s32 frames[1]; /* 0x48 */
} RenderModel_FDC0;

typedef struct {
    u8 pad00[0x10];
    u8 position[0x14];        /* 0x10 */
    RenderModel_FDC0 *model;  /* 0x24 */
    u8 pad28[4];
    f32 fade; /* 0x2C */
    u8 pad30[2];
    s16 selected_index; /* 0x32 */
    u16 flags;          /* 0x34 */
    u8 pad36[0x50 - 0x36];
    u8 state_a; /* 0x50 */
    u8 state_b;
    u8 selected_a;
    u8 selected_b;
    f32 blend; /* 0x54 */
    u8 pad58[0x68 - 0x58];
    u8 *frame_data_a; /* 0x68 */
    u8 *frame_data_b; /* 0x6C */
    u8 pad70;
    u8 cached_selector;
    u8 opacity;
    u8 pad73[5];
    u8 *animation_positions; /* 0x78 */
} LevelRenderObject_FDC0;

typedef struct {
    u8 pad00[0x34];
    s32 time;           /* 0x34 */
    s32 frame;          /* 0x38 */
    s32 sequence_frame; /* 0x3C */
    s16 end;            /* 0x40 */
    u8 pad42[2];
    s16 count; /* 0x44 */
    u8 pad46[0x58 - 0x46];
    s32 source_begin;                   /* 0x58 */
    s32 source_end;                     /* 0x5C */
    s32 prepared_frames[0x46];          /* 0x60 */
    LevelRenderObject_FDC0 *objects[1]; /* 0x178 */
} LevelGameplayState_FDC0;

typedef struct {
    u8 pad00[0x50];
    s32 history_index; /* 0x50 */
    s32 history_count; /* 0x54 */
    s32 mode;          /* 0x58 */
    s32 state;         /* 0x5C */
    u8 pad60[0xC0 - 0x60];
    Vector4_FDC0 primary_history_positions[32];   /* 0xC0 */
    Vector4_FDC0 secondary_history_positions[32]; /* 0x2C0 */
} LevelRenderState_FDC0;

typedef struct {
    u8 pad00[0xB0];
    f32 projection_scale;
} LevelProjectionState_FDC0;

typedef struct {
    s32 offset;
    s32 available;
} RenderArchiveEntry_FDC0;

typedef struct {
    u8 pad00[4];
    s32 data_offset; /* 0x04 */
    u8 pad08[0x50 - 0x08];
    s32 scene_offsets[5]; /* 0x50 */
} RenderArchiveTable_FDC0;

typedef struct {
    u8 pad00[4];
    s32 source_begin_offset;
    s32 source_end_offset;
    u8 pad0C[8];
    RenderArchiveTable_FDC0 *archive_table; /* 0x14 */
} LevelRenderArchive_FDC0;

typedef struct {
    u32 w0;
    u32 w1;
} ColorPair_FDC0;

extern s32 current_level_FDC0 __asm__("D_0015EE84") MACRO_ADDR;
extern u8 level_available_FDC0[20] __asm__("D_0013DE48") NOT_SDA;
extern LevelRenderState_FDC0 level_render_state_FDC0 __asm__("D_0013E130");
extern u8 voice_volume_records_FDC0[] __asm__("D_0013E6C0") MACRO_ADDR;
extern s32 sound_bank_handle_FDC0 __asm__("D_0015EE5C") MACRO_ADDR;
extern s16 D_0015EF48_FDC0 SDATA(D_0015EF48);
extern s16 D_0015EF48_macro_FDC0[2] __asm__("D_0015EF48") MACRO_ADDR;
extern f32 D_0015F53C MACRO_ADDR;
extern s32 space_load_in_progress_FDC0 __asm__("D_0015F6FC") MACRO_ADDR;
extern f32 D_00160504_FDC0 __asm__("D_00160504") MACRO_ADDR;
extern u8 D_00160560_FDC0[16] __asm__("D_00160560");
extern s32 D_001605E0_FDC0 SDATA(D_001605E0);
extern s32 D_001605F0_FDC0 SDATA(D_001605F0);
extern s32 D_00160600_FDC0 SDATA(D_00160600);
extern s32 current_vu_chain_FDC0 __asm__("D_0016100C");
extern LevelGameplayState_FDC0 render_sequence_FDC0 __asm__("D_0018CC20");
/* A second name for render_sequence: the fade loop takes its own lui/addiu (docs/DECOMP_PROGRESS.md,
   "Two C names on one asm symbol defeat address CSE"). */
extern LevelGameplayState_FDC0 render_sequence_lui_FDC0 __asm__("D_0018CC20");
extern LevelRenderObject_FDC0 *D_0018CD98_FDC0[] __asm__("D_0018CD98");
extern LevelProjectionState_FDC0 view_context_FDC0 __asm__("D_0018CE00");
extern LevelRenderArchive_FDC0 D_001941C0_FDC0 __asm__("D_001941C0");
extern ColorPair_FDC0 warp_trail_colors_FDC0[] __asm__("D_001D9DC0");
extern u128_FDC0 D_001D9E70_FDC0[] __asm__("D_001D9E70");
extern f32 D_001D9EC0_FDC0[] __asm__("D_001D9EC0");
extern f32 cam_mode_min_fov_FDC0[] __asm__("D_001D9ED8");

extern void fill_transfer_words_FDC0(void *, s32, s32) __asm__("func_001F99B0");
extern void read_global_table_entry_FDC0(void) __asm__("func_0012EC30");
extern void snd_flush_sound_commands_FDC0(void) __asm__("func_0012DDC0");
extern void snd_play_sound_FDC0(s32, s32, s32, s32, s32, s32, s32, void *) __asm__("func_0012E448");
extern void snd_reset_state_FDC0(void) __asm__("func_0012EC40");
extern void noop_callback_h_FDC0(void) __asm__("func_001E97A8");
extern void noop_callback_i_FDC0(void) __asm__("func_001E97B0");
extern void update_view_context_FDC0(void) __asm__("func_001F3140");
extern s32 scale_ticks_FDC0(s32) __asm__("func_001F98C0");
extern u32 fast_mem_copy_FDC0(void *, void *, s32) __asm__("func_001F9A98");
extern void add_vectors_FDC0(void *, void *, void *) __asm__("func_001F9BD8");
extern void scale_vector_FDC0(void *, void *, f32) __asm__("func_001F9C30");
extern f32 int_to_float_FDC0(s32) __asm__("func_001FA888");
extern void parse_space_scene_chunk_FDC0(s32) __asm__("func_00205220");
extern void mark_moby_for_removal_FDC0(LevelRenderObject_FDC0 *) __asm__("func_0020D678");
extern void update_moby_animation_state_FDC0(LevelRenderObject_FDC0 *) __asm__("func_0020D6D0");
extern void calculate_object_transform_FDC0(LevelRenderObject_FDC0 *, s32, void *) __asm__("func_0020DAF8");
extern void refresh_moby_spatial_bounds_FDC0(LevelRenderObject_FDC0 *) __asm__("func_0020ED48");
extern void build_object_rotation_matrix_FDC0(void) __asm__("func_0022F128");
extern s32 rand_FDC0(void) __asm__("func_001160D8");

/* One frame of the space flight between planets: steps the fade and the sequence's frame and time counters,
   starts the flight sound on the first frame, and when the sequence ends releases its objects and picks the
   next flight mode and its scene from the render archive (or, after the last one, flags the load as done);
   then places the camera and, for every object of the sequence, interpolates its position between two frames
   of its path twice per frame, records the bone positions in the trail history and fades the warp trail out
   over the last 0x38 ticks. Adapted from Lombyte (GPL-3.0): src/gameplay/state/update_level_gameplay_frame.c,
   update_level_gameplay_frame (US 0x22EAA8). */
void func_0022FDC0(void) {
    Vector4_FDC0 first_position;
    Vector4_FDC0 second_position;
    Matrix4x4_FDC0 first_transform;
    Matrix4x4_FDC0 second_transform;
    u16 transform_count;
    LevelRenderObject_FDC0 *object;
    LevelRenderObject_FDC0 *expired_object;
    s32 mode_advance;
    RenderModel_FDC0 *model;
    RenderArchiveTable_FDC0 *archive_table;
    u8 *archive_data;
    RenderArchiveEntry_FDC0 *archive_entry;
    s32 expired_object_index;
    s32 prepared_frame_index;
    s32 payload_offset;
    u8 *frame_offsets;
    s32 *current_frame_pointer;
    s32 *next_frame_pointer;
    s32 subframe;
    s32 color_index;
    s32 fill_index;
    s32 history_slot;
    s32 next_frame_payload;
    s32 current_frame_payload;
    s32 frame;
    u8 *animation_positions;
    s32 current_frame;
    s32 next_frame;
    f32 blend;
    f32 scale;
    s32 object_index;

    noop_callback_i_FDC0();
    D_0015F53C -= 0.25f;
    render_sequence_FDC0.frame++;
    render_sequence_FDC0.time++;
    if (D_0015F53C < 0.0f) {
        D_0015F53C = 0.0f;
    }
    if (render_sequence_FDC0.time == 1 && current_level_FDC0 != 0 &&
        (current_level_FDC0 != 1 || level_available_FDC0[3] != 0)) {
        read_global_table_entry_FDC0();
        snd_play_sound_FDC0(sound_bank_handle_FDC0, level_render_state_FDC0.mode, 0x400, 0, 0, 0, 0,
                            voice_volume_records_FDC0);
        snd_reset_state_FDC0();
        snd_flush_sound_commands_FDC0();
    }
    if (render_sequence_FDC0.time >= render_sequence_FDC0.end) {
        for (expired_object_index = 0; expired_object_index < render_sequence_FDC0.count;
             expired_object_index++) {
            expired_object = render_sequence_FDC0.objects[expired_object_index];
            if (expired_object != 0) {
                expired_object->model->count--;
                expired_object->model->frames[expired_object->model->count] = 0;
                mark_moby_for_removal_FDC0(expired_object);
            }
        }
        if (current_level_FDC0 != 0 && (current_level_FDC0 != 1 || level_available_FDC0[3] != 0) &&
            D_0015EF48_FDC0 < 3) {
            level_render_state_FDC0.state = 0;
        }
        if (level_render_state_FDC0.state < 2) {
            if (level_render_state_FDC0.state == 0) {
                mode_advance = (rand_FDC0() >> 16) % 3 + 1;
                level_render_state_FDC0.mode = (level_render_state_FDC0.mode + mode_advance) & 3;
            } else {
                level_render_state_FDC0.mode = 4;
            }
            level_render_state_FDC0.history_index = 0;
            level_render_state_FDC0.history_count = 0;
            level_render_state_FDC0.state++;
            qcopy(&D_001605F0_FDC0, &D_001605E0_FDC0);
            fill_transfer_words_FDC0(&render_sequence_FDC0, 0, 0x1C0);
            render_sequence_FDC0.source_begin = D_001941C0_FDC0.source_begin_offset + current_vu_chain_FDC0;
            render_sequence_FDC0.source_end = D_001941C0_FDC0.source_end_offset + current_vu_chain_FDC0;
            archive_table = D_001941C0_FDC0.archive_table;
            archive_data = (u8 *)archive_table + archive_table->data_offset;
            archive_entry = (RenderArchiveEntry_FDC0 *)(archive_data +
                                                        archive_table->scene_offsets[level_render_state_FDC0.mode]);
            for (prepared_frame_index = 0; prepared_frame_index < 0x46 && archive_entry->available != 0;
                 prepared_frame_index++, archive_entry++) {
                payload_offset = archive_entry->offset + 0x800;
                render_sequence_FDC0.prepared_frames[prepared_frame_index] =
                    (s32)(archive_data + archive_table->scene_offsets[level_render_state_FDC0.mode]) +
                    payload_offset;
            }
            parse_space_scene_chunk_FDC0(0);
        } else {
            if (D_0015EF48_macro_FDC0[1] != 0) {
                D_0015EF48_macro_FDC0[1] = 0;
            }
            space_load_in_progress_FDC0 = 1;
            return;
        }
    } else if (render_sequence_FDC0.frame >= 0x60) {
        parse_space_scene_chunk_FDC0(++render_sequence_FDC0.sequence_frame);
    }
    build_object_rotation_matrix_FDC0();
    if (view_context_FDC0.projection_scale < cam_mode_min_fov_FDC0[level_render_state_FDC0.mode]) {
        view_context_FDC0.projection_scale = cam_mode_min_fov_FDC0[level_render_state_FDC0.mode];
    }
    update_view_context_FDC0();
    scale = 1.0f;
    if (level_render_state_FDC0.mode == 4) {
        scale = (f32)(render_sequence_FDC0.end - render_sequence_FDC0.time) / (f32)render_sequence_FDC0.end;
        scale_vector_FDC0(&first_position, &D_00160600_FDC0, scale);
        add_vectors_FDC0(&D_001605F0_FDC0, &D_001605F0_FDC0, &first_position);
    }
    scale_vector_FDC0(D_00160560_FDC0, &D_001D9E70_FDC0[level_render_state_FDC0.mode],
                      (f32)(render_sequence_FDC0.time - scale_ticks_FDC0(0x78)) * 20.0f * scale);
    D_00160504_FDC0 = D_001D9EC0_FDC0[level_render_state_FDC0.mode];
    for (object_index = 0; object_index < render_sequence_FDC0.count; object_index++) {
        object = D_0018CD98_FDC0[object_index];
        for (subframe = 0; subframe < 2; subframe++) {
            frame = render_sequence_FDC0.frame;
            current_frame = frame >> 1;
            next_frame = current_frame + 1;
            model = object->model;
            frame_offsets = (u8 *)model->frames[model->count - 1] + 0x1C;
            current_frame_pointer = (s32 *)(frame_offsets + current_frame * 4);
            next_frame_pointer = (s32 *)(frame_offsets + next_frame * 4);
            current_frame_payload = *current_frame_pointer + 0x10;
            next_frame_payload = *next_frame_pointer + 0x10;
            blend = int_to_float_FDC0(frame & 1) * 0.5f + (f32)subframe * 0.25f;
            object->blend = blend;
            animation_positions = object->animation_positions;
            scale_vector_FDC0(&first_position, animation_positions + current_frame * 16, 1.0f - blend);
            scale_vector_FDC0(&second_position, animation_positions + next_frame * 16, object->blend);
            transform_count = 2;
            add_vectors_FDC0(object->position, &first_position, &second_position);
            object->selected_a = 2;
            object->selected_b = transform_count;
            object->state_a = 0;
            object->state_b = 0;
            update_moby_animation_state_FDC0(object);
            fast_mem_copy_FDC0(object->frame_data_a + 0x10, (void *)current_frame_payload, 0x20);
            fast_mem_copy_FDC0(object->frame_data_b + 0x10, (void *)next_frame_payload, 0x20);
            object->opacity = 0xFF;
            object->selected_index = 0x1FF;
            object->cached_selector = 0xFF;
            refresh_moby_spatial_bounds_FDC0(object);
            object->selected_a = object->model->count - 1;
            object->selected_b = object->model->count - 1;
            calculate_object_transform_FDC0(object, 1, &first_transform);
            calculate_object_transform_FDC0(object, 2, &second_transform);
            level_render_state_FDC0.history_index = (level_render_state_FDC0.history_index + 1) & 0x1F;
            if (level_render_state_FDC0.history_count < 0x20) {
                level_render_state_FDC0.history_count++;
            }
            history_slot = level_render_state_FDC0.history_index;
            qcopy(&level_render_state_FDC0.primary_history_positions[history_slot], &first_transform.rows[3]);
            qcopy(&level_render_state_FDC0.secondary_history_positions[history_slot], &second_transform.rows[3]);
            if (level_render_state_FDC0.mode == 4) {
                if (current_level_FDC0 == 0 || (current_level_FDC0 == 1 && level_available_FDC0[3] == 0)) {
                    object->flags |= 1;
                    level_render_state_FDC0.history_count = 0;
                }
                if (render_sequence_FDC0.time > render_sequence_FDC0.end - 0x38) {
                    object->fade = object->model->base_scale *
                                   ((f32)(render_sequence_FDC0.end - render_sequence_FDC0.time) * 0.017857144f);
                    for (color_index = 0; color_index < 3; color_index++) {
                        warp_trail_colors_FDC0[color_index].w0 =
                            (warp_trail_colors_FDC0[color_index].w0 & 0xFFFFFF) |
                            ((render_sequence_lui_FDC0.end - render_sequence_FDC0.time) << 24);
                    }
                } else {
                    for (fill_index = 0; fill_index < 3; fill_index++) {
                        warp_trail_colors_FDC0[fill_index].w0 =
                            (warp_trail_colors_FDC0[fill_index].w0 & 0xFFFFFF) | 0x38000000;
                    }
                }
            }
        }
    }
    noop_callback_h_FDC0();
}
