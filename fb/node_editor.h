/* nuklear - v1.00 - public domain */
/* This is a simple node editor just to show a simple implementation and that
 * it is possible to achieve it with this library. While all nodes inside this
 * example use a simple color modifier as content you could change them
 * to have your custom content depending on the node time.
 * Biggest difference to most usual implementation is that this example does
 * not have connectors on the right position of the property that it links.
 * This is mainly done out of laziness and could be implemented as well but
 * requires calculating the position of all rows and add connectors.
 * In addition adding and removing nodes is quite limited at the
 * moment since it is based on a simple fixed array. If this is to be converted
 * into something more serious it is probably best to extend it.*/
#include "type_def.h"
#define NK_IMPLEMENTATION
#include "nuklear.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "fb_references.h"
#define MAX_FB_NUM 1024
#define MAX_FB_REGISTERS 1024
#define MAX_REGISTERS (MAX_FB_NUM*MAX_FB_REGISTERS)
static reg_t regs_buffer[MAX_REGISTERS] = {0};
static reg_t local_regs_buffer[MAX_REGISTERS] = {0};


static int kernel_lap(void);
struct node {
    int ID;
    char name[32];
    struct nk_rect bounds;
    int input_count;
    int local_count;
    int output_count;
    u32 * input_locations;
    u32 * local_locations;
    u32 * output_locations;
};

struct node_link {
    int input_id;
    int input_slot;
    int output_id;
    int output_slot;
    struct nk_vec2 in;
    struct nk_vec2 out;
};

struct node_linking {
    int active;
    struct node *node;
    int input_id;
    int input_slot;
};

struct node_editor {
    int initialized;
    struct node node_buf[MAX_FB_NUM];
    struct node_link links[MAX_FB_REGISTERS*MAX_FB_NUM];
    int node_count;
    int link_count;
    struct nk_rect bounds;
    struct node *selected;
    int show_grid;
    struct nk_vec2 scrolling;
    struct node_linking linking;
};
static struct node_editor nodeEditor;

static struct node* node_editor_find(struct node_editor *editor, int ID){
    for (int i=0;i<editor->node_count;i++){
        if (editor->node_buf[i].ID == ID){
            return &editor->node_buf[i];
        }
    }
    return NULL;
}
static const fb_cals_t * get_fb_description(u32 fb_number){
    for (u32 i=0;i<FB_NUMBER;i++){
        if(fb_cals[i].number == fb_number){
            return &fb_cals[i];
        }
    }
    return NULL;
}
static u32 local_reg_number = 0;
static u32 get_current_local_register(){
    return local_reg_number;
}
static int appoint_registers(u32 regs_number,u32 * locations, reg_flag_t * flags){
    for (u8 i=0;i<regs_number;i++){
        if (locations[i]<MAX_REGISTERS){
            if (regs_buffer[locations[i]].reg_flag !=0 && 
                regs_buffer[locations[i]].reg_flag !=flags[i]){
                printf("reg type mismatch");
            }else{
                regs_buffer[locations[i]].reg_flag = flags[i];
            }
        }else{
            printf("lcoation error");
        }
    }
}

