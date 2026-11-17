/* ER-002 sanity test: hand-written guesses at three small Eternal Ring functions, to compare the
   compiler's output shape with the ELF. Names and types are Unofficial (invented); only the shape matters. */

/* Shape of func_0012F7E0: a wrapper that shifts its arguments and passes 0 as the second one. */
extern int func_0012B540(int a, int b, int c, int d);
int wrap_12f7e0(int a, int c, int d)
{
    return func_0012B540(a, 0, c, d);
}

/* Shape of func_001257E8: two small-data (gp-relative) globals, a call, a store back. */
struct obj { int pad; int f4; };
extern struct obj *D_gp_5328;
extern char *D_gp_52D0;
extern int func_001139C8(int a, char *b);
void update_1257e8(void)
{
    D_gp_5328->f4 = func_001139C8(D_gp_5328->f4, D_gp_52D0 + 0x1200);
}

/* Shape of func_0012D3B8: two float arguments packed into a 16-byte vector on the stack. */
typedef struct { float x, y, z, w; } vec4;
extern void func_0012D318(int a, vec4 *v);
void pack_12d3b8(int a, float x, float z)
{
    vec4 v;
    v.x = x;
    v.y = 0.0f;
    v.z = z;
    v.w = 0.0f;
    func_0012D318(a, &v);
}