static int appoint_local_registers(u32 regs_number,u32 * local_locations, reg_flag_t * flags){
    for (u8 i=0;i<regs_number;i++){
        local_locations[i] = local_reg_number;
        local_regs_buffer[local_reg_number].reg_flag = flags[i];
        local_reg_number++;
    }
}
static int node_editor_add(struct node_editor *editor, struct nk_rect bounds,
    int fb_number,u32 * input_locations, u32 * output_locations){
    fb_cals_t const * fb_cal = get_fb_description(fb_number);
    int id = -1;
    if(fb_cal!=NULL){
        id = editor->node_count;
        struct node *node;
        NK_ASSERT((nk_size)editor->node_count < NK_LEN(editor->node_buf));
        node = &editor->node_buf[editor->node_count];
        editor->node_count++;
        node->ID = id;
        node->input_count = fb_cal->input_regs;
        node->input_locations = malloc(fb_cal->input_regs*sizeof(u32));
        memcpy(node->input_locations,input_locations,fb_cal->input_regs*sizeof(u32));
        appoint_registers(node->input_count,input_locations,fb_cal->input_flags);
        node->local_count = fb_cal->local_regs;
        node->local_locations = malloc(fb_cal->local_regs*sizeof(u32));
        appoint_local_registers(node->local_count,node->local_locations,fb_cal->local_flags);
        node->output_count = fb_cal->output_regs;
        node->output_locations = malloc(fb_cal->output_regs*sizeof(u32));
        memcpy(node->output_locations,output_locations,fb_cal->output_regs*sizeof(u32));
        appoint_registers(node->output_count,output_locations,fb_cal->output_flags);
        node->bounds = bounds;
        strncpy(node->name, fb_cal->name, 30);
    }else{
        printf("fb error %u",fb_number);
    }
    return id;
}
static int find_output(struct node_editor *editor,u32 location,u32 * fb_id,u32 * number){
    for (u32 i =0;i<editor->node_count;i++){
        for (u32 j =0;j<editor->node_buf[i].output_count;j++){
            if(editor->node_buf[i].output_locations[j]==location){
                *fb_id = editor->node_buf[i].ID;
                *number = j;
                return 1;
            }
        }
    }
    return 0;
}
static void node_editor_add_links(struct node_editor *editor){
    struct node_link *link;
    for (u32 i =0;i<editor->node_count;i++){
        for (u32 j =0;j<editor->node_buf[i].input_count;j++){
            u32 fb_id = 0;u32 number = 0;
            if (find_output(editor,editor->node_buf[i].input_locations[j],&fb_id,&number)>0){
                NK_ASSERT((nk_size)editor->link_count < NK_LEN(editor->links));
                link = &editor->links[editor->link_count++];
                link->input_id = fb_id;
                link->input_slot = number;
                link->output_id = editor->node_buf[i].ID;
                link->output_slot = j;
            }
        }
    }
}
static int time_id;
static void node_editor_init(struct node_editor *editor){
    memset(editor, 0, sizeof(*editor));
    u32 locations_input[10] = {0,1,2,3};
    u32 locations_output[10] = {4};
    node_editor_add(editor, nk_rect(40, 10, 90, 90), 1,locations_input,locations_output);
    locations_input[0]=5;locations_input[1]=6;locations_input[2]=7;locations_input[3]=4;
    locations_output[0]=8;
    node_editor_add(editor, nk_rect(40, 260, 90, 90), 2,locations_input,locations_output);
    locations_input[0]=9;locations_input[1]=10;
    locations_output[0]=0;
    node_editor_add(editor, nk_rect(400, 10, 90, 90), 101,locations_input,locations_output);
    locations_input[0]=11;locations_input[1]=12;
    locations_output[0]=1;
    time_id = node_editor_add(editor, nk_rect(400, 260, 90, 90),101, locations_input,locations_output);
    node_editor_add_links(editor);
    editor->show_grid = nk_true;
}

static int node_editor(struct nk_context *ctx){
    int n = 0;
    const int TIME_PERIOD_MS = 100;
    time_t rawtime;
    static clock_t timer = 0;
    int kernel_laps=0;
    struct nk_rect total_space;
    const struct nk_input *in = &ctx->input;
    struct nk_command_buffer *canvas;
    struct node_editor *nodedit = &nodeEditor;

    if (!nodeEditor.initialized) {
        node_editor_init(&nodeEditor);
        nodeEditor.initialized = 1;
    }

    if (nk_begin(ctx, "NodeEdit", nk_rect(0, 0, 800, 600),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))    {
        /* allocate complete window space */
        canvas = nk_window_get_canvas(ctx);
        total_space = nk_window_get_content_region(ctx);
        nk_layout_space_begin(ctx, NK_STATIC, total_space.h, nodedit->node_count);
        {
            struct node *it = &nodedit->node_buf[0];
            struct nk_rect size = nk_layout_space_bounds(ctx);
            struct nk_panel *node = 0;
            if (nodedit->show_grid) {
                /* display grid */
                float x, y;
                const float grid_size = 32.0f;
                const struct nk_color grid_color = nk_rgb(50, 50, 50);
                for (x = (float)fmod(size.x - nodedit->scrolling.x, grid_size); x < size.w; x += grid_size)
                    nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color);
                for (y = (float)fmod(size.y - nodedit->scrolling.y, grid_size); y < size.h; y += grid_size)
                    nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color);
            }
            /* execute each node as a movable group */
            printf("aga %u %u",clock(),timer);
            if (clock()-timer>TIME_PERIOD_MS){
                kernel_laps++;
                printf("kernel lap");
                timer = clock();
                if(kernel_lap()<0){
                    printf("kernel lap error");
                }else{
                    printf("kernel lap %i",kernel_laps);
                }
            }

            for (int i=0;i<nodedit->node_count;i++){
                it = &nodedit->node_buf[i];            
                /* calculate scrolled node window position and size */
                nk_layout_space_push(ctx, nk_rect(it->bounds.x - nodedit->scrolling.x,
                    it->bounds.y - nodedit->scrolling.y, it->bounds.w, it->bounds.h));
                /* execute node window */
                if (nk_group_begin(ctx, it->name, NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER|NK_WINDOW_TITLE))                {
                    /* always have last selected node on top */
                    node = nk_window_get_panel(ctx);


                    /* ================= NODE CONTENT =====================*/
                    nk_layout_row_dynamic(ctx, 25, 1);
                    int property = 0;
                    nk_property_int(ctx, "value:", 0, &property, 100, 10, 1);

                    /* ====================================================*/
                    nk_group_end(ctx);
                }
                {
                    /* node connector and linking */
                    float space;
                    struct nk_rect bounds;
                    bounds = nk_layout_space_rect_to_local(ctx, node->bounds);
                    bounds.x += nodedit->scrolling.x;
                    bounds.y += nodedit->scrolling.y;
                    it->bounds = bounds;
                    /* output connector */
                    space = node->bounds.h / (float)((it->output_count) + 1);
                    for (n = 0; n < it->output_count; ++n) {
                        struct nk_rect circle;
                        circle.x = node->bounds.x + node->bounds.w-4;
                        circle.y = node->bounds.y + space * (float)(n+1);
                        circle.w = 8; circle.h = 8;
                        nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));
                        /* start linking process */
                        if (nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, circle, nk_true)) {
                            nodedit->linking.active = nk_true;
                            nodedit->linking.node = it;
                            nodedit->linking.input_id = it->ID;
                            nodedit->linking.input_slot = n;
                        }
                        /* draw curve from linked node slot to mouse position */
                        if (nodedit->linking.active && nodedit->linking.node == it &&
                            nodedit->linking.input_slot == n) {
                            struct nk_vec2 l0 = nk_vec2(circle.x + 3, circle.y + 3);
                            struct nk_vec2 l1 = in->mouse.pos;
                            nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                                l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
                        }
                    }

                    /* input connector */
                    space = node->bounds.h / (float)((it->input_count) + 1);
                    for (n = 0; n < it->input_count; ++n) {
                        struct nk_rect circle;
                        circle.x = node->bounds.x-4;
                        circle.y = node->bounds.y + space * (float)(n+1);
                        circle.w = 8; circle.h = 8;
                        nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));
                    }
                }
            }
            /* reset linking connection */
            if (nodedit->linking.active && nk_input_is_mouse_released(in, NK_BUTTON_LEFT)) {
                nodedit->linking.active = nk_false;
                nodedit->linking.node = NULL;
                fprintf(stdout, "linking failed\n");
            }
            /* draw each link */
            for (n = 0; n < nodedit->link_count; ++n) {
                struct node_link *link = &nodedit->links[n];
                struct node *ni = node_editor_find(nodedit, link->input_id);
                struct node *no = node_editor_find(nodedit, link->output_id);
                float spacei = node->bounds.h / (float)((ni->output_count) + 1);
                float spaceo = node->bounds.h / (float)((no->input_count) + 1);
                struct nk_vec2 l0 = nk_layout_space_to_screen(ctx,
                    nk_vec2(ni->bounds.x + ni->bounds.w, 3.0f + ni->bounds.y + spacei * (float)(link->input_slot+1)));
                struct nk_vec2 l1 = nk_layout_space_to_screen(ctx,
                    nk_vec2(no->bounds.x, 3.0f + no->bounds.y + spaceo * (float)(link->output_slot+1)));
                l0.x -= nodedit->scrolling.x;
                l0.y -= nodedit->scrolling.y;
                l1.x -= nodedit->scrolling.x;
                l1.y -= nodedit->scrolling.y;
                nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                    l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
            }
            /* node selection */
            if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_layout_space_bounds(ctx))) {
                
                nodedit->selected = NULL;
                nodedit->bounds = nk_rect(in->mouse.pos.x, in->mouse.pos.y, 100, 200);
                for (int i=0;i<nodedit->node_count;i++){
                    it = &nodedit->node_buf[i];
                    struct nk_rect b = nk_layout_space_rect_to_screen(ctx, it->bounds);
                    b.x -= nodedit->scrolling.x;
                    b.y -= nodedit->scrolling.y;
                    if (nk_input_is_mouse_hovering_rect(in, b))
                        nodedit->selected = it;
                }
            }
            /* contextual menu */
            if (nk_contextual_begin(ctx, 0, nk_vec2(100, 220), nk_window_get_bounds(ctx))) {
                const char *grid_option[] = {"Show Grid", "Hide Grid"};
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_contextual_item_label(ctx, grid_option[nodedit->show_grid],NK_TEXT_CENTERED))
                    nodedit->show_grid = !nodedit->show_grid;
                nk_contextual_end(ctx);
            }
        }
        nk_layout_space_end(ctx);

        /* window content scrolling */
        if (nk_input_is_mouse_hovering_rect(in, nk_window_get_bounds(ctx)) &&
            nk_input_is_mouse_down(in, NK_BUTTON_MIDDLE)) {
            nodedit->scrolling.x += in->mouse.delta.x;
            nodedit->scrolling.y += in->mouse.delta.y;
        }
    }
    nk_end(ctx);
    return !nk_window_is_closed(ctx, "NodeEdit");
}
static int kernel_lap(){
    struct node * time_node = node_editor_find(&nodeEditor, time_id);
    return 0;
}
